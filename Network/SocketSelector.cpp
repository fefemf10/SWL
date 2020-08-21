#include "SocketSelector.hpp"

namespace swl
{
    SocketSelector::SocketSelector()
    {

    }
	void SocketSelector::add(Socket& socket)
	{
        SOCKET handle = socket.getHandle();
        if (handle != INVALID_SOCKET)
        {
            if (socketCount >= FD_SETSIZE)
                return;
            if (FD_ISSET(handle, &allSockets))
                return;
            socketCount++;
            FD_SET(handle, &allSockets);
        }
	}
	void SocketSelector::remove(Socket& socket)
	{
        SOCKET handle = socket.getHandle();
        if (handle != INVALID_SOCKET)
        {
            if (!FD_ISSET(handle, &allSockets))
                return;
            socketCount--;
            FD_CLR(handle, &allSockets);
            FD_CLR(handle, &readySockets);
        }
	}
	bool SocketSelector::wait(uint64_t millisecond)
	{
		timeval time{millisecond/1000, millisecond * 1000};
        readySockets = allSockets;
        int count = select(1, &readySockets, NULL, NULL, millisecond != 0 ? &time : NULL);
        return count > 0;
	}
	bool SocketSelector::isReady(Socket& socket)
	{
        SOCKET handle = socket.getHandle();
        if (handle != INVALID_SOCKET)
            return FD_ISSET(handle, &readySockets) != 0;
        return false;
	}
	void SocketSelector::clear()
	{
        FD_ZERO(&allSockets);
        FD_ZERO(&readySockets);
        socketCount = 0;
	}
}