#include "HTTP.hpp"
namespace swl
{
    std::string HTTP::sendRequest(const std::string& url)
    {
        std::string protocol;
        std::string host;
        std::string request;
        size_t pos1 = url.find("://");
        size_t pos2 = url.find("/", pos1 != std::string::npos ? pos1 : 0);
        if (pos1 != std::string::npos)
        {
            protocol = url.substr(0, pos1);
        }
        if (pos2 != std::string::npos)
        {
            host = url.substr(pos1 != std::string::npos ? pos1 : 0, pos2);
            request = url.substr(pos2);
        }

        swl::TCPSocket socket;
        socket.connect(swl::IPEndpoint(host), 80);
        std::string header("GET " + request + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n");
        socket.sendAll(header.data(), header.size());

        char buffer[1 << 16]{};
        uint32_t size{};
        socket.receive(buffer, 1 << 16, size);

        socket.close();
        return buffer;
    }
}