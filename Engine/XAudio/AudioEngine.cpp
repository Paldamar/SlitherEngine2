#include "XAudioPCH.h"


AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
	if (m_MasterVoice != NULL)
		m_MasterVoice->DestroyVoice();

	if (m_Xaudio2 != NULL)
	{
		m_Xaudio2->StopEngine();
		m_Xaudio2->Release();
	}
		
}

HRESULT AudioEngine::Init()
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&m_Xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		MessageBox(0, L"XAudio : Failed to make XAudio2 engine", 0, 0);
		return hr;
	}		
		
	m_Xaudio2->StartEngine();
	
	if (FAILED(hr = m_Xaudio2->CreateMasteringVoice(&m_MasterVoice)))
	{
		MessageBox(0, L"XAudio : Failed to make MasteringVoice", 0, 0);
		return hr;
	}
		

	return S_OK;
}
