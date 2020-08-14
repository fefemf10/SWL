#pragma once
#include <iostream>
#include <fstream>
#include <Network/Network.hpp>
#include <Network/TCPSocket.hpp>
#include <Network/UDPSocket.hpp>
#include <Network/IPEndpoint.hpp>
#include <Network/Packet.hpp>

using namespace swl;

int main()
{
	setlocale(LC_ALL, "UTF8");
	if (Network::initialize())
	{
		/*std::cout << "Winsock api successfully initialized.\n";
		UDPSocket socket;
		IPEndpoint ip("127.0.0.1");
		if (socket.bind(ip, 50001))
			return 0;
		std::string buffer = "Heellow asd";
		while (true)
		{
			Packet packet;
			packet << buffer;
			if (socket.send(packet, ip, 50000) != Socket::Done)
				break;
			Sleep(500);
		}
		socket.close();*/
		TCPSocket socket;
		socket.setBlocking(true);
		IPEndpoint ip("127.0.0.1");
		socket.bind(ip, 50001);
		if (socket.connect(ip, 50000) == Socket::NotReady)
			std::cout << "NotReady";
		
		std::string buffer = "Heellow asd";
		Packet packet;
		packet << buffer;
		socket.send(packet);
		socket.close();
	}
	Network::shutdown();
	system("pause");
	return 0;
}