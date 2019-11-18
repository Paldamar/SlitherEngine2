#pragma once

enum EventTypes
{
	EventType_Input,
	EventType_NumTypes,
};

class Event
{
protected:

public:
	Event() {}
	virtual ~Event() {}

	virtual EventTypes GetEventType() = 0;
};

