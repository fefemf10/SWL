#pragma once
#include <iostream>
#include <fstream>
#include <Network/Network.hpp>
#include <Network/UDPSocket.hpp>
#include <Network/IPEndpoint.hpp>
#include <Network/Packet.hpp>
#include <Network/File.hpp>

using namespace swl;

int main()
{
	setlocale(LC_ALL, "UTF8");
	std::string test{};
	std::string filepath{"Network.pdb"};
	Packet packet;
	File file(filepath);
	File fileBack;
	file.readFile();
	packet << file;
	packet >> fileBack;
	fileBack.saveFile();
	system("pause");
	return 0;
}