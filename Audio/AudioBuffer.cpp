#include "AudioBuffer.hpp"


namespace swl
{
	AudioBuffer::AudioBuffer(const uint16_t& seconds, const uint32_t& samplingRate, const uint8_t& bitsPerSample) : seconds{ seconds }, samplingRate{ samplingRate }, bitsPerSample{ bitsPerSample }, readPos{}
	{
		buffer.reserve(samplingRate * seconds * bitsPerSample/8);
	}
	void AudioBuffer::in(const void* data, uint32_t bytes)
	{
		buffer.insert(buffer.end(), (char*)data, (char*)data + bytes);
	}

	void AudioBuffer::out(void* data, uint32_t bytes)
	{
		if (readPos + bytes <= buffer.size())
		{
			memcpy((char*)data, buffer.data() + readPos, bytes);
			readPos += bytes;
		}
		else
		{
			memcpy((char*)data, buffer.data() + readPos, buffer.size() - readPos);
			readPos = buffer.size();
		}
	}
	
	char* AudioBuffer::getBuffer()
	{
		return buffer.data();
	}
	uint32_t AudioBuffer::getSizeRecorded() const
	{
		return buffer.size();
	}
}