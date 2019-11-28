#include "PhsyXPCH.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

RigidBody::RigidBody()
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::Init(PhysXWorld* world, Vector4 position, Vector4 qRotation, bool isStatic, void* userData /*= nullptr*/)
{
	if (isStatic)
	{

	}
	else
	{

	}
}
