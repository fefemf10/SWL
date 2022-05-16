#pragma once
#include <cstdint>
#include <queue>
#include <thread>
#include "IPEndpoint.hpp"
#include "TCPSocket.hpp"
#include "UDPSocket.hpp"
#include "ZipPacket.hpp"

namespace swl
{
	class Client
	{
	public:
		Client();
		virtual ~Client();
		virtual Socket::Status connect(const IPEndpoint& ip, const uint16_t& port) = 0;
		virtual void disconnect() = 0;
		virtual void send(ZipPacket& packet, uint32_t id) = 0;
		void setSettingsSend(const bool& encrypt, const bool& zip);
		ZipPacket getLastPacket(uint32_t& id);
		bool isConnected() const;
	protected:
		bool encrypt = false;
		bool zip = false;
		bool connection;
		uint32_t packetId;
		std::queue<std::pair<ZipPacket, uint32_t>> packets;
	};
	class TCPClient : public Client
	{
	public:
		TCPClient();
		~TCPClient() override;
		Socket::Status connect(const IPEndpoint& ip, const uint16_t& port) override;
		void disconnect() override;
		void send(ZipPacket& packet, uint32_t id) override;
		TCPSocket& getSocket();
	private:
		TCPSocket socket;
	};
	class UDPClient : public Client
	{
	public:
		UDPClient();
		~UDPClient() override;
		Socket::Status connect(const IPEndpoint& ip, const uint16_t& port) override;
		void disconnect() override;
		void send(ZipPacket& packet, uint32_t id) override;
		UDPSocket& getSocket();
	private:
		IPEndpoint ip;
		uint16_t port;
		UDPSocket socket;
	};
}