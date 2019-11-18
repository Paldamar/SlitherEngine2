#include "SlitherEnginePCH.h"

GameCore::GameCore(Framework* frameWork) : m_Framework(frameWork)
{
	m_SubSystemManager = new SubSystemManager(/*this*/);

	m_SubSystemManager->CreateSubSystem<EventHandlerSubSystem>("EventHandler", SubSystemID::EventSystem);

	m_Framework = frameWork;
	m_Framework->m_GameCore = this;
}

GameCore::~GameCore()
{
	SafeDelete(m_SubSystemManager);
}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}

void GameCore::OnEvent(Event* event)
{
}

SubSystemManager* GameCore::GetSubSystemManager()
{
	if (m_SubSystemManager == nullptr)
	{
		m_SubSystemManager = new SubSystemManager(/*this*/);
		return m_SubSystemManager;
	}

	return m_SubSystemManager;
}


