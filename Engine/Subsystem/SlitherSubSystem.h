#pragma once

class Framework;

enum SubSystemID
{
	NULLSystem,
	EventSystem,
	DXSystem,
	XAudio,
	World,
	PhysX,
	Timers,
};

class SlitherSubSystem
{
public:
	SlitherSubSystem(std::string systemName, Framework* engineInstance,SubSystemID instanceID = NULLSystem);
	virtual ~SlitherSubSystem();

	virtual void Init() {};

	virtual void Update(float deltaTime) {};
	virtual void Draw() {};

	SubSystemID GetSystemID() const { return m_InstanceID; }

	inline bool operator ==(const SlitherSubSystem& o) const { return this->m_InstanceID == o.m_InstanceID; }
	inline bool operator !=(const SlitherSubSystem& o) const { return this->m_InstanceID != o.m_InstanceID; }
private:
	SubSystemID m_InstanceID;
	std::string m_SystemName;
	Framework* m_EngineInstance;
};

