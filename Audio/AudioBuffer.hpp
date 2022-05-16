#pragma once
#include <Windows.h>
#include <vector>
#include <cstdint>

namespace swl
{
	class AudioBuffer
	{
	public:
		AudioBuffer(const uint16_t& seconds, const uint32_t& samplingRate, const uint8_t& bitsPerSample);
		void in(const void* data, uint32_t bytes);
		void out(void* data, uint32_t bytes);
		char* getBuffer();
		uint32_t getSizeRecorded() const;
	private:
		uint32_t readPos;
		uint32_t samplingRate;
		uint16_t seconds;
		uint8_t bitsPerSample;
		std::vector<char> buffer;
	};
}
