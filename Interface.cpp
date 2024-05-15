#include "Interface.hpp"

void Interface::Init(sf::RenderWindow& window)
{
	// �������������� ���������
	ImGui::SFML::Init(window, false);
	// �������� ��������� ���������
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("arial.ttf", 16.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
}

void Interface::Update(sf::RenderWindow& window, sf::Time time)
{
	// ���������
	ImGui::SFML::Update(window, time);
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(1280.f, 720.f));
	ImGui::Begin(u8"����� - ������� ������ ����������, ������ ��. �.�. ���������", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// ������ �������� �������
	if (ImGui::Button(u8"������� ����� ������") == true)
	{
		isCreateServerNetwork = true;
	}

	// ����� �����
	for (size_t iterator = 0; iterator < textBoxes.size(); iterator++)
	{
		ImGui::Text(u8"��� �� �����:");
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

			// ���� ������������� ����� textBox, �� ������� ������ 2 * inputSize �������� (� ������ ���������� ����� 1000 ��������� �������� 256 ��������)
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
	// ������� ������ ����� [] �����, ����� ������ ���������������� ����-������������
	// � ��������� ����� ���������� �� ���������
	// � ���� ������ ���������� ������� ������
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