#pragma once
#include <cstdint>
#include "Socket.hpp"

namespace swl
{
	class SocketSelector
	{
	public:
		SocketSelector();
		void add(Socket& socket);
		void remove(Socket& socket);
		bool wait(uint64_t millisecond = 0);
		bool isReady(Socket& socket);
		void clear();
	private:
		fd_set allSockets;
		fd_set readySockets;
		uint32_t socketCount;
	};
}