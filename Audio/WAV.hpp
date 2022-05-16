#pragma once
#include <string>
#include <fstream>
#include "AudioBuffer.hpp"

namespace swl
{
	class WAV
	{
	public:
		WAV(const std::string& path, const bool& read);
		~WAV();
		void close();
		void writeStruct();
		void copyData(const char* data, const uint32_t& bytes);
		void loadData(AudioBuffer& buffer, const uint32_t& bytes = 0);

		uint32_t samplingRate;
		uint16_t bitsPerSample;
		uint8_t channels;
	private:
		bool read;
		std::fstream file;
		uint32_t chunkSize;
		uint32_t subchunk2Size;
		uint32_t readPos;
		uint32_t writePos;
	};
}