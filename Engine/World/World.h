#pragma once


class World
{
public:
	World();
	~World();

	BaseScene* GetScene(std::string sceneName);
	bool DoesSceneExist(std::string sceneName);

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
		Object* spawnedObject = new Object();

		spawnedObject->SetName(ObjectName);

		m_SceneMap["Main Scene"]->AddObjectToScene(spawnedObject);

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

		Object* spawnedObject = new Object();

		spawnedObject->SetName(ObjectName);

		m_SceneMap[sceneName]->AddObjectToScene(spawnedObject);

		return spawnedObject;
	}

	template<class Object>
	Object* SpawnObject(std::string ObjectName, std::string sceneName, Transform3D spawnTransform, bool createSceneIfFailed = false)
	{
		if (m_SceneMap.find(sceneName) == m_SceneMap.end() && createSceneIfFailed == true)
		{
			m_SceneMap.insert(std::pair<std::string, BaseScene*>(sceneName, new BaseScene(sceneName)));
		}
		else
		{
			return NULL;
		}

		Object* spawnedObject = new Object();

		spawnedObject->SetName(ObjectName);

		m_SceneMap[sceneName]->AddObjectToScene(spawnedObject);

		spawnedObject->SetTransform(spawnTransform);

		return spawnedObject;
	}

protected:
	std::map<std::string, BaseScene*> m_SceneMap;
};

