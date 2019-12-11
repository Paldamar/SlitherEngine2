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

	template<class worldType>
	worldType* CreateWorld(std::string worldName)
	{
		worldType* newWorld = new worldType(worldName);

		if (m_ActiveWorld == nullptr)
			m_ActiveWorld = newWorld;

		m_Worlds[worldName] = newWorld;

		return newWorld;
	}

	template<class worldType>
	worldType* CreateWorld(std::string worldName, int numberStartingScenes)
	{
		worldType* world = CreateWorld(worldName);

		if (world)
		{
			world->MakeScene("Main Scene");
			numberStartingScenes--;
			for (int i = 0; i < numberStartingScenes; i++)
			{
				world->MakeScene("Scene : " + std::to_string(i));
			}

			m_ActiveWorld = world;
		}
	}

	SlitherWorld* GetWorld(std::string worldName);
	inline SlitherWorld* GetActiveWorld() { return m_ActiveWorld; }

	SlitherWorld* ChangeActiveWorld(std::string worldName);
	SlitherWorld* ChangeActiveWorld(SlitherWorld* world);

protected:
	std::map<std::string, SlitherWorld*> m_Worlds;
	SlitherWorld* m_ActiveWorld = nullptr;
};	