#include "AudioOut.hpp"

namespace swl
{
	AudioOut::AudioOut(const uint32_t& samplingRate, const uint8_t bitsPerSample, const uint8_t& channels) : buffer1(10, samplingRate, bitsPerSample), buffer2(10, samplingRate, bitsPerSample)
	{
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = channels;
		format.wBitsPerSample = bitsPerSample;
		format.nSamplesPerSec = samplingRate;
		format.nAvgBytesPerSec = samplingRate * channels * bitsPerSample / 8;;
		format.nBlockAlign = channels * bitsPerSample / 8;
		format.cbSize = 0;
		using std::placeholders::_1;
		result = waveOutOpen(&handle, WAVE_MAPPER, &format, (DWORD_PTR)&AudioOut::startPlay, CALLBACK_FUNCTION, WAVE_FORMAT_DIRECT);
		waveOutPrepareHeader(handle, buffer1.getWaveBuffer(), sizeof(wavehdr_tag));
		waveOutPrepareHeader(handle, buffer2.getWaveBuffer(), sizeof(wavehdr_tag));
		bufferIndex = 1;
	}
	void AudioOut::start()
	{
		waveOutRestart(handle);
	}
	void AudioOut::stop()
	{
		waveOutUnprepareHeader(handle, buffer1.getWaveBuffer(), sizeof(wavehdr_tag));
		waveOutUnprepareHeader(handle, buffer2.getWaveBuffer(), sizeof(wavehdr_tag));
		waveOutClose(handle);
	}
	void CALLBACK AudioOut::startPlay(HWAVEOUT handle, UINT uMsg, DWORD dwInstance, DWORD param1, DWORD param2)
	{
		AudioOut* temp = (AudioOut*)GetWindowLongPtr((HWND)handle, GWLP_USERDATA);
		if (uMsg == WOM_DONE) {

			if (temp->bufferIndex == 1)
			{
				waveOutWrite(handle, temp->buffer1.getWaveBuffer(), sizeof(wavehdr_tag));
				temp->bufferIndex = 2;
			}
			else
			{
				waveOutWrite(handle, temp->buffer2.getWaveBuffer(), sizeof(wavehdr_tag));
				temp->bufferIndex = 1;
			}
		}
	}
}