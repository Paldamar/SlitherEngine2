#pragma once

#include "../Math/Transform3D.h"
class BaseScene;

class SlitherWorld
{
public:
	SlitherWorld(std::string worldName);
	~SlitherWorld();

	BaseScene* GetScene(std::string sceneName);
	bool DoesSceneExist(std::string sceneName);

#pragma region SpawnObject
	template<class Object>
	Object* SpawnObject(std::string ObjectName)
	{
		Object* spawnedObject = new Object();
		
		spawnedObject->SetName(ObjectName);

		m_SceneMap["Main Scene"]->AddObjectToScene(spawnedObject);
		
		return spawnedObject;
	}

	template<class Object>
	Object* SpawnObject(std::string ObjectName, Transform3D spawnTransform)
	{
		Object* spawnedObject = SpawnObject<Object>(ObjectName, spawnTransform);

		spawnedObject->SetTransform(spawnTransform);

		return spawnedObject;
	}

	template<class Object>
	Object* SpawnObject(std::string ObjectName, std::string sceneName, bool createSceneIfFailed = false)
	{
		if (m_SceneMap.find(sceneName) == m_SceneMap.end() && createSceneIfFailed == true)
		{
			m_SceneMap.insert(std::pair<std::string, BaseScene*>(sceneName, new BaseScene(sceneName)));
		}
		else
		{
			return NULL;
		}

		Object* spawnedObject = SpawnObject<Object>(ObjectName);

		m_SceneMap[sceneName]->AddObjectToScene(spawnedObject);

		return spawnedObject;
	}

	template<class Object>
	Object* SpawnObject(std::string ObjectName, std::string sceneName, Transform3D spawnTransform, bool createSceneIfFailed = false)
	{
		Object* spawnedObject = SpawnObject<Object>(ObjectName,sceneName,createSceneIfFailed)

		spawnedObject->SetTransform(spawnTransform);

		return spawnedObject;
	}
#pragma endregion SpawnObject

#pragma region MakeScene
	template<class sceneClass>
	sceneClass* MakeScene(std::string sceneName)
	{
		if (DoesSceneExist(sceneName))
			return NULL;

		m_SceneMap[sceneName] = new sceneClass(sceneName);
		m_SceneMap[sceneName]->m_World = this;
	}

	template<class sceneClass>
	sceneClass* MakeScene(std::string sceneName, bool affectedByPhysics)
	{
		sceneClass* scene = MakeScene<sceneClass>(sceneName)
		if(scene);
		{
			scene->SetAffectedByPhysics(affectedByPhysics);
		}
	}
#pragma endregion MakeScene

	void Update(float deltaTime);
	void Draw();

	void CleanupWorld();
	inline std::string GetMapName() { return m_MapName; }
protected:
	std::map<std::string, BaseScene*> m_SceneMap;
	std::string m_MapName;
};

