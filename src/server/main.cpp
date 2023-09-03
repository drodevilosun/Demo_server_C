#include "server.hpp"

#define SERVER_PORT "8989"

int main() {
    Server server;
    server.Init(SERVER_PORT);
    server.Run();
    return 0;
}
