// Везде бы использовал std::string, но ImGui использует const char :P

#pragma once
#define textBoxSize 65536
#define textBoxHeight 16
#define inputSize 128

#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>
#include <vector>


class Interface
{
private:
	std::vector<std::vector<char*>> textBoxes;
	std::vector<unsigned short> ports;
	bool isCreateServerNetwork = false;
public:
	void Init(sf::RenderWindow& window);					// Инициализация интерфейса и поддержки кириллицы
	void Update(sf::RenderWindow& window, sf::Time time);	// Функция обновления для управления
	void AddTextBox(unsigned short);
	void ModifyTextBox(std::string, std::string, unsigned short);
	void ModifyTextBoxSystemMessage(std::string, unsigned short);
	void ModifyTextBoxSize();
	// Getters
	bool GetIsCreateServerNetwork();
	// Setters
	void SetIsCreateServerNetwork(bool);
};