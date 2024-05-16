#include "Interface.hpp"

void Interface::Init(sf::RenderWindow& window)
{
	// Инициализируем интерфейс
	ImGui::SFML::Init(window, false);
	// Включаем поддержку кириллицы
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("arial.ttf", 16.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
}

void Interface::Update(sf::RenderWindow& window, sf::Time time)
{
	// Интерфейс
	ImGui::SFML::Update(window, time);
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(1280.f, 720.f));
	ImGui::Begin(u8"Автор - Ошурков Никита Алексеевич, АлтГТУ им. И.И. Ползунова", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Кнопка создания сервера
	if (ImGui::Button(u8"Создать новый сервер") == true)
	{
		isCreateServerNetwork = true;
	}

	// Вывод строк
	for (size_t iterator = 0; iterator < textBoxes.size(); iterator++)
	{
		if (ImGui::CollapsingHeader(std::to_string(ports.at(iterator)).c_str()))
		{
			ImGui::BeginChild(std::to_string(ports.at(iterator)).c_str(), ImVec2(0, TEXT_BOX_HEIGHT), true);
			for (size_t iterator2 = 0; iterator2 < textBoxes[iterator].size(); iterator2++)
			{
				ImGui::TextWrapped(textBoxes[iterator][iterator2]);
			}
			ImGui::EndChild();
		}
	}
	ImGui::StyleColorsClassic();
	ImGui::End();
}

void Interface::ModifyTextBox(std::string receivedString, std::string receivedName, unsigned short port)
{
	for (size_t iterator = 0; iterator < ports.size(); iterator++)
	{
		if (port == ports[iterator])
		{
			textBoxes[iterator].push_back(new char[TEXT_BOX_SIZE]());
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], receivedName.c_str());
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], " : ");
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], receivedString.c_str());
		}
	}
}

void Interface::ModifyTextBoxSystemMessage(std::string receivedString, unsigned short port)
{
	for (size_t iterator = 0; iterator < ports.size(); iterator++)
	{
		if (port == ports[iterator])
		{
			textBoxes[iterator].push_back(new char[TEXT_BOX_SIZE]());
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], "\n");
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], "\t");
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], receivedString.c_str());
			std::strcat(textBoxes[iterator][textBoxes[iterator].size() - 1], "\n");
		}
	}
}

void Interface::ModifyTextBoxSize()
{
	for (size_t iterator = 0; iterator < textBoxes.size(); iterator++)
	{
		while (textBoxes.size() > TEXT_BOX_MESSAGE_LIMIT)
		{
			textBoxes[iterator].erase(textBoxes[iterator].begin());
		}
	}
}

void Interface::AddTextBox(unsigned short newPort)
{
	textBoxes.push_back(std::vector<char*>());
	ports.push_back(newPort);
}

void Interface::SetIsCreateServerNetwork(bool newIsCreateServerNetwork)
{
	isCreateServerNetwork = newIsCreateServerNetwork;
}

bool Interface::GetIsCreateServerNetwork()
{
	return isCreateServerNetwork;
}