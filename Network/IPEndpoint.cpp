#include "IPEndpoint.hpp"

namespace swl
{
	IPEndpoint::IPEndpoint()
	{
	}
	IPEndpoint::IPEndpoint(const std::string& ip)
	{
		inet_pton(AF_INET, ip.c_str(), &this->ip.S_un.S_addr);
	}
	IPEndpoint::IPEndpoint(const uint32_t& ip)
	{
		this->ip.S_un.S_addr = ip;
	}
	IPEndpoint::IPEndpoint(const in_addr& addr)
	{
		ip = addr;
	}
	IPEndpoint& IPEndpoint::getLocalAddress()
	{
		IPEndpoint localAddress;
		SOCKET sock = ::socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == INVALID_SOCKET)
			return localAddress;

		sockaddr_in address;
		address.sin_addr.s_addr = INADDR_LOOPBACK;
		address.sin_family = AF_INET;
		address.sin_port = htons(9);
		if (connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
		{
			::closesocket(sock);
			return localAddress;
		}
		int size = sizeof(address);
		if (getsockname(sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
		{
			::closesocket(sock);
			return localAddress;
		}
		::closesocket(sock);
		localAddress = IPEndpoint(address.sin_addr);

		return localAddress;
	}
	sockaddr_in IPEndpoint::createAddress(const uint32_t& ip, const uint16_t& port)
	{
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = ip;
		addr.sin_port = port;
		return addr;
	}
	std::string IPEndpoint::toString() const
	{
		return std::string(std::to_string(ip.S_un.S_addr >> 24 & 0xFF) + "." + std::to_string(ip.S_un.S_addr >> 16 & 0xFF) + "." + std::to_string(ip.S_un.S_addr >> 8 & 0xFF) + "." + std::to_string(ip.S_un.S_addr & 0xFF));
	}
	uint32_t IPEndpoint::toInteger() const
	{
		return ip.S_un.S_addr;
	}
}