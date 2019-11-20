#pragma once

class SlitherWorld;
class SlitherSubSystem;

class WorldsSubSystem : public SlitherSubSystem
{
public:
	WorldsSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID = NULLSystem);
	virtual ~WorldsSubSystem();

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void CleanupWorlds();

	virtual SlitherWorld* CreateWorld(std::string worldName);

	//World* GetWorld() { return nullptr; }

protected:
	std::map<std::string, SlitherWorld*> m_Worlds;
};	