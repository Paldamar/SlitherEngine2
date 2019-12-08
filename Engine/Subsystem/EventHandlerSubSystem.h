#pragma once
#include "SlitherSubSystem.h"
#include <queue>

class Event;
class GameCore;

class EventHandlerSubSystem : public SlitherSubSystem
{
public:
	EventHandlerSubSystem(std::string systemName, Framework* engineInstance,SubSystemID instanceID = NULLSystem);
	~EventHandlerSubSystem();

	void QueueEvent(Event* event);
	void DispatchEvents(GameCore* gameCore);

protected:
	std::queue<Event*> m_Events;
};

