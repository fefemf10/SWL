#pragma once

#include <windows.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <cstdint>
#include "AudioBuffer.hpp"

namespace swl
{
    class Audio
    {
    public:
        Audio(const uint32_t& samplingRate, const uint8_t bitsPerSample, const uint8_t& channels);
        void record();
        void playback();
        AudioBuffer buffer;
    private:
        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        const IID IID_IAudioClient = __uuidof(IAudioClient);
        const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
        const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

        

        tWAVEFORMATEX* format;
        uint32_t samplingRate;
        uint8_t bitsPerSample;
        uint8_t channels;
        IMMDeviceEnumerator* pEnumerator = nullptr;
        IMMDevice* pDevice = nullptr;
        IAudioClient* pAudioClient = nullptr;
        IAudioCaptureClient* pCaptureClient = nullptr;
        IAudioRenderClient* pRenderClient = nullptr;
    };
}