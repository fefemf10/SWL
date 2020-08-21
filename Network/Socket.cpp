#include "Socket.hpp"

namespace swl
{
	Socket::Socket(SOCKET handle)
	{
		this->handle = handle;
	}
	Socket::~Socket()
	{
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
	Socket::Status Socket::setBlocking(const bool& blocking)
	{
		unsigned long b = !blocking;
		if (ioctlsocket(handle, FIONBIO, &b))
			return getErrorStatus();
		return Socket::Done;
	}
	Socket::Status Socket::getErrorStatus()
	{
		switch (WSAGetLastError())
		{
		case WSAEWOULDBLOCK:  return Socket::NotReady;
		case WSAEALREADY:     return Socket::NotReady;
		case WSAECONNABORTED: return Socket::Disconnected;
		case WSAECONNRESET:   return Socket::Disconnected;
		case WSAETIMEDOUT:    return Socket::Disconnected;
		case WSAENETRESET:    return Socket::Disconnected;
		case WSAENOTCONN:     return Socket::Disconnected;
		case WSAEISCONN:      return Socket::Done; // when connecting a non-blocking socket
		default:              return Socket::Error;
		}
	}
}