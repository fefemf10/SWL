#include "TCPSocket.hpp"

namespace swl
{
    TCPSocket::TCPSocket() : Socket{ socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) }
    {
        setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (char*)0x01, 1);
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
            return Status::Error;

        socket = TCPSocket(acceptedHandle);
        return Status::Done;
    }
    Socket::Status TCPSocket::connect(const IPEndpoint& endpoint, const uint16_t& port)
    {
        if (::connect(handle, (sockaddr*)(&IPEndpoint::createAddress(endpoint.toInteger(), port)), sizeof(sockaddr_in)))
            return Status::Error;
        return Status::Done;
    }
    Socket::Status TCPSocket::send(const void* data, const uint32_t& numberBytes, uint32_t& bytesSent)
    {
        bytesSent = ::send(handle, (const char*)data, numberBytes, NULL);
        //std::cout << WSAGetLastError() << std::endl;
        if (bytesSent == SOCKET_ERROR)
            return Status::Error;
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
            if (send(bufferOffset, bytesRemainig, bytesSent))
                return Status::Error;
            totalBytesSent += bytesSent;
        }
        return Status::Done;
    }
    Socket::Status TCPSocket::receive(void* destination, const uint32_t& numberBytes, uint32_t& bytesRecived)
    {
        bytesRecived = ::recv(handle, (char*)destination, numberBytes, NULL);
        //std::cout << WSAGetLastError() << std::endl;
        if (bytesRecived == 0)
            return Status::Error;
        if (bytesRecived == SOCKET_ERROR)
            return Status::Error;
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
            if (receive(bufferOffset, bytesRemainig, bytesReceived))
                return Status::Error;
            totalBytesReceived += bytesReceived;
        }
        return Status::Done;
    }
    Socket::Status TCPSocket::send(Packet& packet)
    {
        uint32_t packetSize = htonl(packet.getSize());
        if (sendAll(&packetSize, sizeof(uint32_t)))
            return Status::Error;

        if (sendAll(packet.getData(), packet.getSize()))
            return Status::Error;
        return Status::Done;
    }
    Socket::Status TCPSocket::receive(Packet& packet)
    {
        packet.clear();
        uint32_t packetSize{ 0 };
        if (receiveAll(&packetSize, sizeof(uint32_t)))
            return Status::Error;

        packetSize = ntohl(packetSize);
        packet.resize(packetSize);

        if (receiveAll(packet.getData(), packetSize))
            return Status::Error;
        return Status::Done;
    }
}