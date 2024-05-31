#include "ServerNetwork.hpp"

ServerNetwork::ServerNetwork(unsigned short port) : listenPort(port)
{
    systemMessages.push_back("Server has started\n");
    std::cout << systemMessages.back() << std::endl;
    systemMessages.pop_back();

    // Генерируем ключи RSA
    rsa.GenerateKeys();

    if (listener.listen(listenPort) != sf::Socket::Done)
    {
        systemMessages.push_back("Could not listen port: ");
        systemMessages.back().append(std::to_string(listenPort)).append("\n");
        std::cout << systemMessages.back() << std::endl;
    }
    else
    {
        systemMessages.push_back("Listening port: ");
        systemMessages.back().append(std::to_string(listenPort)).append("\n");
        std::cout << systemMessages.back() << std::endl;
        systemMessages.pop_back();

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
    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        sf::TcpSocket* client = clients[iterator];
        if (client->send(packet) != sf::Socket::Done)
        {
            systemMessages.push_back("Can't send broadcast\n");
            std::cout << systemMessages.back() << std::endl;
        }
    }
}

void ServerNetwork::ReceivePacket(sf::TcpSocket* client, size_t iterator)
{
    sf::Packet packet;
    if (client->receive(packet) == sf::Socket::Disconnected)
    {
        packet.clear();
        packet << (unsigned short)PACKET_TYPE_CLIENT_DISCONNECTED << clientNames[iterator] << client->getRemoteAddress().toString() << client->getRemotePort();
        BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());

        DisconnectClient(client, iterator);
        return;
    }

    if (packet.getDataSize() > 0)
    {
        unsigned short type;
        //std::string name;
        std::string message;
        packet >> type >> message;
        packet.clear();

        switch (type)
        {
        case PACKET_TYPE_MESSAGE:
        {
            packet << type << clientNames[iterator] << message << client->getRemoteAddress().toString() << client->getRemotePort();

            packets.push_back(packet);
            BroadcastPacket(packet);

            systemMessages.push_back("From ");
            systemMessages.back().append(clientNames[iterator]).append(" with address ").append(client->getRemoteAddress().toString()).append(":").append(std::to_string(client->getRemotePort())).append(" - ").append(aes[iterator]->Decrypt(message, aes[iterator]->GetIV())).append("\n");
            std::cout << systemMessages.back() << std::endl;
            systemMessages.pop_back();

            break;
        }
        case PACKET_TYPE_INITIAL_DATA:
        {
            clientNames[iterator] = message;

            systemMessages.push_back("Client name of ");
            systemMessages.back().append(client->getRemoteAddress().toString()).append(":").append(std::to_string(client->getRemotePort())).append(" - ").append(message).append("\n");
            std::cout << systemMessages.back() << std::endl;

            packet << (unsigned short)PACKET_TYPE_CLIENT_CONNECTED << clientNames[iterator] << client->getRemoteAddress().toString() << client->getRemotePort();
            BroadcastPacket(packet);
            
            packet.clear();
            packet << (unsigned short)PACKET_TYPE_RSA_KEY << rsa.GetPublicKey();
            SendPacket(packet, client->getRemoteAddress(), client->getRemotePort());

            break;
        }
        case PACKET_TYPE_AES_KEY:
        {
            aes[iterator]->SetKey(rsa.Decrypt(message));

            //std::cout << "KEY : " << aes[iterator]->GetKey();

            //systemMessages.push_back("Got AES key\n");
            //std::cout << systemMessages.back() << std::endl;

            break;
        }
        case PACKET_TYPE_AES_IV:
        {
            aes[iterator]->SetIV(rsa.Decrypt(message));

            //std::cout << "IV : " << aes[iterator]->GetIV();

            //systemMessages.push_back("Got IV\n");
            //std::cout << systemMessages.back() << std::endl;

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