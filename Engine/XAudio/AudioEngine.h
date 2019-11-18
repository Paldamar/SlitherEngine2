#pragma once

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	HRESULT Init();

	IXAudio2* GetXAudio2() { return m_Xaudio2; }
	IXAudio2MasteringVoice* GetMasteringVoice() { return m_MasterVoice; }

private:
	IXAudio2* m_Xaudio2 = NULL;
	IXAudio2MasteringVoice* m_MasterVoice = NULL;
};