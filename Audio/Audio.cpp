#include "Audio.hpp"

namespace swl
{
	Audio::Audio(const uint32_t& samplingRate, const uint8_t bitsPerSample, const uint8_t& channels) : buffer(1, samplingRate, bitsPerSample)
	{
		/*format->wFormatTag = WAVE_FORMAT_PCM;
		format->nChannels = channels;
		format->nSamplesPerSec = samplingRate;
		format->wBitsPerSample = bitsPerSample;
		format->nBlockAlign = format->nChannels * (format->wBitsPerSample / 8);
		format->nAvgBytesPerSec = format->nSamplesPerSec * format->nBlockAlign;
		format->cbSize = 0;*/
	}
	void Audio::record()
	{
		//nanosec
		REFERENCE_TIME hnsRequestedDuration = 10000000;
		REFERENCE_TIME hnsActualDuration;
		UINT32 bufferFrameCount;
		UINT32 numFramesAvailable;
		UINT32 packetLength = 0;
		BOOL bDone = FALSE;
		BYTE* pData;
		DWORD flags;

		CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
		pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
		pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&pAudioClient);
		pAudioClient->GetMixFormat(&format);
		pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, format, nullptr);
		// Get the size of the allocated buffer.
		pAudioClient->GetBufferSize(&bufferFrameCount);
		pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
		// Calculate the actual duration of the allocated buffer.
		hnsActualDuration = (double)10000000 * bufferFrameCount / format->nSamplesPerSec;
		pAudioClient->Start();  // Start recording.
		// Each loop fills about half of the shared buffer.
		while (bDone == FALSE)
		{
			// Sleep for half the buffer duration.
			Sleep(hnsActualDuration / 10000 / 2);
			pCaptureClient->GetNextPacketSize(&packetLength);
			while (packetLength != 0)
			{
				// Get the available data in the shared buffer.
				pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
				if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
				{
					pData = nullptr;  // Tell CopyData to write silence.
				}
				// Copy the available capture data to the audio sink.
				buffer.copyData(pData, numFramesAvailable, bDone);
				pCaptureClient->ReleaseBuffer(numFramesAvailable);
				pCaptureClient->GetNextPacketSize(&packetLength);
			}
		}
		pAudioClient->Stop();
	}
	void Audio::playback()
	{
		REFERENCE_TIME hnsRequestedDuration = 10000000;
		REFERENCE_TIME hnsActualDuration;
		UINT32 bufferFrameCount;
		UINT32 numFramesAvailable;
		UINT32 numFramesPadding;
		BYTE* pData;
		DWORD flags = 0;

		CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
		pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
		pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&pAudioClient);
		pAudioClient->GetMixFormat(&format);
		pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, format, nullptr);
		// Get the actual size of the allocated buffer.
		pAudioClient->GetBufferSize(&bufferFrameCount);
		pAudioClient->GetService(IID_IAudioRenderClient, (void**)&pRenderClient);
		// Grab the entire buffer for the initial fill operation.
		pRenderClient->GetBuffer(bufferFrameCount, &pData);
		// Load the initial data into the shared buffer.
		buffer.loadData(pData, bufferFrameCount, flags);
		pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
		// Calculate the actual duration of the allocated buffer.
		hnsActualDuration = (double)10000000 * bufferFrameCount / format->nSamplesPerSec;
		pAudioClient->Start();
		// Each loop fills about half of the shared buffer.
		while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
		{
			// Sleep for half the buffer duration.
			Sleep((DWORD)(hnsActualDuration / 10000 / 2));
			// See how much buffer space is available.
			pAudioClient->GetCurrentPadding(&numFramesPadding);
			numFramesAvailable = bufferFrameCount - numFramesPadding;
			// Grab all the available space in the shared buffer.
			pRenderClient->GetBuffer(numFramesAvailable, &pData);
			// Get next 1/2-second of data from the audio source.
			buffer.loadData(pData, numFramesAvailable, flags);
			pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
			Sleep((DWORD)(hnsActualDuration / 10000 / 2));
			pAudioClient->Stop();
			if (pEnumerator != nullptr)
			{
				pEnumerator->Release();
				pEnumerator = nullptr;
			}
		}
	}
}