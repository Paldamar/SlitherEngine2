#pragma once

class PhysXWorld;

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	void Init(PhysXWorld* world, Vector4 position, Vector4 qRotation, bool isStatic, void* userData = nullptr);

protected:

private:
	float m_Mass;
	float m_Drag;
	float m_AngularDrag;
	bool m_bUseGravity;
	bool m_bIsKinematic;

	Vector3 m_Velocity;
	MyMatrix m_Transform;

	physx::PxRigidDynamic* m_PhysxRigidDynamic;
	physx::PxRigidStatic* m_PhysxRigidStatic;
};