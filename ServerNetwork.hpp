// Класс для соединения и обработки сообщений
#pragma once

#define PACKET_TYPE_MESSAGE 1
#define PACKET_TYPE_CLIENT_NAME 2
#define PACKET_TYPE_CLIENT_CONNECTED 3
#define PACKET_TYPE_CLIENT_DISCONNECTED 4
#define PACKET_TYPE_SERVER_DOWN 5
#define PACKET_TYPE_RSA_KEY 6
#define PACKET_TYPE_AES_KEY 7
#define PACKET_TYPE_AES_IV 8

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>

#include <SFML/Network.hpp>
#include "Encryption.hpp"

// Переопределяем размер допустимого размера содержимого sf::Packet
#define MAX_RAW_DATA 256


class ServerNetwork
{
private:
    std::vector<sf::TcpSocket*> clients;                                            // Массив сокетов для каждого клиента
    std::vector<std::string> clientNames;                                           // Имена клиентов
    std::vector<sf::Packet> packets;                                                // Полученные пакеты
    std::vector<std::string> systemMessages;                                        // Различные сообщения
    std::thread* connectionThread;                                                  // Отдельный поток для подключения клиентов
    unsigned short port;                                                            // Порт для прослушивания
    RSAEncryption rsa;                                                              // Объект класса RSAEncryption для соответственного шифрования
    std::vector<AESEncryption*> aes;                                                // Объект класса AESEncryption для соответственного шифрования
    sf::TcpListener listener;                                                       /* TcpListerner - тот же сокет, только он имеет всего одну роль
                                                                                    : для прослушивания входящих соединиений*/
public:
    ServerNetwork(unsigned short);
    void ConnectClients(std::vector<sf::TcpSocket*>*, std::vector<std::string>*);   // Подключаем клиентов к серверу
    void DisconnectClient(sf::TcpSocket*, size_t);                                  // Отключаем клиентов от сервера
    void SendPacket(sf::Packet, sf::IpAddress, unsigned short);                     // Отправка пакета конкретному клиенту
    void BroadcastPacket(sf::Packet&);                                              // Рассылаем пакеты клиентам сервера (Отсылать зашифрованные сообщения тут)
    void BroadcastPacket(sf::Packet&, sf::IpAddress, unsigned short);               // Рассылаем пакеты клиентам сервера кроме одного
    void ReceivePacket(sf::TcpSocket*, size_t);                                     // Получаем пакет и его содержимое
    void ManagePackets();                                                           // Получаем пакеты от каждого подключенного клиента (затем передаем их методу ReceivePacket)
    void Run();                                                                     // Запуск процесса
    void ClearPackets();                                                            // Очистка пакетов
    void ClearSystemMessages();                                                     // Очистка системных сообщений
    // Setters
    // Getters
    std::vector<sf::Packet> GetPackets();                                           // Чтобы получить пакеты извне
    std::vector<std::string> GetSystemMessages();                                   // Чтобы получить системные сообщения извне
    unsigned short GetPort();                                                       // Получить текущий порт на котором работает чат
};