#include "WAV.hpp"

namespace swl
{
	WAV::WAV(const std::string& path, const bool& read) : read{ read }
	{
		file.open(path, read ? std::ios::in : std::ios::out | std::ios::binary);
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
	}
	void WAV::copyData(const char* data, const uint32_t& bytes, bool& done)
	{

	}
	void WAV::loadData(char* data, const uint32_t& bytes, bool& done)
	{
	}
}