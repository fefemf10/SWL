#include "TCPSocket.hpp"

namespace swl
{
    TCPSocket::TCPSocket() : Socket{ socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) }
    {
        char value = 1;
        setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, &value, 1);
    }
    TCPSocket::TCPSocket(SOCKET& handle) : Socket{ handle }
    {

    }
    Socket::Status TCPSocket::listen(const int& backlog)
    {
        if (::listen(handle, backlog))
            return Status::Error;
        return Status::Done;
    }
    Socket::Status TCPSocket::accept(TCPSocket& socket)
    {
        sockaddr_in addr{};
        int len = sizeof(addr);
        SOCKET acceptedHandle = ::accept(handle, (sockaddr*)(&addr), &len);
        if (acceptedHandle == INVALID_SOCKET)
            return getErrorStatus();

        socket = TCPSocket(acceptedHandle);
        return Status::Done;
    }
    Socket::Status TCPSocket::connect(const IPEndpoint& endpoint, const uint16_t& port)
    {
        if (::connect(handle, (sockaddr*)(&IPEndpoint::createAddress(endpoint.toInteger(), port)), sizeof(sockaddr_in)))
            return getErrorStatus();
        return Status::Done;
    }
    Socket::Status TCPSocket::send(const void* data, const uint32_t& numberBytes, uint32_t& bytesSent)
    {
        bytesSent = ::send(handle, (const char*)data, numberBytes, NULL);
        if (bytesSent < numberBytes)
            return Socket::Partial;
        if (bytesSent > 2147483647)
            return getErrorStatus();
        return Status::Done;
    }
    Socket::Status TCPSocket::sendAll(const void* data, const uint32_t& numberBytes)
    {
        uint32_t totalBytesSent{ 0 };
        while (totalBytesSent < numberBytes)
        {
            uint32_t bytesRemainig = numberBytes - totalBytesSent;
            uint32_t bytesSent{ 0 };
            char* bufferOffset = (char*)data + totalBytesSent;
            Socket::Status status = send(bufferOffset, bytesRemainig, bytesSent);
            if (status != Status::Partial && status != Status::Done)
                return getErrorStatus();
            totalBytesSent += bytesSent;
        }
        return Status::Done;
    }
    Socket::Status TCPSocket::receive(void* destination, const uint32_t& numberBytes, uint32_t& bytesRecived)
    {
        bytesRecived = ::recv(handle, (char*)destination, numberBytes, NULL);
        if (bytesRecived < numberBytes)
            return Socket::Partial;
        if (bytesRecived > 2147483647)
            return getErrorStatus();
        return Status::Done;
    }
    Socket::Status TCPSocket::receiveAll(void* destination, const uint32_t& numberBytes)
    {
        uint32_t totalBytesReceived{ 0 };
        while (totalBytesReceived < numberBytes)
        {
            uint32_t bytesRemainig = numberBytes - totalBytesReceived;
            uint32_t bytesReceived{ 0 };
            char* bufferOffset = (char*)destination + totalBytesReceived;
            Socket::Status status = receive(bufferOffset, bytesRemainig, bytesReceived);
            if (status != Status::Partial && status != Status::Done)
                return getErrorStatus();
            totalBytesReceived += bytesReceived;
        }
        return Status::Done;
    }
    Socket::Status TCPSocket::send(Packet& packet)
    {
        uint32_t packetSize{ 0 };
        const void* data = packet.onSend(packetSize);
        if (sendAll(&packetSize, sizeof(uint32_t)))
            return getErrorStatus();
        if (sendAll(data, packetSize))
            return getErrorStatus();
        return Status::Done;
    }
    Socket::Status TCPSocket::receive(Packet& packet)
    {
        packet.clear();
        uint32_t packetSize{ 0 };
        if (receiveAll(&packetSize, sizeof(uint32_t)))
            return getErrorStatus();
        void* data = new char[packetSize];
        if (receiveAll(data, packetSize))
            return getErrorStatus();
        packet.onReceive(data, packetSize);
        delete[] data;
        return Status::Done;
    }
}