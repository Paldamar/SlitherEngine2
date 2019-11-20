#include "SubsystemPCH.h"

#include "../SlitherEngine/Framework.h"

AudioSubSystem::AudioSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID)
	: SlitherSubSystem(systemName, engineInstance, instanceID)
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
