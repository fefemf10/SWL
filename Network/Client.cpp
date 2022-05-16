#include "Client.hpp"

namespace swl
{
	Client::Client() : packets{}, packetId{}
	{
		connection = false;
	}
	Client::~Client()
	{

	}
	void Client::setSettingsSend(const bool& encrypt, const bool& zip)
	{
		this->encrypt = encrypt;
		this->zip = zip;
	}
	ZipPacket Client::getLastPacket(uint32_t& id)
	{
		ZipPacket packet;
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
	TCPClient::TCPClient() : socket{}
	{
		
	}
	TCPClient::~TCPClient()
	{
		connection = false;
		socket.close();
	}
	Socket::Status TCPClient::connect(const IPEndpoint& ip, const uint16_t& port)
	{
		if (connection) return Socket::Done;
		if (socket.getHandle() == INVALID_SOCKET)
			socket = TCPSocket();
		if (socket.connect(ip, port))
			return Socket::Error;
		connection = true;
		std::thread([&]() {
			ZipPacket packet;
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
	void TCPClient::send(ZipPacket& packet, uint32_t id)
	{
		socket.sendAll((const void*)&id, 4);
		socket.send(packet);
	}
	TCPSocket& TCPClient::getSocket()
	{
		return socket;
	}

	UDPClient::UDPClient() : socket{}
	{

	}
	UDPClient::~UDPClient()
	{
		connection = false;
		socket.close();
	}
	Socket::Status UDPClient::connect(const IPEndpoint& ip, const uint16_t& port)
	{
		if (connection) return Socket::Done;
		if (socket.getHandle() == INVALID_SOCKET)
			socket = UDPSocket();
		socket.bind(IPEndpoint::getLocalAddress(), 0);
		connection = true;
		this->ip = ip;
		this->port = port;
		uint32_t conn = 0x7FFFFFFF;
		ZipPacket pconn;
		socket.sendAll((void*)&conn, 4, this->ip, this->port);
		socket.send(pconn, this->ip, this->port);
		std::thread([&]() {
			ZipPacket packet;
			uint32_t id{};
			IPEndpoint ip;
			uint16_t port;
			Socket::Status status;
			while (connection)
			{
				status = socket.receiveAll((void*)&id, 4, ip, port);
				status = socket.receive(packet, ip, port);
				if (status != Socket::Done)
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
		swl::ZipPacket packet;
		send(packet, 0x7FFFFFFF);
		connection = false;
		socket.close();
	}
	void UDPClient::send(ZipPacket& packet, uint32_t id)
	{
		socket.sendAll((const void*)&id, 4, ip, port);
		socket.send(packet, ip, port);
	}
	UDPSocket& UDPClient::getSocket()
	{
		return socket;
	}
}