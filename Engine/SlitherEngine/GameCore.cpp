#include "SlitherEnginePCH.h"

GameCore::GameCore(Framework* frameWork) : m_Framework(frameWork)
{
	m_SubSystemManager = new SubSystemManager(frameWork);

	m_SubSystemManager->CreateSubSystem<EventHandlerSubSystem>("EventHandler", SubSystemID::EventSystem);

	m_Framework = frameWork;
	m_Framework->m_GameCore = this;
}

GameCore::~GameCore()
{
	SafeDelete(m_SubSystemManager);
	SafeDelete(m_World);
}

void GameCore::LoadContent()
{
	m_World = new SlitherWorld("MainWorld");
}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}

void GameCore::OnEvent(Event* event)
{
}

void GameCore::Update(float deltaTime)
{
	m_World->Update(deltaTime);
}

void GameCore::Draw()
{

}

SubSystemManager* GameCore::GetSubSystemManager()
{
	if (m_SubSystemManager == nullptr)
	{
		m_SubSystemManager = new SubSystemManager(m_Framework);
		return m_SubSystemManager;
	}

	return m_SubSystemManager;
}


