#include "WorldPCH.h"

SlitherWorld::SlitherWorld()
{
	m_SceneMap.insert(std::pair<std::string, BaseScene*>("Main Scene", new BaseScene("Main Scene")));
}

SlitherWorld::~SlitherWorld()
{
	for (auto Scene : m_SceneMap)
	{
		delete (Scene.second);
		Scene.second = nullptr;
	}
	m_SceneMap.clear();
}

BaseScene* SlitherWorld::GetScene(std::string sceneName)
{
	for (auto Scene : m_SceneMap)
	{
		if (Scene.second->GetSceneName() == sceneName)
			return Scene.second;
	}

	return nullptr;
}

bool SlitherWorld::DoesSceneExist(std::string sceneName)
{
	for (auto Scene : m_SceneMap)
	{
		if (Scene.second->GetSceneName() == sceneName)
			return true;
	}

	return false;
}

void SlitherWorld::Update(float deltaTime)
{
	for (auto Scene : m_SceneMap)
	{
		Scene.second->Update(deltaTime);
	}
}

void SlitherWorld::Draw()
{
	for (auto Scene : m_SceneMap)
	{
		Scene.second->Draw();
	}
}

void SlitherWorld::CleanupWorld()
{
	for (std::pair<std::string, BaseScene*> scene : m_SceneMap)
	{
		scene.second->CleanupKilledObjects();
	}
}
