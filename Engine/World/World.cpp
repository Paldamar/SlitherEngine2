#include "WorldPCH.h"

World::World()
{
	m_SceneMap.insert(std::pair<std::string, BaseScene*>("Main Scene", new BaseScene("Main Scene")));
}

World::~World()
{
	for (auto Scene : m_SceneMap)
	{
		delete (Scene.second);
		Scene.second = nullptr;
	}
	m_SceneMap.clear();
}

BaseScene* World::GetScene(std::string sceneName)
{
	for (auto Scene : m_SceneMap)
	{
		if (Scene.second->GetSceneName() == sceneName)
			return Scene.second;
	}

	return nullptr;
}

bool World::DoesSceneExist(std::string sceneName)
{
	for (auto Scene : m_SceneMap)
	{
		if (Scene.second->GetSceneName() == sceneName)
			return true;
	}

	return false;
}
