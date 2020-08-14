#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <cstdint>
#include "AudioBuffer.hpp"

namespace swl
{
	class AudioIn
	{
	public:
		AudioIn(const uint32_t& samplingRate, const uint8_t bitsPerSample, const uint8_t& channels);
		void start();
		void stop();
		AudioBuffer buffer1, buffer2;
		int bufferIndex;
	private:
		static void CALLBACK startCapture(HWAVEIN handle, UINT uMsg, DWORD dwInstance, DWORD param1, DWORD param2);
		uint32_t samplingRate;
		uint8_t bitsPerSample;
		uint8_t channels;
		tWAVEFORMATEX format;
		HRESULT hr;
	};
	
}