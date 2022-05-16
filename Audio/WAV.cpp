#include "WAV.hpp"

namespace swl
{
	WAV::WAV(const std::string& path, const bool& read) : read{ read }, writePos{44}
	{
		file.open(path, read ? (std::ios::in | std::ios::binary) : (std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc));
	}
	WAV::~WAV()
	{
		file.close();
	}
	void WAV::close()
	{
		file.close();
	}
	void WAV::writeStruct()
	{
		file.write("RIFF", 4);
		//chunkSize
		uint32_t temp = 0;
		file.write((char*)&temp, 4);

		file.write("WAVE", 4);
		file.write("fmt ", 4);
		temp = 16;
		file.write((char*)&temp, 4);
		temp = 1;
		file.write((char*)&temp, 2);
		file.write((char*)&channels, 2);
		file.write((char*)&samplingRate, 4);
		temp = samplingRate * bitsPerSample / 8 * channels;
		file.write((char*)&temp, 4);
		temp = bitsPerSample / 8 * channels;
		file.write((char*)&temp, 2);
		file.write((char*)&bitsPerSample, 2);
		file.write("data", 4);
		temp = 0;
		file.write((char*)&temp, 4);
	}
	void WAV::copyData(const char* data, const uint32_t& bytes)
	{
		uint32_t temp{1};
		file.seekg(40, file.beg);
		file.read((char*)&temp, 4);
		file.seekp(40, file.beg);
		temp += bytes;
		file.write((char*)&temp, 4);
		file.seekp(writePos, file.beg);
		file.write(data, bytes);

		file.seekg(0, file.end);
		int sizeFile = (int)(file.tellg()) - 8;
		file.seekp(4, file.beg);
		file.write((char*)&sizeFile, 4);
		writePos += bytes;
	}
	void WAV::loadData(AudioBuffer& buffer, const uint32_t& bytes)
	{
		uint32_t sizeBuffer;
		if (bytes == 0)
		{
			file.seekg(40, file.beg);
			file.read((char*)&sizeBuffer, 4);
			file.read(buffer.getBuffer(), sizeBuffer);
		}
		else
		{
			file.seekg(44, file.beg);
			file.read(buffer.getBuffer(), bytes);
		}
	}
}