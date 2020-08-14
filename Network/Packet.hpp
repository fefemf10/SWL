#pragma once
#include <WinSock2.h>
#include <vector>
#include <string>

namespace swl
{
	class Packet
	{
	public:
		void clear();
		void resize(const uint32_t& size);
		uint32_t getSize() const;
		void* getData();
		void append(const void* data, const uint32_t& size);
		template <typename T>
		Packet& operator << (const T& data);
		template <typename T>
		Packet& operator >> (T& data);
		Packet& operator << (const std::string& data);
		Packet& operator >> (std::string& data);
	private:
		uint32_t readPos = 0;
		std::vector<char> data;
	};

	template <typename T>
	Packet& Packet::operator << (const T& data)
	{
		append(&data, sizeof(T));
		return *this;
	}
	template <typename T>
	Packet& Packet::operator >> (T& data)
	{
		if (readPos + sizeof(T) <= this->data.size())
		{
			data = *reinterpret_cast<T*>(&this->data[readPos]);
			readPos += sizeof(T);
		}
		return *this;
	}
}