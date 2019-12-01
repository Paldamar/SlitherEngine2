#pragma once

enum class ColliderShape
{
	Plane,
	BoxCollider,
	//SphereCollider,
	//CapsuleCollider
};

const static std::string ColliderShapeNames[2] = { "Plane", "BoxCollider" };


class BaseComponent;

using namespace physx;
using namespace Ps;
using namespace physx::shdfnd;

class ColliderComponent : public BaseComponent
{

public:
	ColliderComponent();
	~ColliderComponent();

	PxShape* GetPhysXShape() { return m_physxShape; }
	

	virtual void Init(class PhysXWorld* world, class RigidBodyComponent* rigidBody, Vector3 halfSize, Vector3 center = Vector3(0.0f, 0.0f, 0.0f), bool isTrigger = false) = 0;

protected:
	class RigidBodyComponent* m_RigidBody;

	bool m_bEnabled = true;
	bool m_bIsTrigger = false;

	PxShape* m_physxShape = nullptr;
	
};