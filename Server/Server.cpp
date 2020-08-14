#include <iostream>
#include <Network/Network.hpp>
#include <Network/UDPSocket.hpp>
#include <Network/IPEndpoint.hpp>
#include <Network/Packet.hpp>

using namespace swl;

int main()
{
	setlocale(LC_ALL, "UTF8");
	if (Network::initialize())
	{
		std::cout << "Winsock api successfully initialized.\n";
		UDPSocket socket;
		IPEndpoint ip("127.0.0.1");
		socket.bind(ip, 50000);
		
		uint16_t port;
		std::string buffer = "";
		while (true)
		{
			Packet packet;
			socket.receive(packet, ip, port);
			packet >> buffer;
			std::cout << buffer << std::endl;
		}
		socket.close();
	}
	Network::shutdown();
	system("pause");
	return 0;
}