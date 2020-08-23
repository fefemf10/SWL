#include <iostream>
#include <memory>
#include <Network/Network.hpp>
#include <Network/IPEndpoint.hpp>
#include <Network/Server.hpp>

using namespace swl;


int main()
{
	std::shared_ptr<TCPServer> server;
	if (Network::initialize())
	{
		server = std::make_shared<TCPServer>();
		server->run(IPEndpoint::getLocalAddress(), 25565);
		system("pause");
		server->stop();
		Network::shutdown();
	}
	
	return 0;
}