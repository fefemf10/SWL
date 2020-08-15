#include "UDPSocket.hpp"

namespace swl
{
    UDPSocket::UDPSocket() : Socket{ socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) }
    {
        
    }
    Socket::Status UDPSocket::send(const void* data, const uint32_t& numberBytes, uint32_t& bytesSent, const IPEndpoint& ip, const uint16_t& port)
    {
        bytesSent = ::sendto(handle, (const char*)data, numberBytes, NULL, (sockaddr*)&IPEndpoint::createAddress(ip.toInteger(), port), sizeof(sockaddr));
        if (bytesSent > 2147483647)
            return getErrorStatus();
        return Status::Done;
    }
    Socket::Status UDPSocket::sendAll(const void* data, const uint32_t& numberBytes, const IPEndpoint& ip, const uint16_t& port)
    {
        uint32_t totalBytesSent{ 0 };
        while (totalBytesSent < numberBytes)
        {
            uint32_t bytesRemainig = numberBytes - totalBytesSent;
            uint32_t bytesSent{ 0 };
            char* bufferOffset = (char*)data + totalBytesSent;
            Socket::Status status = send(bufferOffset, bytesRemainig, bytesSent, ip, port);
            if (status != Status::Partial && status != Status::Done)
                return getErrorStatus();
            totalBytesSent += bytesSent;
        }
        return Status::Done;
    }
    Socket::Status UDPSocket::receive(void* destination, const uint32_t& numberBytes, uint32_t& bytesRecived, IPEndpoint& ip, uint16_t& port)
    {
        sockaddr_in addr{};
        int sizeAddr = sizeof(addr);
        bytesRecived = ::recvfrom(handle, (char*)destination, numberBytes, NULL, (sockaddr*)(&addr), &sizeAddr);
        if (bytesRecived > 2147483647)
            return getErrorStatus();
        ip = IPEndpoint(addr.sin_addr);
        port = addr.sin_port;
        return Status::Done;
    }
    Socket::Status UDPSocket::receiveAll(void* destination, const uint32_t& numberBytes, IPEndpoint& ip, uint16_t& port)
    {
        uint32_t totalBytesReceived{ 0 };
        while (totalBytesReceived < numberBytes)
        {
            uint32_t bytesRemainig = numberBytes - totalBytesReceived;
            uint32_t bytesReceived{ 0 };
            char* bufferOffset = (char*)destination + totalBytesReceived;
            Socket::Status status = receive(bufferOffset, bytesRemainig, bytesReceived, ip, port);
            if (status != Status::Partial && status != Status::Done)
                return getErrorStatus();
            totalBytesReceived += bytesReceived;
        }
        return Status::Done;
    }
    Socket::Status UDPSocket::send(Packet& packet, const IPEndpoint& ip, const uint16_t& port)
    {
        uint32_t packetSize{ 0 };
        const void* data = packet.onSend(packetSize);
        if (sendAll(&packetSize, sizeof(uint32_t), ip, port))
            return getErrorStatus();
        if (sendAll(data, packetSize, ip, port))
            return getErrorStatus();
        return Status::Done;
    }
    Socket::Status UDPSocket::receive(Packet& packet, IPEndpoint& ip, uint16_t& port)
    {
        packet.clear();
        uint32_t packetSize{ 0 };
        if (receiveAll(&packetSize, sizeof(uint32_t), ip, port))
            return getErrorStatus();
        void* data = new char[packetSize];
        if (receiveAll(data, packetSize, ip, port))
            return getErrorStatus();
        packet.onReceive(data, packetSize);
        delete[] data;
        return Status::Done;
    }
}