// Везде бы использовал std::string, но ImGui использует const char :P

#pragma once
#define TEXT_BOX_SIZE 256
#define TEXT_BOX_HEIGHT 256
#define TEXT_BOX_MESSAGE_LIMIT 128

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>
#include <cstring>
#include <vector>


class Interface
{
private:
	std::vector<std::vector<char*>> textBoxes;						// Поля чатов
	std::vector<unsigned short> ports;								// Порты чатов
	bool isCreateServerNetwork = false;								// Флаг была ли нажата кнопка создания чата
public:
	void Init(sf::RenderWindow& window);							// Инициализация интерфейса и поддержки кириллицы
	void Update(sf::RenderWindow& window, sf::Time time);			// Функция обновления для управления
	void AddTextBox(unsigned short);								// Добавляние очередного текствого окна с выводом сообщений из разных портов
	void ModifyTextBox(std::string, std::string, unsigned short);	// Получаем сообщения и загружаем их textBoxes
	void ModifyTextBoxSystemMessage(std::string, unsigned short);	// Уведомления системы
	void ModifyTextBoxSize();										// Чтобы сообщения постоянно в программе не копились удаляем самые старые (TEXT_BOX_MESSAGE_LIMIT)
	// Getters
	bool GetIsCreateServerNetwork();								// Для получения значения была ли нажата кнопка создания чата
	// Setters
	void SetIsCreateServerNetwork(bool);							// После того как чат создали устанавливаем кнопку в false снова
};