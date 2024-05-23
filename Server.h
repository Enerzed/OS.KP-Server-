#pragma once

#define BASE_PORT 9010;

#include "ServerNetwork.hpp"
#include "Interface.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <signal.h>


class Server
{
private:
	std::vector<int> classVector;
	sf::RenderWindow* window;
	std::vector<ServerNetwork*> networks;
	Interface* interface;
	unsigned short basePort = BASE_PORT;
	unsigned short port = BASE_PORT;
public:
	Server()
	{
		window = new sf::RenderWindow(sf::VideoMode(1280, 720), L"Мессенджер Сервер", sf::Style::Close);
		window->setFramerateLimit(120);

		interface = new Interface();
		interface->Init(*window);
	}
	void Run();
	void RunNetwork();
	void RunSystemMessages();
	void RunPackets();
	void Callback();
	void AddClassVector(int);
};

