#pragma once
#include <WinSock2.h>
#include "IPEndpoint.hpp"
#include "Packet.hpp"

namespace swl
{
	class Socket
	{
	public:
		enum Status
		{
			Done,
			NotReady,
			Partial,
			Disconnected,
			Error
		};
		enum Option
		{
			TCPNoDelay,
			SOLinger
		};
		Socket(SOCKET handle);
		Status bind(const IPEndpoint& endpoint, const uint16_t& port);
		Status close();
		SOCKET getHandle();
		Status setBlocking(const bool& blocking);
	protected:
		SOCKET handle;
		Status getErrorStatus();
	};
}