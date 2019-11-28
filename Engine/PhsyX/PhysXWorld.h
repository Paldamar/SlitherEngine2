#pragma once


#define	SAFE_RELEASE(x)	if(x){ x->release(); x = NULL;	}

using namespace physx;
using namespace Ps;
using namespace physx::shdfnd;

enum class CollisionShape
{
	Plane,
	BoxCollider,
	SphereCollider,
	CapsuleCollider
};

//std::vector<PxVec3>

PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return PxFilterFlag::eDEFAULT;
}

class ContactReportCallback : public PxSimulationEventCallback
{
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override { PX_UNUSED(constraints); PX_UNUSED(count); }
	virtual void onWake(PxActor** actors, PxU32 count) override { PX_UNUSED(actors); PX_UNUSED(count); }
	virtual void onSleep(PxActor** actors, PxU32 count) override { PX_UNUSED(actors); PX_UNUSED(count); }
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override { PX_UNUSED(pairs); PX_UNUSED(count); }
	virtual void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {}
};

class PhysXWorld
{
public:
	PhysXWorld();
	~PhysXWorld();

	PxPhysics* GetPhysics() { return m_Physics; }
	PxScene* GetScene() { return m_Scene; }
	PxMaterial* GetMaterial() { return m_Material; }

	virtual bool Init(ContactReportCallback* contactReport);
	virtual void Update();

	//create RigidBody

	PxU32 getNbPhysicalCores()
	{
		return Ps::Thread::getNbPhysicalCores();
	}

protected:
	std::vector<PxVec3> m_ContactPositions;
	std::vector<PxVec3> m_ContactImpulses;
	std::vector<PxVec3> m_ContactSphereActorPositions;

private:

	PxFoundation* m_Foundation = nullptr;
	PxPhysics* m_Physics = nullptr;

	PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
	PxScene* m_Scene = nullptr;
	PxMaterial* m_Material = nullptr;
	//PxPvd* m_Pvd = nullptr; Apparently not used

	//PxRigidStatic* CreatePlane(); Not used either looks like
};