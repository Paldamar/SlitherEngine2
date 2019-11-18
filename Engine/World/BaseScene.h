#pragma once
class BaseObject;

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
protected:
	virtual void Startup();
	friend BaseScene;
protected:
	std::map<std::string, BaseObject*> m_SceneObjects;
	std::string m_SceneName;	
	bool m_AffectedByPhysics;
};

