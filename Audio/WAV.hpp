#pragma once
#include <string>
#include <fstream>

namespace swl
{
	class WAV
	{
	public:
		WAV(const std::string& path, const bool& read);
		~WAV();
		void close();
		void writeStruct();
		void copyData(const char* data, const uint32_t& bytes, bool& done);
		void loadData(char* data, const uint32_t& bytes, bool& done);
		//short extraFormatBytes = AudioIn::reverse(0);
		//data
		uint32_t samplingRate;
		uint8_t bitsPerSample;
		uint8_t channels;
	private:
		bool read;
		std::fstream file;
		uint32_t chunkSize;
		uint32_t subchunk2Size;
		uint32_t readPos;
	};
}