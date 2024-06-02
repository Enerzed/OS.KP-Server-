#include "ServerNetwork.hpp"

ServerNetwork::ServerNetwork(unsigned short port) : port(port)
{
    // Сообщение о старте работы сервера
    systemMessages.push_back("Server has started\n");
    std::cout << systemMessages.back() << std::endl;
    systemMessages.pop_back();
    // Генерируем ключи RSA
    rsa.GenerateKeys();

    if (listener.listen(port) != sf::Socket::Done)
    {
        // Сообщение о невозможности прослушивания порта
        systemMessages.push_back("Could not listen port: ");
        systemMessages.back().append(std::to_string(port)).append("\n");
        std::cout << systemMessages.back() << std::endl;
    }
    else
    {
        // Сообщение о работе на порте
        systemMessages.push_back("Listening port: ");
        systemMessages.back().append(std::to_string(port)).append("\n");
        std::cout << systemMessages.back() << std::endl;
        systemMessages.pop_back();
        // Поток для подключения клиентов
        connectionThread = new std::thread(&ServerNetwork::ConnectClients, this, &clients, &clientNames);
    }
}

void ServerNetwork::ConnectClients(std::vector<sf::TcpSocket*>* clients, std::vector<std::string>* clientNames)
{
    while (true)
    {
        sf::TcpSocket* newClient = new sf::TcpSocket();
        if (listener.accept(*newClient) == sf::Socket::Done)
        {
            systemMessages.push_back("Added client ");
            systemMessages.back().append(newClient->getRemoteAddress().toString()).append(":").append(std::to_string(newClient->getRemotePort())).append(" on slot ").append(std::to_string(clients->size())).append("\n");
            std::cout << systemMessages.back();

            newClient->setBlocking(false);
            clients->push_back(newClient);
            clientNames->push_back("UNDEFINED");
            aes.push_back(new AESEncryption);
        }
        else
        {
            systemMessages.push_back("Server listener error, please restart the server");
            std::cout << systemMessages.back() << std::endl;
            delete (newClient);
            break;
        }
    }
}

void ServerNetwork::DisconnectClient(sf::TcpSocket* socketPointer, size_t position)
{
    systemMessages.push_back("Client ");
    systemMessages.back().append(socketPointer->getRemoteAddress().toString()).append(":").append(std::to_string(socketPointer->getRemotePort())).append(" ").append(clientNames[position]).append(" disconnected\n");
    std::cout << systemMessages.back() << std::endl;
    
    socketPointer->disconnect();
    delete (socketPointer);

    clients.erase(clients.begin() + position);
    clientNames.erase(clientNames.begin() + position);
    aes.erase(aes.begin() + position);
}

void ServerNetwork::SendPacket(sf::Packet packet, sf::IpAddress address, unsigned short port)
{
    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        sf::TcpSocket* client = clients[iterator];
        if (client->getRemoteAddress() == address && client->getRemotePort() == port)
        {
            if (client->send(packet) != sf::Socket::Done)
            {
                systemMessages.push_back("Can't send packet\n");
                std::cout << systemMessages.back() << std::endl;
            }
            break;
        }
    }
}

void ServerNetwork::BroadcastPacket(sf::Packet& packet, sf::IpAddress address, unsigned short port)
{
    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        sf::TcpSocket* client = clients[iterator];
        if (client->getRemoteAddress() != address || client->getRemotePort() != port)
        {
            if (client->send(packet) != sf::Socket::Done)
            {
                systemMessages.push_back("Can't send broadcast\n");
                std::cout << systemMessages.back() << std::endl;
            }
        }
    }
}

void ServerNetwork::BroadcastPacket(sf::Packet& packet)
{
    std::string name;
    std::string message;
    sf::Packet restore = packet;
    unsigned short type;

    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        packet >> type;

        if (type == PACKET_TYPE_MESSAGE)
        {
            packet >> name >> message;
            packet.clear();
            packet << type << aes[iterator]->Encrypt(name) << aes[iterator]->Encrypt(message);
        }
        else if (type == PACKET_TYPE_CLIENT_NAME)
        {
            packet >> name;
            packet.clear();
            packet << type << aes[iterator]->Encrypt(name);
        }
        else if (type == PACKET_TYPE_CLIENT_DISCONNECTED)
        {
            packet >> name;
            packet.clear();
            packet << type << aes[iterator]->Encrypt(name);
        }
        else
        {
            packet = restore;
        }
        if (clients[iterator]->send(packet) != sf::Socket::Done)
        {
            systemMessages.push_back("Can't send broadcast\n");
            std::cout << systemMessages.back() << std::endl;
        }
        packet = restore;
    }
}

void ServerNetwork::ReceivePacket(sf::TcpSocket* client, size_t iterator)
{
    sf::Packet packet;
    if (client->receive(packet) == sf::Socket::Disconnected)
    {
        DisconnectClient(client, iterator);
        packet.clear();
        packet << (unsigned short)PACKET_TYPE_CLIENT_DISCONNECTED << clientNames[iterator];
        BroadcastPacket(packet);
        return;
    }

    if (packet.getDataSize() > 0)
    {
        unsigned short type;
        std::string message;
        packet >> type >> message;
        packet.clear();

        switch (type)
        {
        case PACKET_TYPE_MESSAGE:
        {
            // Рассылаем пакет
            packet << type << clientNames[iterator] << aes[iterator]->Decrypt(message, aes[iterator]->GetIV());
            BroadcastPacket(packet);
            // Пакет с расшифрованным сообщением используется для вывода на экран
            packet.clear();
            packet << type << clientNames[iterator] << aes[iterator]->Decrypt(message, aes[iterator]->GetIV());
            packets.push_back(packet);
            // Выводим сообщение в консоль
            systemMessages.push_back(clientNames[iterator]);
            systemMessages.back().append(": ").append(aes[iterator]->Decrypt(message, aes[iterator]->GetIV()));
            std::cout << systemMessages.back() << std::endl;
            systemMessages.pop_back();
            break;
        }
        case PACKET_TYPE_CLIENT_CONNECTED:
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{ 64 });
            // Рассылаем пакет о подключении клиента
            //packet << (unsigned short)PACKET_TYPE_CLIENT_CONNECTED << client->getRemoteAddress().toString() << client->getRemotePort();
            //BroadcastPacket(packet);
            // Отсылаем этому клиенту пакет с RSA ключом
            packet.clear();
            packet << (unsigned short)PACKET_TYPE_RSA_KEY << rsa.GetPublicKey();
            // Пауза на 32 миллисекунды
            std::this_thread::sleep_for(std::chrono::milliseconds{ 32 });
            SendPacket(packet, client->getRemoteAddress(), client->getRemotePort());
            break;
        }
        case PACKET_TYPE_CLIENT_NAME:
        {
            // В таком типе пакета в сообщении находится имя клиента, которое он себе выбрал
            clientNames[iterator] = aes[iterator]->Decrypt(message, aes[iterator]->GetIV());
            // Выводим имя в консоль
            systemMessages.push_back("Client name is ");
            systemMessages.back().append(aes[iterator]->Decrypt(message, aes[iterator]->GetIV())).append("\n");
            std::cout << systemMessages.back() << std::endl;
            packet.clear();
            packet << (unsigned short)PACKET_TYPE_CLIENT_NAME << clientNames[iterator];
            BroadcastPacket(packet);
            break;
        }
        case PACKET_TYPE_AES_KEY:
        {
            // Получаем AES ключ зашифрованный отданным клиенту RSA ключом
            aes[iterator]->SetKey(rsa.Decrypt(message));
            systemMessages.push_back("Got AES key from ");
            systemMessages.back().append(client->getRemoteAddress().toString()).append(":").append(std::to_string(client->getRemotePort())).append("\n");
            std::cout << systemMessages.back() << std::endl;
            break;
        }
        case PACKET_TYPE_AES_IV:
        {
            // Получаем вектор инициализации зашифрованный отданным клиенту RSA ключом
            aes[iterator]->SetIV(rsa.Decrypt(message));
            systemMessages.push_back("Got IV from ");
            systemMessages.back().append(client->getRemoteAddress().toString()).append(":").append(std::to_string(client->getRemotePort())).append("\n");
            std::cout << systemMessages.back() << std::endl;
            break;
        }
        }
    }
}

void ServerNetwork::ManagePackets()
{
    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        ReceivePacket(clients[iterator], iterator);
    }
}

void ServerNetwork::Run()
{
    ManagePackets();
}

void ServerNetwork::ClearPackets()
{
    packets.clear();
}

void ServerNetwork::ClearSystemMessages()
{
    systemMessages.clear();
}

std::vector<sf::Packet> ServerNetwork::GetPackets()
{
    return packets;
}

std::vector<std::string> ServerNetwork::GetSystemMessages()
{
    return systemMessages;
}

unsigned short ServerNetwork::GetPort()
{
    return port;
}