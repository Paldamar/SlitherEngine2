#include "SubsystemPCH.h"

AudioSubSystem::AudioSubSystem(std::string systemName, SubSystemID instanceID)
	: SlitherSubSystem(systemName, instanceID)
{
}

AudioSubSystem::~AudioSubSystem()
{
}

void AudioSubSystem::Init()
{
	if (m_AudioEngine == NULL)
		m_AudioEngine->Init();
}

void AudioSubSystem::Update(float deltaTime)
{
}
