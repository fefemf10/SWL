#include <iostream>
#include <Network/Network.hpp>
#include <Network/TCPSocket.hpp>
#include <Network/UDPSocket.hpp>
#include <Network/IPEndpoint.hpp>
#include <Network/ZipPacket.hpp>

using namespace swl;

int main()
{
	setlocale(LC_ALL, "UTF8");
	if (Network::initialize())
	{
		/*std::cout << "Winsock api successfully initialized.\n";
		UDPSocket socket;
		IPEndpoint ip("127.0.0.1");
		socket.bind(ip, 50000);
		IPEndpoint ipc;
		uint16_t portc;
		std::string buffer = "";
		while (true)
		{
			Packet packet;
			socket.receive(packet, ipc, portc);
			packet >> buffer;
			std::cout << buffer << " " << ipc.toString() << " " << std::to_string(portc) << std::endl;
		}
		socket.close();*/
		TCPSocket socket;
		socket.bind(IPEndpoint::getLocalAddress(), 50000);
		socket.listen();
		TCPSocket client;
		socket.accept(client);
		Packet packet;
		File file;
		client.receive(packet);
		packet >> file;
		file.saveFile();
		socket.close();
	}
	Network::shutdown();
	system("pause");
	return 0;
}