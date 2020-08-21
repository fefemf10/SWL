#include <iostream>
#include <Network/Network.hpp>
#include <Network/IPEndpoint.hpp>
#include <Network/Server.hpp>

using namespace swl;

int main()
{
	setlocale(LC_ALL, "UTF8");
	if (Network::initialize())
	{
		UDPServer server;
		server.run(IPEndpoint::getLocalAddress(), 50000);
		system("pause");
		server.stop();
		Network::shutdown();
	}
	
	return 0;
}