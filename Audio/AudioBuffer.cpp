#include "AudioBuffer.hpp"


namespace swl
{
    AudioBuffer::AudioBuffer(const uint8_t& seconds, const uint32_t& samplingRate, const uint8_t& bitsPerSample) : seconds{ seconds }, samplingRate{ samplingRate }, bitsPerSample{ bitsPerSample }, copyPos{ 0 }, loadPos{ 0 }
	{
		buffer = new unsigned char[samplingRate * seconds * bitsPerSample/8];
        size = samplingRate * seconds * bitsPerSample / 8;
	}
    void AudioBuffer::copyData(const unsigned char* data, const uint32_t& bytes, int32_t& done)
    {
        memcpy(buffer+copyPos, data, bytes);
        copyPos += bytes;
        if (copyPos >= size)
            done = true;
        else
            done = false;
    }
    void AudioBuffer::loadData(unsigned char* data, const uint32_t& bytes, DWORD& flags)
    {
        if (loadPos >= bytesRecorded)
            flags = AUDCLNT_BUFFERFLAGS_SILENT;
        else
        {
            memcpy(data, data + loadPos, bytes);
            loadPos += bytes;
            flags = 0;
        }
    }
    unsigned char* AudioBuffer::getBuffer()
    {
        return buffer;
    }
    uint32_t AudioBuffer::getSizeRecorded() const
    {
        return bytesRecorded;
    }
    uint64_t AudioBuffer::getNanoSeconds() const
    {
        return seconds * 10000000ULL;
    }
}