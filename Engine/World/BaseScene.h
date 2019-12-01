#pragma once

class BaseObject;
class SlitherWorld;

class BaseScene
{
public:
	BaseScene(std::string sceneName);
	~BaseScene();

	virtual BaseObject* GetSceneObjectByName(std::string name);
	virtual std::vector<BaseObject*> GetSceneObjectsByTags(std::string tag);
	inline virtual std::string GetSceneName() { return m_SceneName; }

	virtual void AddObjectToScene(BaseObject* object);

	virtual void Update(float deltaTime);
	virtual void Draw();

	bool IsAffectedByPhysics() { return true; }
	void SetAffectedByPhysics(bool toggle) { m_AffectedByPhysics = toggle; }

	virtual void CleanupKilledObjects();

	SlitherWorld* GetWorld() { return m_World; }
protected:
	virtual void Startup();

protected:
	std::map<std::string, BaseObject*> m_SceneObjects;
#if !DESTROY_GAMEOBJECTS_DURING_RUNTIME
	std::map<std::string, BaseObject*> m_KilledOjbects;
#endif
	std::string m_SceneName;	
	bool m_AffectedByPhysics = false;

	// World that this scene is in.
	SlitherWorld* m_World;

	friend class SlitherWorld;
};

