// Устанавливаем кодировку utf-8
#pragma execution_character_set("utf-8")
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
    // Кодировка
    system("chcp 65001");
    std::cout << std::endl;
    // Сервер
    Server server;
    ServerPtr = &server;
    // Сигналы
    signal(SIGABRT, SignalHandler);
    signal(SIGINT, SignalHandler);\
    // SIGBREAK только на Windows
    //signal(SIGBREAK, SignalHandler);
    signal(SIGTERM, SignalHandler);
    server.Run();
    return 0;
}