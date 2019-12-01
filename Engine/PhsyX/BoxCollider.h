#pragma once
#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{

public:
	virtual void Init(PhysXWorld* world, RigidBodyComponent* rigidBody, Vector3 halfSize, Vector3 center = Vector3(0.0f, 0.0f, 0.0f), bool isTrigger = false) override;

};