#include "ServerNetwork.hpp"

ServerNetwork::ServerNetwork(unsigned short port) : listenPort(port)
{
    systemMessage = "Server has started\n";
    std::cout << systemMessage << std::endl;

    if (listener.listen(listenPort) != sf::Socket::Done)
    {
        systemMessage = "Could not listen port: ";
        systemMessage.append(std::to_string(listenPort));
        isSystemMessage = true;

        std::cout << systemMessage << std::endl;
    }
    else
    {
        systemMessage = "Listening port: ";
        systemMessage.append(std::to_string(listenPort));
        isSystemMessage = true;

        std::cout << systemMessage << std::endl;

        connectionThread = new std::thread(&ServerNetwork::ConnectClients, this, &clients);
    }
}

void ServerNetwork::ConnectClients(std::vector<sf::TcpSocket*>* clientArray)
{
    while (true)
    {
        sf::TcpSocket* newClient = new sf::TcpSocket();
        if (listener.accept(*newClient) == sf::Socket::Done)
        {
            newClient->setBlocking(false);
            clientArray->push_back(newClient);
            std::cout << "Added client " << newClient->getRemoteAddress() << ":" << newClient->getRemotePort() << " on slot " << clientArray->size() << std::endl;
        }
        else
        {
            std::cout << "Server listener error, please restart the server" << std::endl;
            delete (newClient);
            break;
        }
    }
}

void ServerNetwork::DisconnectClient(sf::TcpSocket* socketPointer, size_t position)
{
    std::cout << "Client " << socketPointer->getRemoteAddress() << ":" << socketPointer->getRemotePort() << " disconnected" << std::endl;
    socketPointer->disconnect();
    delete (socketPointer);
    clients.erase(clients.begin() + position);
}

void ServerNetwork::BroadcastPacket(sf::Packet& replyPacket)
{
    for (size_t iterator = 0; iterator < clients.size(); iterator++)
    {
        sf::TcpSocket* client = clients[iterator];
        if (client->send(replyPacket) != sf::Socket::Done)
        {
            std::cout << "Can't send broadcast" << std::endl;
        }
        else
        {
            isPacketsReceived = true;
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
        std::string receivedMessage;
        std::string name;
        packet >> receivedMessage >> name;
        packet.clear();

        packet << receivedMessage << name << client->getRemoteAddress().toString() << client->getRemotePort();
        packets.push_back(packet);
        BroadcastPacket(packet);
        std::cout << "From client " << name << " with address " << client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " - " << receivedMessage << std::endl;
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

std::vector<sf::Packet> ServerNetwork::GetPackets()
{
    return packets;
}

void ServerNetwork::SetIsPacketsReceived(bool newIsPacketsReceived)
{
    isPacketsReceived = newIsPacketsReceived;
}

bool ServerNetwork::GetIsPacketsReceived()
{
    return isPacketsReceived;
}