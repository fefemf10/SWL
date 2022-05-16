#pragma once
#include <windows.h>
#include <cstdint>
#include "AudioBuffer.hpp"

namespace swl
{
    class Audio
    {
    public:
        Audio(const uint32_t& samplingRate, const uint8_t bitsPerSample, const uint8_t& channels, AudioBuffer& buffer);
        void record();
        void recordStop();
        void play();
        void playStop();
        tWAVEFORMATEX format;
        uint32_t samplingRate;
        uint8_t bitsPerSample;
        uint8_t channels;
        HWAVEIN waveIn;
        HWAVEOUT waveOut;
        wavehdr_tag waveInBuffer1;
        wavehdr_tag waveInBuffer2;
        wavehdr_tag waveOutBuffer1;
        wavehdr_tag waveOutBuffer2;
        bool recording;
        bool playing;
        AudioBuffer& buffer;
    private:
        void static CALLBACK recordCallback(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
        void static CALLBACK playCallback(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
    };
}