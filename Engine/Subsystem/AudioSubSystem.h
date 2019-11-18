#pragma once

class SlitherSubSystem;
class AudioEngine;

class AudioSubSystem : public SlitherSubSystem
{
public:
	AudioSubSystem(std::string systemName, SubSystemID instanceID = NULLSystem);
	~AudioSubSystem();

	virtual void Init();

	virtual void Update(float deltaTime);

	AudioEngine* GetAudioEngine() { return m_AudioEngine; }
protected:
	AudioEngine* m_AudioEngine = NULL;
};

