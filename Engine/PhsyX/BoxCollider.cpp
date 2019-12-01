#include "PhsyXPCH.h"
#include "BoxCollider.h"
#include "PhysXWorld.h"

void BoxColliderComponent::Init(PhysXWorld* world, RigidBodyComponent* rigidBody, Vector3 halfSize, Vector3 center, bool isTrigger /*= false*/)
{
	m_RigidBody = rigidBody;
	m_physxShape = world->GetPhysics()->createShape(PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z), *world->GetMaterial());
	m_physxShape->setLocalPose(PxTransform(center.x, center.y, center.z));
	m_physxShape->userData = m_Owner;
}
