#pragma once

class SubSystemManager;
class Event;
class Framework;

class GameCore
{
public:
	GameCore(Framework* frameWork);
	virtual ~GameCore();

	virtual void LoadContent() {};
	virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
	virtual void OnEvent(Event* event);
	virtual void Update(float deltaTime) {};
	virtual void Draw() {};

	SubSystemManager* GetSubSystemManager();
	Framework* GetFramework() { return m_Framework; }
protected:
	Framework* m_Framework;
	friend Framework;
private:

	SubSystemManager* m_SubSystemManager = nullptr;
};


