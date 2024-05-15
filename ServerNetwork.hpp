// Класс для соединения и обработки сообщений


#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>

#include <SFML/Network.hpp>

// Переопределяем размер допустимого размера содержимого sf::Packet
#define MAX_RAW_DATA 256


class ServerNetwork
{
private:
    sf::TcpListener listener;               // TcpListerner - тот же сокет, только он имеет всего одну роль: для прослушивания входящих соединиений
    std::vector<sf::TcpSocket*> clients;    // Массив сокетов для каждого клиента
    std::vector<sf::Packet> packets;        // Полученные пакеты
    std::thread* connectionThread;          // Отдельный поток для подключения клиентов
    std::string systemMessage;              // Различные сообщения
    bool isSystemMessage = false;           // Если хотим отправить сообщение в основную экранную форму, то меняем флаг на true
    bool isPacketsReceived = false;         // Если были получены пакеты ставим true
    unsigned short listenPort;              // Порт для прослушивания

public:
    ServerNetwork(unsigned short);
    void ConnectClients(std::vector<sf::TcpSocket*>*);  // Подключаем клиентов к серверу
    void DisconnectClient(sf::TcpSocket*, size_t);      // Отключаем клиентов от сервера
    void BroadcastPacket(sf::Packet&);                  // Рассылаем пакеты клиентам сервера
    void ReceivePacket(sf::TcpSocket*, size_t);         // Получаем пакет и его содержимое
    void ManagePackets();                               // Получаем пакеты от каждого подключенного клиента (затем передаем их методу ReceivePacket)
    void Run();                                         // Запуск процесса
    void ClearPackets();
    // Setters
    void SetIsPacketsReceived(bool);
    // Getters
    std::vector<sf::Packet> GetPackets();
    bool GetIsPacketsReceived();
};