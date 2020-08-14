#include "Socket.hpp"

namespace swl
{
	Socket::Socket(SOCKET handle)
	{
		this->handle = handle;
	}
	Socket::Status Socket::bind(const IPEndpoint& endpoint, const uint16_t& port)
	{
		if (::bind(handle, (sockaddr*)(&IPEndpoint::createAddress(endpoint.toInteger(), port)), sizeof(sockaddr_in)))
			return Status::Error;
		return Status::Done;
	}
	Socket::Status Socket::close()
	{
		if (handle == INVALID_SOCKET)
			return Status::Error;
		if (closesocket(handle))
			return Status::Error;
		handle = INVALID_SOCKET;
		return Status::Done;
	}
	SOCKET Socket::getHandle()
	{
		return handle;
	}
}