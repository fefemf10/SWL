#pragma once
#include <string>
#include "TCPSocket.hpp"
namespace swl
{
	class HTTP
	{
	public:
		static std::string sendRequest(const std::string& url);
	};
}