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
	m_ActiveWorld->Update(deltaTime);
}

void WorldsSubSystem::Draw()
{
	m_ActiveWorld->Draw();
}

void WorldsSubSystem::CleanupWorlds()
{
	for (auto world : m_Worlds)
	{
		world.second->CleanupWorld();
	}
}

SlitherWorld* WorldsSubSystem::GetWorld(std::string worldName)
{
	auto it = m_Worlds.find(worldName);
	if (it == m_Worlds.end())
		return nullptr;

	return it->second;
}

SlitherWorld* WorldsSubSystem::ChangeActiveWorld(std::string worldName)
{
	auto it = m_Worlds.find(worldName);
	if (it == m_Worlds.end())
		return nullptr;

	m_ActiveWorld = it->second;
	return it->second;
}

SlitherWorld* WorldsSubSystem::ChangeActiveWorld(SlitherWorld* world)
{
	auto it = m_Worlds.find(world->GetMapName());
	if (it == m_Worlds.end())
	{
		m_Worlds[world->GetMapName()] = world;
	}

	m_ActiveWorld = world;
	return it->second;
}
