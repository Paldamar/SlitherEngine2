#include "SubsystemPCH.h"

SubSystemManager::SubSystemManager()
	//: m_EngineInstance(engineInstance)
{
}

SubSystemManager::~SubSystemManager()
{
	for (auto iteratorSystem : m_EngineSubSystems)
	{
		SafeDelete(iteratorSystem.second);
	}
	m_EngineSubSystems.clear();	
}

SlitherSubSystem* SubSystemManager::GetSubSystemByName(std::string systemName)
{
	auto it = m_EngineSubSystems.find(systemName);
	if (it == m_EngineSubSystems.end())
		return nullptr;

	return it->second;
}

SlitherSubSystem* SubSystemManager::GetSubSystemByType(SubSystemID ID)
{
	std::map<std::string, SlitherSubSystem*>::iterator it;
	for (it = m_EngineSubSystems.begin(); it != m_EngineSubSystems.end(); it++)
	{
		if (it->second->GetSystemID() == ID)
			return it->second;
	}

	return NULL;
}

void SubSystemManager::Update(float deltaTime)
{
	for (auto iteratorSubsyste : m_EngineSubSystems)
	{
		iteratorSubsyste.second->Update(deltaTime);
	}
}

void SubSystemManager::Draw()
{
	for (auto iteratorSubsyste : m_EngineSubSystems)
	{
		iteratorSubsyste.second->Draw();
	}
}

bool SubSystemManager::AddSystemToEngine(SlitherSubSystem* system, std::string systemName)
{
#if RELEASE
	assert(m_EngineSubSystems.find(systemName) == m_EngineSubSystems.end());
#endif
	if (m_EngineSubSystems.find(systemName) != m_EngineSubSystems.end())
		return false;

	std::map<std::string, SlitherSubSystem*>::iterator it;
	for (it = m_EngineSubSystems.begin(); it != m_EngineSubSystems.end(); it++)
	{
		if (system->GetSystemID() == it->second->GetSystemID())
			return false;
	}

	m_EngineSubSystems[systemName] = system;

	return true;
}
