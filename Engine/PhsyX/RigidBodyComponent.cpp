#include "PhsyXPCH.h"
#include "RigidBodyComponent.h"
#include "BoxCollider.h"
#include "PhsyXSubSystem.h"
#include "PhysXWorld.h"

RigidBodyComponent::RigidBodyComponent()
{

}

RigidBodyComponent::~RigidBodyComponent()
{

}

void RigidBodyComponent::Init(PhysXWorld* world, ColliderShape shapeType, Vector3 position, Vector3 eulerRotation, bool isStatic, Vector3 shapeDetails)
{
	ColliderComponent* newCollider = nullptr;
	switch (shapeType)
	{
		case ColliderShape::Plane:
		{
			//stuff
			break;
		}

		case ColliderShape::BoxCollider:
		{
			newCollider = CreateComponent<BoxColliderComponent>(ColliderShapeNames[(int)shapeType], GetOwner());
			newCollider->Init(world, this, shapeDetails, Vector3::Zero());
			break;
		}
	}
	PxShape* shape = nullptr;
	if (isStatic)
	{
		m_PhysxRigidStatic = world->GetPhysics()->createRigidStatic(PxTransform(PhsyXSubSystem::PxVec3FromVec3(position), PhsyXSubSystem::PxQuatFromEuler(eulerRotation)));
		shape = newCollider->GetPhysXShape();
		m_PhysxRigidStatic->attachShape(*shape);
	}
	else
	{
		m_PhysxRigidDynamic = world->GetPhysics()->createRigidDynamic(PxTransform(PhsyXSubSystem::PxVec3FromVec3(position), PhsyXSubSystem::PxQuatFromEuler(eulerRotation)));
		shape = newCollider->GetPhysXShape();
		m_PhysxRigidDynamic->attachShape(*newCollider->GetPhysXShape());
	}
	if (shape)
	{
		shape->release();
	}

	//setup Registering RigidBody
	if (m_PhysxRigidDynamic)
	{
		//CALL PxRigidBodyExt::updateMassAndInertia() passing in (*m_physxRigidDynamic, 10.0f)
		PxRigidBodyExt::updateMassAndInertia(*m_PhysxRigidDynamic, 10.0f);
		//CALL setActorFlag() on m_physxRigidDynamic passing in (PxActorFlag::eDISABLE_GRAVITY, !m_useGravity)
		m_PhysxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
		//CALL PhysicsWorld::GetInstance()->GetScene()->addActor() passing in (*m_physxRigidDynamic)
		world->GetScene()->addActor(*m_PhysxRigidDynamic);
	}
	//IF m_physxRigidStatic
	if (m_PhysxRigidStatic)
	{
		//CALL setActorFlag() on m_physxRigidStatic passing in (PxActorFlag::eDISABLE_GRAVITY, !m_useGravity)
		m_PhysxRigidStatic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		//CALL PhysicsWorld::GetInstance()->GetScene()->addActor() passing in *m_physxRigidStatic
		world->GetScene()->addActor(*m_PhysxRigidStatic);
	}
}

void RigidBodyComponent::Draw()
{
}

void RigidBodyComponent::Update(float deltaTime)
{
	if (m_PhysxRigidDynamic)
	{
		PxTransform transform = m_PhysxRigidDynamic->getGlobalPose();
		GetOwner()->SetTransform(PhsyXSubSystem::TransformFromPxTransform(transform));
	}
	else if (m_PhysxRigidStatic)
	{
		PxTransform transform = m_PhysxRigidStatic->getGlobalPose();
		GetOwner()->SetTransform(PhsyXSubSystem::TransformFromPxTransform(transform));
	}
}
