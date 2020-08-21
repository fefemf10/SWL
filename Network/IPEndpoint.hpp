#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <sstream>
#include <iostream>
#include <WS2tcpip.h>

namespace swl
{
	class IPEndpoint
	{
	public:
		IPEndpoint();
		IPEndpoint(const std::string& ip);
		IPEndpoint(const uint32_t& ip);
		IPEndpoint(const in_addr& addr);
		std::string toString() const;
		uint32_t toInteger() const;
		static IPEndpoint getLocalAddress();
		static sockaddr_in createAddress(const uint32_t& ip, const uint16_t& port);
		bool operator == (const IPEndpoint& ip);
		bool operator != (const IPEndpoint& ip);
	private:
		in_addr ip;
	};
}
