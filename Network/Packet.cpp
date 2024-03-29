#include "Packet.hpp"
#include <fstream>
#include <iostream>

namespace swl
{
	Packet::Packet() : readPos{ 0 }, data{}
	{
	}
	Packet::~Packet()
	{
	}
	void Packet::clear()
	{
		data.clear();
		readPos = 0;
	}
	void Packet::resize(const uint32_t& size)
	{
		data.resize(size);
	}
	uint32_t Packet::getSize() const
	{
		return static_cast<uint32_t>(data.size());
	}
	void* Packet::getData()
	{
		return data.data();
	}
	void Packet::append(const void* data, const uint32_t& size)
	{
		this->data.insert(this->data.end(), (char*)data, (char*)data + size);
	}
	Packet& Packet::operator<<(const std::string& data)
	{
		*this << (uint32_t)data.size();
		append(data.data(), static_cast<uint32_t>(data.size()));
		return *this;
	}
	Packet& Packet::operator>>(std::string& data)
	{
		if (readPos + sizeof(uint32_t) <= this->data.size())
		{
			uint32_t stringSize{ 0 };
			*this >> stringSize;
			if (readPos + stringSize <= this->data.size())
			{
				data.reserve(stringSize);
				data.assign(&this->data[readPos], stringSize);
				readPos += stringSize;
			}
		}
		return *this;
	}
	Packet& Packet::operator<<(swl::File& file)
	{
		*this << file.getFileName();
		*this << file.getDataSize();
		this->append(file.getFileData(), file.getDataSize());
		return *this;
	}
	Packet& Packet::operator>>(swl::File& file)
	{
		std::string tempName;
		std::vector<char> tempData;
		*this >> tempName;
		file.setFileName(tempName);
		*this >> tempData;
		file.setFileData(tempData);
		return *this;
	}
	const void* Packet::onSend(std::uint32_t& size)
	{
		size = getSize();
		return getData();
	}
	void Packet::onReceive(const void* data, const std::uint32_t& size)
	{
		append(data, size);
	}
}