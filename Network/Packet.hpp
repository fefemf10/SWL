#pragma once
#include <WinSock2.h>
#include "File.hpp"
#include <vector>
#include <string>

namespace swl
{
	class TCPSocket;
	class UDPSocket;
	class Packet
	{
	public:
		enum Type
		{
			Chat,
			File,
			Audio
		};
		Packet();
		virtual ~Packet();
		void clear();
		void resize(const uint32_t& size);
		uint32_t getSize() const;
		void* getData();
		void append(const void* data, const uint32_t& size);
		template <typename T>
		Packet& operator << (const T& data);
		template <typename T>
		Packet& operator >> (T& data);
		template <typename T>
		Packet& operator << (const std::vector<T>& data);
		template <typename T>
		Packet& operator >> (std::vector<T>& data);
		Packet& operator << (const std::string& data);
		Packet& operator >> (std::string& data);
		Packet& operator << (swl::File& file);
		Packet& operator >> (swl::File& file);
	protected:
		friend TCPSocket;
		friend UDPSocket;
		virtual const void* onSend(std::uint32_t& size);
		virtual void onReceive(const void* data, const std::uint32_t& size);
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
	template <typename T>
	Packet& Packet::operator << (const std::vector<T>& data)
	{
		*this << data.size();
		append(data.data(), sizeof(T) * data.size());
		return *this;
	}
	template <typename T>
	Packet& Packet::operator >> (std::vector<T>& data)
	{
		uint32_t size{ 0 };
		*this >> size;
		if (readPos + sizeof(T)*size <= this->data.size())
		{
			data.resize(size);
			memcpy(data.data(), this->data.data() + readPos, sizeof(T) * size);
			readPos += sizeof(T)* size;
		}
		return *this;
	}
}