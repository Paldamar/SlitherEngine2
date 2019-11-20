#include "SubsystemPCH.h"


WorldsSubSystem::WorldsSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID)
	: SlitherSubSystem(systemName, engineInstance, instanceID)
{
	
}

WorldsSubSystem::~WorldsSubSystem()
{
	for (auto world : m_Worlds)
	{
		SafeDelete(world.second);
		m_Worlds.erase(world.first);
	}
	m_Worlds.clear();
}

void WorldsSubSystem::Update(float deltaTime)
{
	for (auto world : m_Worlds)
	{
		world.second->Update(deltaTime);
	}
}

void WorldsSubSystem::Draw()
{
	for (auto world : m_Worlds)
	{
		world.second->Draw();
	}
}

void WorldsSubSystem::CleanupWorlds()
{
	for (auto world : m_Worlds)
	{
		world.second->CleanupWorld();
	}
}

SlitherWorld* WorldsSubSystem::CreateWorld(std::string worldName)
{
	return m_Worlds[worldName] = new SlitherWorld();
}