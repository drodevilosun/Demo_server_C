#include "server.hpp"

Server::Server() : mSocketServer(-1), mEpollFd(-1), mRunable(false) {}

Server::~Server() {
    if (mSocketServer != -1) {
        close(mSocketServer);
    }
    if (mEpollFd != -1) {
        close(mEpollFd);
    }
}

bool Server::Init(std::string serverPort) {
    SA_IN server_addr;
    logger.log("Init server");
    // create a socket
    mSocketServer = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocketServer < 0) {
        logger.log("Socket creation failed");
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoul(serverPort));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // bind the socket
    if (bind(mSocketServer, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        logger.log("Bind failed");
        return false;
    }

    // listen for incoming connections
    if (listen(mSocketServer, SOMAXCONN) < 0)
    {
        logger.log("Listen failed");
        return false;
    }

    // create an epoll descriptor
    mEpollFd = epoll_create1(0);
    if (mEpollFd < 0)
    {
        logger.log("Fail to create epoll");
        return false;
    }

    // add the server socket to the epoll events
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = mSocketServer;
    epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mSocketServer, &event);

    mRunable = true;
    return true;
}

void Server::Run() {
    // create a thread pool and start accepting connections
    try {
    for (int i = 0; i < kThreadPoolSize; ++i) {
        thread_pool.emplace_back([&]() {
            epoll_event events[kMaxEvents];
            while (true) {
                int num_events = epoll_wait(mEpollFd, events, kMaxEvents, -1);
                for (int j = 0; j < num_events; ++j) {
                    // check if there is any new incoming connection
                    if (events[j].data.fd == mSocketServer) {
                        sockaddr_in client_addr{};
                        socklen_t client_len = sizeof(client_addr);
                        int client_socket = accept(mSocketServer, (struct sockaddr*)&client_addr, &client_len);

                        // add the new client socket to epoll
                        event.events = EPOLLIN | EPOLLET;
                        event.data.fd = client_socket;
                        epoll_ctl(mEpollFd, EPOLL_CTL_ADD, client_socket, &event);
                    } else {
                        // handle requests
                        int client_socket = events[j].data.fd;
                        HandleRequest(client_socket);
                        // remove the client from epoll after processed
                        epoll_ctl(mEpollFd, EPOLL_CTL_DEL, client_socket, nullptr);
                    }
                }
            }
        });
    }
    }
    catch (const std::runtime_error &e)
    {
        logger.log("Run error");
        logger.log(e.what());
    }

    // Join the thread pool when the server is stopped
    for (std::thread& t : thread_pool) {
        t.join();
    }
}

void Server::Stop() {
    mRunable = false;

    // Close server socket and clean up resources
    if (mSocketServer != -1) {
        close(mSocketServer);
    }
    if (mEpollFd != -1) {
        close(mEpollFd);
    }
    // Interrupt the threads in the thread pool to stop them
    for (std::thread& t : thread_pool) {
        t.detach(); // This may not be a clean way to stop threads; you may need a better mechanism.
    }
}

void Server::HandleRequest(int client_socket) {
    // char buffer[1024];
    // size_t bytes_received;
    // bytes_received = read(client_socket, buffer, 1024);
    // logger.log(buffer);

    // Send a response back to the client
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 1\r\nContent-Type: text/plain\r\nAccess-Control-Allow-Origin: *\r\n\r\nX";
    send(client_socket, response.c_str(), response.length(), 0);

    close(client_socket);
}
