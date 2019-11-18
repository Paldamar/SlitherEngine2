#pragma once

class AudioEngine;

class AudioObject
{
public:
	AudioObject(TCHAR*, AudioEngine*);
	~AudioObject();

	HRESULT Play();

	void SetDoesLoop(bool loop = true);
	bool GetDoesLoop();

	unsigned int GetDurationMS();
	short GetDuration();
private:
	IXAudio2SourceVoice* m_Sourcevoice;

	WAVEFORMATEXTENSIBLE m_wfx = { 0 };
	XAUDIO2_BUFFER m_Buffer = { 0 };

	AudioEngine* m_AudioEngine;

	HRESULT CreateAudioFile(TCHAR*);
	HRESULT CreateSourceVoice();
	HRESULT FindChunkInRIFF_File(HANDLE hfile, DWORD& fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
};

