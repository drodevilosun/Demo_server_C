#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "Logger.hpp"

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

class Server {
public:
    Server();
    ~Server();
    bool Init(std::string serverPort);
    void Run();
    void Stop();

private:
    void HandleRequest(int client_socket);
    int mSocketServer;
    int mEpollFd;
    static constexpr int kMaxEvents = 10000;
    static constexpr int kThreadPoolSize = 15;
    bool mRunable;
    std::vector<std::thread> thread_pool;
    epoll_event event;
    

    Logger &logger = Logger::getInstance();
};

#endif  // SERVER_HPP
