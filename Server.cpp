// Server.cpp: определяет точку входа для приложения.

#pragma execution_character_set("utf-8")

#include "Server.h"

void Server::Run()
{
    ServerNetwork serverNetwork();
    // Окно
    sf::RenderWindow window(sf::VideoMode(1280, 720), L"Мессенджер Сервер", sf::Style::Close);
    window.setFramerateLimit(120);

    Interface interface;
    interface.Init(window);
    // Время
    sf::Clock clock;
    // Основной цикл
    while (window.isOpen())
    {
        sf::Time time = clock.getElapsedTime();
        clock.restart();
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                
                window.close();
            }
        }
        // Сеть
        for (size_t iterator = 0; iterator < networks.size(); iterator++)
        {
            networks[iterator]->Run();
        }
        for (size_t iterator = 0; iterator < networks.size(); iterator++)
        {
            if (networks[iterator]->GetIsPacketsReceived() == true)
            {
                std::vector<sf::Packet> packets = networks[iterator]->GetPackets();
                for (size_t iterator2 = 0; iterator2 < packets.size(); iterator2++)
                {
                    std::string receivedString;
                    std::string receivedName;
                    std::string senderAddress;
                    unsigned short senderPort;
                    packets[iterator2] >> receivedString >> receivedName >> senderAddress >> senderPort;
                    interface.ModifyTextBox(receivedString, receivedName, iterator + basePort);
                }
                networks[iterator]->ClearPackets();
                networks[iterator]->SetIsPacketsReceived(false);
            }
        }
        // Обновляем интерфейс
        interface.Update(window, time);

        if (interface.GetIsCreateServerNetwork() == true)
        {
            interface.AddTextBox(port);
            networks.push_back(new ServerNetwork(port++));
            interface.SetIsCreateServerNetwork(false);
        }
        // Очищаем окно
        window.clear();
        // Render интерфейса должен быть перед показом его на экране для того, чтобы он был на первом плане
        ImGui::SFML::Render(window);
        // Показываем окно
        window.display();
    }
    // Отключаем ImGui-SFML после закрытия окна
    ImGui::SFML::Shutdown();
}


int main()
{
    system("chcp 65001");

    Server server;
    server.Run();

    return 0;
}