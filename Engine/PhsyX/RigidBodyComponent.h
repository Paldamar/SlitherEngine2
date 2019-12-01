#pragma once

class PhysXWorld;
enum class ColliderShape;

class RigidBodyComponent : public BaseComponent
{
public:
	RigidBodyComponent();
	~RigidBodyComponent();

	void Init(PhysXWorld* world, ColliderShape shapeType, Vector3 position, Vector3 qRotation, bool isStatic);

	virtual void Draw() override;

	virtual void Update(float deltaTime) override;

	physx::PxRigidDynamic* GetPhysXDynamic() { return m_PhysxRigidDynamic; }
	physx::PxRigidStatic* GetPhysXStatic() { return m_PhysxRigidStatic; }

protected:

private:
	float m_Mass;
	float m_Drag;
	float m_AngularDrag;
	bool m_bUseGravity;
	bool m_bIsKinematic;

	Vector3 m_Velocity;
	MyMatrix m_Transform;

	physx::PxRigidDynamic* m_PhysxRigidDynamic = nullptr;
	physx::PxRigidStatic* m_PhysxRigidStatic = nullptr;
};