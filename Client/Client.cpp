#pragma once
#include <iostream>
#include <fstream>
#include <Network/Network.hpp>
#include <Network/SocketSelector.hpp>
#include <Network/Client.hpp>

using namespace swl;

int main()
{
	setlocale(LC_ALL, "UTF8");
	if (Network::initialize())
	{
		TCPClient client;
		client.connect(IPEndpoint::getLocalAddress(), 25565);
		std::thread([&]()
			{
				std::string buffer;
				Packet packet;
				uint32_t id{};
				while (client.isConnected())
				{
					packet = client.getLastPacket(id);
					if (packet.getSize() > 0)
					{
						packet >> buffer;
						std::cout << buffer << " id=" << id << std::endl;
					}
					Sleep(16);
				}
			}).detach();
			Packet packet;
			uint32_t id{};
			std::string buffer = "Heelloo";
			packet << buffer;
			while (client.isConnected())
			{
				client.send(packet, id);
				Sleep(1000);
			}
			system("pause");
			client.disconnect();
			Network::shutdown();
	}
	return 0;
}