#pragma once

#define BASE_PORT 9010;

#include "ServerNetwork.hpp"
#include "Interface.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <csignal>


class Server
{
private:
	sf::RenderWindow* window;					// Основное окно программы
	std::vector<ServerNetwork*> networks;		// Сервера, к которым подключаются клиенты
	Interface* interface;						// Интерфейс
	unsigned short basePort = BASE_PORT;		// Базовый порт
	unsigned short port = BASE_PORT;			// Текущий порт
	std::vector<int> serverVector;				// Вектор для сигналов
public:
	Server();
	void Run();									// Основной цикл
	void RunNetwork();							// Обработка серверов
	void RunSystemMessages();					// Обработка системных сообщений
	void RunPackets();							// Обработка полученных пакетов
	void ServerDownHandler();					// Обработчик, если сервер был так или иначе выключен
	void AddServerVector(int);					// Добавления сигнала для обработки
};