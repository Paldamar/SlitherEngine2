#include "WorldPCH.h"

#include "../ECS/BaseGameObject.h"

BaseScene::BaseScene(std::string sceneName)
{
	m_SceneName = sceneName;
}

BaseScene::~BaseScene()
{
}

BaseGameObject* BaseScene::GetSceneObjectByName(std::string name)
{
	if (name == "")
		return nullptr;

	for (auto GameObject : m_SceneObjects)
	{
		if (name == GameObject.second->GetObjectName())
			return GameObject.second;
	}

	return nullptr;
}

std::vector<BaseGameObject*> BaseScene::GetSceneObjectsByTags(std::string tag)
{
	if (tag == "")
		return std::vector<BaseGameObject*>();

	std::vector<BaseGameObject*> foundObjects;

	for (auto GameObject : m_SceneObjects)
	{
		if (GameObject.second->ObjectHasTag(tag))
		{
			foundObjects.push_back(GameObject.second);
		}
	}

	return foundObjects;
}


void BaseScene::AddObjectToScene(BaseGameObject* object)
{
	assert(m_SceneObjects.find(object->GetObjectName()) == m_SceneObjects.end());

	m_SceneObjects.insert(std::pair<std::string, BaseGameObject*>(object->GetObjectName(), object));
	object->m_Scene = this;
}

void BaseScene::Update(float deltaTime)
{
	for (auto GameObject : m_SceneObjects)
	{
		if (GameObject.second->HasStarted() == false)
		{
			GameObject.second->Startup();
		}

		if (GameObject.second->CanTick())
			GameObject.second->Update(deltaTime);
	}
}

void BaseScene::Draw()
{
	for (auto GameObject : m_SceneObjects)
	{
		if (GameObject.second->HasStarted() == false)
		{
			continue;
		}

		GameObject.second->Draw();
	}
}

void BaseScene::CleanupKilledObjects()
{
#if DESTROY_GAMEOBJECTS_DURING_RUNTIME
	for (std::pair<std::string, GameObject*> object : m_SceneObjects)
	{
		if (object.second->IsActive() == false)
		{
			SafeDelete(object.second);
			m_SceneObjects.erase(object.first);
		}
	}
#else
	for (std::pair<std::string, BaseGameObject*> object : m_SceneObjects)
	{
		if (object.second->IsActive() == false)
		{
			m_KilledOjbects[object.first] = object.second;
			m_SceneObjects.erase(object.first);
		}
	}
#endif
}

void BaseScene::Startup()
{
	for (auto GameObject : m_SceneObjects)
	{
		GameObject.second->Startup();
	}
}
