#include "XAudioPCH.h"

AudioObject::AudioObject(TCHAR* filename, AudioEngine* audiomanager)
{
	m_AudioEngine = audiomanager;

	if (m_AudioEngine == nullptr)
	{
		assert(false);
	}

	WAVEFORMATEXTENSIBLE* twfx = &m_wfx;
	XAUDIO2_BUFFER* tbuffer = &m_Buffer;

	HRESULT successA = CreateAudioFile(filename);
	assert(SUCCEEDED(successA));

	HRESULT successB = CreateSourceVoice();
	assert(SUCCEEDED(successB));
}

AudioObject::~AudioObject()
{
	if (m_Buffer.pAudioData != nullptr)
	{
		delete[] m_Buffer.pAudioData;
	}
}

HRESULT AudioObject::Play()
{
	HRESULT hr = S_OK;

	if (FAILED(hr = m_Sourcevoice->Start(0)))
	{
		MessageBox(0, L"AudioObject : Failed to play sound.", 0, 0);
		return hr;
	}

	return hr;
}

void AudioObject::SetDoesLoop(bool loop)
{
}

bool AudioObject::GetDoesLoop()
{
	return m_Buffer.LoopCount == XAUDIO2_LOOP_INFINITE;
}

unsigned int AudioObject::GetDurationMS()
{
	return 0;
}

short AudioObject::GetDuration()
{
	return 0;
}

HRESULT AudioObject::CreateAudioFile(TCHAR* filename)
{
	// Open the file
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;

	// check the file type, should be fourccWave or 'XWMA'
	FindChunkInRIFF_File(hFile, *(DWORD*)fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	// locate the 'fmt' chunk, and copy its contents into the WAVEFORMATEXENSIBLE structure
	FindChunkInRIFF_File(hFile, *(DWORD*)fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &m_wfx, dwChunkSize, dwChunkPosition);

	// locate the 'data' chunk, and read its contents into a buffer
	FindChunkInRIFF_File(hFile, *(DWORD*)fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* DataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, DataBuffer, dwChunkSize, dwChunkPosition);

	m_Buffer.AudioBytes = dwChunkSize; // buffer containing audio data
	m_Buffer.pAudioData = DataBuffer;  // size of the audio buffer in bytes
	m_Buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	return S_OK;
}

HRESULT AudioObject::CreateSourceVoice()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = m_AudioEngine->GetXAudio2()->CreateSourceVoice(&m_Sourcevoice, (WAVEFORMATEX*)&m_wfx)))
		return hr;

	if (FAILED(hr = m_Sourcevoice->SubmitSourceBuffer(&m_Buffer)))
		return hr;

	return hr;
}

HRESULT AudioObject::FindChunkInRIFF_File(HANDLE hFile, DWORD& fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;

		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT AudioObject::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
