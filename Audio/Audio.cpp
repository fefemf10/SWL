#include "Audio.hpp"

namespace swl
{
	Audio::Audio(const uint32_t& samplingRate, const uint8_t bitsPerSample, const uint8_t& channels, AudioBuffer& buffer) : buffer(buffer), waveInBuffer1{}, waveInBuffer2{}, waveOutBuffer1{}, waveOutBuffer2{}
	{
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = channels;
		format.nSamplesPerSec = samplingRate;
		format.wBitsPerSample = bitsPerSample;
		format.nBlockAlign = format.nChannels * (format.wBitsPerSample / 8);
		format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
		format.cbSize = 0;

		waveInBuffer1.lpData = new char[format.nSamplesPerSec * 1 * 2];
		waveInBuffer1.dwBufferLength = format.nSamplesPerSec * 1 * 2;

		waveInBuffer2.lpData = new char[format.nSamplesPerSec * 1 * 2];
		waveInBuffer2.dwBufferLength = format.nSamplesPerSec * 1 * 2;

		waveOutBuffer1.lpData = new char[format.nSamplesPerSec * 1 * 2];
		waveOutBuffer1.dwBufferLength = format.nSamplesPerSec * 1 * 2;

		waveOutBuffer2.lpData = new char[format.nSamplesPerSec * 1 * 2];
		waveOutBuffer2.dwBufferLength = format.nSamplesPerSec * 1 * 2;
	}
	void Audio::record()
	{
		waveInOpen(&waveIn, WAVE_MAPPER, &format, (DWORD_PTR)&Audio::recordCallback, (DWORD_PTR)this, CALLBACK_FUNCTION);
		waveInPrepareHeader(waveIn, &waveInBuffer1, sizeof(wavehdr_tag));
		waveInPrepareHeader(waveIn, &waveInBuffer2, sizeof(wavehdr_tag));
		waveInAddBuffer(waveIn, &waveInBuffer1, sizeof(wavehdr_tag));
		recording = true;
		waveInStart(waveIn);
	}
	void Audio::recordStop()
	{
		recording = false;
		waveInStop(waveIn);
		waveInUnprepareHeader(waveIn, &waveInBuffer1, sizeof(wavehdr_tag));
		waveInUnprepareHeader(waveIn, &waveInBuffer2, sizeof(wavehdr_tag));
		waveInClose(waveIn);
	}
	void Audio::play()
	{
		waveOutOpen(&waveOut, WAVE_MAPPER, &format, (DWORD_PTR)&Audio::playCallback, (DWORD_PTR)this, CALLBACK_FUNCTION);
		waveOutPrepareHeader(waveOut, &waveOutBuffer1, sizeof(wavehdr_tag));
		waveOutPrepareHeader(waveOut, &waveOutBuffer2, sizeof(wavehdr_tag));
		buffer.out(waveOutBuffer1.lpData, samplingRate * 1 * 2);
		playing = true;
		Sleep(10);
		waveOutWrite(waveOut, &waveOutBuffer1, sizeof(wavehdr_tag));
		waveOutRestart(waveOut);
	}
	void Audio::playStop()
	{
		playing = false;
		waveOutReset(waveOut);
		waveOutUnprepareHeader(waveOut, &waveOutBuffer1, sizeof(wavehdr_tag));
		waveOutUnprepareHeader(waveOut, &waveOutBuffer2, sizeof(wavehdr_tag));
		waveOutClose(waveOut);
	}
	void Audio::recordCallback(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		Audio* self = (Audio*)(dwInstance);
		wavehdr_tag* wavehdr = NULL;

		switch (uMsg)
		{
		case WIM_DATA:
			wavehdr = (WAVEHDR*)dwParam1;
			if (wavehdr->dwFlags & WHDR_DONE)
			{
				if (self->recording)
				{
					if (&self->waveInBuffer1 == wavehdr)
						waveInAddBuffer(hwi, &self->waveInBuffer2, sizeof(wavehdr_tag));
					else
						waveInAddBuffer(hwi, &self->waveInBuffer1, sizeof(wavehdr_tag));
				}
				self->buffer.in(wavehdr->lpData, wavehdr->dwBufferLength);
			}
			break;
		}
	}
	void Audio::playCallback(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		Audio* self = (Audio*)(dwInstance);
		wavehdr_tag* wavehdr = NULL;

		switch (uMsg)
		{
		case WOM_DONE:
			wavehdr = (WAVEHDR*)dwParam1;
			if (wavehdr->dwFlags & WHDR_DONE)
			{
				if (self->playing)
				{
					if (&self->waveOutBuffer1 == wavehdr)
					{
						self->buffer.out(self->waveOutBuffer2.lpData, wavehdr->dwBufferLength);
						waveOutWrite(self->waveOut, &self->waveOutBuffer2, sizeof(wavehdr_tag));
					}
					else
					{
						self->buffer.out(self->waveOutBuffer1.lpData, wavehdr->dwBufferLength);
						waveOutWrite(self->waveOut, &self->waveOutBuffer1, sizeof(wavehdr_tag));
					}
				}
			}
			break;
		}
	}
}