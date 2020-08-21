#include "Client.hpp"

namespace swl
{
	Client::Client()
	{

	}
	Packet Client::getLastPacket(uint32_t& id)
	{
		Packet packet;
		id = 0;
		if (!this->packets.empty())
		{
			packet = this->packets.front().first;
			id = this->packets.front().second;
			this->packets.pop();
		}
		return packet;
	}
	bool Client::isConnected() const
	{
		return connection;
	}
	TCPClient::TCPClient()
	{

	}
	Socket::Status TCPClient::connect(const IPEndpoint& ip, const uint16_t& port)
	{
		if (connection) return Socket::Done;
		if (socket.connect(ip, port))
			return Socket::Error;
		connection = true;
		std::thread([&]() {
			Packet packet;
			uint32_t id{};
			Socket::Status status;
			while (connection)
			{
				status = socket.receiveAll((void*)&id, 4);
				status = socket.receive(packet);
				if (status == Socket::Disconnected)
				{
					connection = false;
					break;
				}
				packets.push(std::make_pair(packet, id));
			}
		}).detach();
		return Socket::Done;
	}
	void TCPClient::disconnect()
	{
		connection = false;
		socket.close();
	}
	void TCPClient::send(Packet& packet, uint32_t id)
	{
		socket.sendAll((const void*)&id, 4);
		socket.send(packet);
	}
	TCPSocket& TCPClient::getSocket()
	{
		return socket;
	}

	UDPClient::UDPClient()
	{

	}
	Socket::Status UDPClient::connect(const IPEndpoint& ip, const uint16_t& port)
	{
		if (connection) return Socket::Done;
		socket.bind(IPEndpoint::getLocalAddress(), port+1);
		connection = true;
		this->ip = ip;
		this->port = port;
		std::thread([&]() {
			Packet packet;
			uint32_t id{};
			IPEndpoint ip;
			uint16_t port;
			Socket::Status status;
			while (connection)
			{
				status = socket.receiveAll((void*)&id, 4, ip, port);
				status = socket.receive(packet, ip, port);
				if (status == Socket::Disconnected)
				{
					connection = false;
					break;
				}
				packets.push(std::make_pair(packet, id));
			}
		}).detach();
		return Socket::Done;
	}
	void UDPClient::disconnect()
	{
		connection = false;
		socket.close();
	}
	void UDPClient::send(Packet& packet, uint32_t id)
	{
		socket.sendAll((const void*)&id, 4, ip, port);
		socket.send(packet, ip, port);
	}
	UDPSocket& UDPClient::getSocket()
	{
		return socket;
	}
}