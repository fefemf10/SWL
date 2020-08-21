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
		UDPClient client;
		client.connect(IPEndpoint::getLocalAddress(), 50000);
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
			int i = 0;
			while (client.isConnected())
			{
				if (i == 10) client.disconnect();
				client.send(packet, id);
				i++;
				Sleep(1000);
			}
			system("pause");
			client.disconnect();
			Network::shutdown();
	}
	return 0;
}