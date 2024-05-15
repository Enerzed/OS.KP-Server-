#pragma once

#define BASE_PORT 9010;

#include "ServerNetwork.hpp"
#include "Interface.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>


class Server
{
private:
	std::vector<ServerNetwork*> networks;
	unsigned short basePort = BASE_PORT;
	unsigned short port = BASE_PORT;
public:
	void Run();
};