#include "SubsystemPCH.h"

#include "../SlitherEngine/Framework.h"
//#include "SlitherSubSystem.h"

EventHandlerSubSystem::EventHandlerSubSystem(std::string systemName, Framework* engineInstance,SubSystemID instanceID)
	: SlitherSubSystem(systemName, engineInstance, instanceID)
{
}

EventHandlerSubSystem::~EventHandlerSubSystem()
{
	while (m_Events.empty() == false)
	{
		delete m_Events.front();
		m_Events.front() = NULL;
		m_Events.pop();
	}
}

void EventHandlerSubSystem::QueueEvent(Event* event)
{
	assert(event != 0);

	m_Events.push(event);
}

void EventHandlerSubSystem::DispatchEvents(GameCore* gameCore)
{
	while (m_Events.empty() == false)
	{
		Event* event = m_Events.front();

		gameCore->OnEvent(event);
		m_Events.pop();

		SafeDelete(event);
	}
}
