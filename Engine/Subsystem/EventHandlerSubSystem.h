#pragma once

class SlitherSubSystem;
class Event;
class GameCore;

class EventHandlerSubSystem : public SlitherSubSystem
{
public:
	EventHandlerSubSystem(std::string systemName, SubSystemID instanceID = NULLSystem);
	~EventHandlerSubSystem();

	void QueueEvent(Event* event);
	void DispatchEvents(GameCore* gameCore);

protected:
	std::queue<Event*> m_Events;
};

