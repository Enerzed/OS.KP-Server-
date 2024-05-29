#include "Server.hpp"


Server* ServerPtr = nullptr;
void(SignalHandler(int));


void SignalHandler(int signal)
{
    ServerPtr->AddServerVector(signal);
    ServerPtr->ServerDownHandler();
}

int main()
{
    system("chcp 65001");
    std::cout << std::endl;

    Server server;
    ServerPtr = &server;

    signal(SIGABRT, SignalHandler);
    signal(SIGINT, SignalHandler);
    signal(SIGBREAK, SignalHandler);
    signal(SIGTERM, SignalHandler);

    server.Run();

    return 0;
}