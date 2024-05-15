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
		ImGui::Text(u8"Чат на порте:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(ports.at(iterator)).c_str());
		for (size_t iterator2 = 0; iterator2 < textBoxes.size(); iterator2++)
		{
			ImGui::TextWrapped(textBoxes[iterator][iterator2]);
			//ImGui::InputTextMultiline(std::to_string(ports.at(iterator)).c_str(), textBoxes.at(iterator), IM_ARRAYSIZE(textBoxes.at(iterator)),
			//	ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * textBoxHeight), ImGuiInputTextFlags_ReadOnly);
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
			std::strcat(textBoxes[iterator], receivedName.c_str());
			std::strcat(textBoxes[iterator], " : ");
			std::strcat(textBoxes[iterator], receivedString.c_str());
			std::strcat(textBoxes[iterator], "\n");

			// Если заканчивается буфер textBox, то удаляем старые 2 * inputSize символов (В буфере помещается около 1000 сообщений размером 256 символов)
			if (strlen(textBoxes[iterator]) + 2 * inputSize >= textBoxSize)
			{
				char* newTextBox = textBoxes[iterator] + 512;
				// memmove O_o
				memmove(textBoxes[iterator], newTextBox, strlen(newTextBox) + 1);
			}
		}
	}
}

void Interface::ModifyTextBoxSystemMessage(std::string recievedString, unsigned short port)
{
	for (size_t iterator = 0; iterator < ports.size(); iterator++)
	{
		if (port == ports[iterator])
		{
			std::strcat(textBoxes[iterator], "\n");
			std::strcat(textBoxes[iterator], "\t");
			std::strcat(textBoxes[iterator], recievedString.c_str());
			std::strcat(textBoxes[iterator], "\n");
		}
	}
}

void Interface::ModifyTextBoxSize()
{

}

void Interface::AddTextBox(unsigned short newTag)
{
	// Круглые скобки после [] важны, тогда массив инициализируется нуль-терминатором
	// А создавать здесь переменную не получится
	// У меня всегда получалась висячая ссылка
	textBoxes.push_back(new char[textBoxSize]());
	ports.push_back(newTag);
}

void Interface::SetIsCreateServerNetwork(bool newIsCreateServerNetwork)
{
	isCreateServerNetwork = newIsCreateServerNetwork;
}

bool Interface::GetIsCreateServerNetwork()
{
	return isCreateServerNetwork;
}