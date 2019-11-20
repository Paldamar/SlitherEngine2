#pragma once
class SlitherSubSystem;
class Framework;

class SubSystemManager
{
public:
	SubSystemManager(Framework* engineInstance);
	~SubSystemManager();

	SlitherSubSystem* GetSubSystemByName(std::string systemName);
	SlitherSubSystem* GetSubSystemByType(SubSystemID ID);

	template<class subsystem>
	inline subsystem* CreateSubSystem(std::string systemName, SubSystemID instanceID);


	void Update(float deltaTime);
	void Draw();

private:
	std::map<std::string, SlitherSubSystem*> m_EngineSubSystems;
	Framework* m_EngineInstance;
	bool AddSystemToEngine(SlitherSubSystem* system, std::string systemName);
};

template<class subsystem>
inline subsystem* SubSystemManager::CreateSubSystem(std::string systemName, SubSystemID instanceID)
{
	// Check if we have a subsystem already by the given name.
	if (m_EngineSubSystems.find(systemName) != m_EngineSubSystems.end())
		return NULL;

	// Check to see if we have that type of subsystem already.
	std::map<std::string, SlitherSubSystem*>::iterator iterator_of_subsystems;
	for (iterator_of_subsystems = m_EngineSubSystems.begin(); iterator_of_subsystems != m_EngineSubSystems.end(); iterator_of_subsystems++)
	{
		if (instanceID == iterator_of_subsystems->second->GetSystemID())
			return NULL;
	}
	
	subsystem* system = new subsystem(systemName, m_EngineInstance, instanceID);
	m_EngineSubSystems[systemName] = system;
	return system;
}