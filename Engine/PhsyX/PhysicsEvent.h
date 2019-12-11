#pragma once
#include "Event.h"

enum class CollisionType
{
	Collision_Start,
	Collision_End,
};

class PhysicsEvent : public Event
{
public:
	PhysicsEvent(BaseObject* object1, BaseObject* object2, Vector3 contactLocation, Vector3 contactImpulse, CollisionType collisionType)
	{
		m_Obj1 = object1;
		m_Obj2 = object2;
		m_Location = contactLocation;
		m_Impluse = contactImpulse;
		m_Type = collisionType;
	}

	virtual EventTypes GetEventType() override { return EventType_Physics; }

	BaseObject* GetObject1() { return m_Obj1; }
	BaseObject* GetObject2() { return m_Obj2; }
	Vector3 GetLocation() { return m_Location; }
	Vector3 GetImpluse() { return m_Impluse; }
	CollisionType GetCollisionType() { return m_Type; }

protected:
	BaseObject* m_Obj1 = nullptr;
	BaseObject* m_Obj2 = nullptr;
	Vector3 m_Location;
	Vector3 m_Impluse;
	CollisionType m_Type;
};