#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <cstdint>
#include <Audioclient.h>

namespace swl
{
	class AudioBuffer
	{
	public:
		AudioBuffer(const uint8_t& seconds, const uint32_t& samplingRate, const uint8_t& bitsPerSample);
		void copyData(const unsigned char* data, const uint32_t& bytes, int32_t& done);
		void loadData(unsigned char* data, const uint32_t& bytes, DWORD& flags);
		unsigned char* getBuffer();
		uint32_t getSizeRecorded() const;
		uint64_t getNanoSeconds() const;
	private:
		uint32_t samplingRate;
		uint32_t bytesRecorded;
		uint32_t size;
		uint32_t copyPos;
		uint32_t loadPos;
		uint8_t seconds;
		uint8_t bitsPerSample;
		unsigned char* buffer;
	};
}
