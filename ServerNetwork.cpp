#include "ServerNetwork.hpp"

ServerNetwork::ServerNetwork(unsigned short port) : listenPort(port)
{
    systemMessages.push_back("Server has started\n");
    std::cout << systemMessages.back() << std::endl;

    if (listener.listen(listenPort) != sf::Socket::Done)
    {
        systemMessages.push_back("Could not listen port: ");
        systemMessages.back().append(std::to_string(listenPort));
        std::cout << systemMessages.back() << std::endl;
    }
    else
    {
        systemMessages.push_back("Listening port: ");
        systemMessages.back().append(std::to_string(listenPort));
        std::cout << systemMessages.back() << std::endl;

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
    systemMessages.push_back("Client");
    systemMessages.back().append(socketPointer->getRemoteAddress().toString()).append(":").append(std::to_string(socketPointer->getRemotePort())).append(" disconnected\n");
    std::cout << systemMessages.back() << std::endl;
    
    socketPointer->disconnect();
    delete (socketPointer);

    clients.erase(clients.begin() + position);
    clientNames.erase(clientNames.begin() + position);
}

void ServerNetwork::BroadcastPacket(sf::Packet& replyPacket)
{
    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        sf::TcpSocket* client = clients[iterator];
        if (client->send(replyPacket) != sf::Socket::Done)
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
        DisconnectClient(client, iterator);
        return;
    }

    if (packet.getDataSize() > 0)
    {
        size_t type;
        std::string name;
        std::string message;
        packet >> type >> name >> message;
        packet.clear();

        switch (type)
        {
        case PACKET_TYPE_MESSAGE:
        {
            packet << type << clientNames[iterator] << message << client->getRemoteAddress().toString() << client->getRemotePort();
            packets.push_back(packet);
            BroadcastPacket(packet);
            std::cout << "From client " << clientNames[iterator] << " with address " << client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " - " << message << std::endl;
            break;
        }
        case PACKET_TYPE_NAME:
        {
            clientNames[iterator] = message;

            packet << type << clientNames[iterator] << message << client->getRemoteAddress().toString() << client->getRemotePort();



            std::cout << "Client name of " << client->getRemoteAddress() << ":" << client->getRemotePort() << " - " << message << std::endl;
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