// Устанавливаем кодировку utf-8


#include "Server.hpp"

Server::Server()
{
    // Инициализируем окно и интерфейс
    window = new sf::RenderWindow(sf::VideoMode(1280, 720), L"Мессенджер Сервер", sf::Style::Close);
    window->setFramerateLimit(120);
    time = new sf::Time();
    interface = new Interface();
    interface->Init(*window);
}

void Server::Run()
{
    // Время
    sf::Clock clock;
    // Основной цикл
    while (window->isOpen())
    {
        *time = clock.getElapsedTime();
        clock.restart();
        // Обработка событий
        sf::Event event;
        while (window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                ServerDownHandler();
                window->close();
            }
        }
        // Сеть
        RunNetwork();
        // Передаем системные сообщения
        RunSystemMessages();
        // Передаем пакеты
        RunPackets();
        // Обновляем интерфейс
        RunInterface();
        // Очищаем окно
        window->clear();
        // Render интерфейса должен быть перед показом его на экране для того, чтобы он был на первом плане
        ImGui::SFML::Render(*window);
        // Показываем окно
        window->display();
    }
    // Отключаем ImGui-SFML после закрытия окна
    ImGui::SFML::Shutdown();
}

void Server::RunNetwork()
{
    for (size_t iterator = 0; iterator < networks.size(); iterator++)
    {
        networks[iterator]->Run();
    }
}

void Server::RunSystemMessages()
{
    for (size_t iterator = 0; iterator < networks.size(); iterator++)
    {
        for (size_t iterator2 = 0; iterator2 < networks[iterator]->GetSystemMessages().size(); iterator2++)
        {
            std::string message = networks[iterator]->GetSystemMessages()[iterator2];
            interface->ModifyTextBoxSystemMessage(message, iterator + basePort);
        }
        networks[iterator]->ClearSystemMessages();
    }
}

void Server::RunPackets()
{
    for (size_t iterator = 0; iterator < networks.size(); iterator++)
    {
        std::vector<sf::Packet> packets = networks[iterator]->GetPackets();
        for (size_t iterator2 = 0; iterator2 < packets.size(); iterator2++)
        {
            unsigned short type;
            std::string name;
            std::string message;
            // Выводим содержимое пакета на экран
            packets[iterator2] >> type >> name >> message;
            interface->ModifyTextBox(message, name, networks[iterator]->GetPort());
        }
        networks[iterator]->ClearPackets();
    }
}

void Server::RunInterface()
{
    interface->Update(*window, *time);

    if (interface->GetIsCreateServerNetwork() == true)
    {
        interface->AddTextBox(port);
        networks.push_back(new ServerNetwork(port++));
        interface->SetIsCreateServerNetwork(false);
    }
}

void Server::ServerDownHandler()
{
    std::cout << "Sending server down packets\n";
    sf::Packet packet;
    packet << (unsigned short)PACKET_TYPE_SERVER_DOWN;
    for (size_t iterator = 0; iterator < networks.size(); iterator++)
    {
        networks[iterator]->BroadcastPacket(packet);
    }
}

void Server::AddServerVector(int signum)
{
    serverVector.push_back(signum);
}