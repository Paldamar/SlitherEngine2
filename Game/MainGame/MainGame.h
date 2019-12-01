#pragma once
#include "PhysXWorld.h"

class MainGameContactReportCallback : public ContactReportCallback
{
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override { PX_UNUSED(constraints); PX_UNUSED(count); }
	virtual void onWake(PxActor** actors, PxU32 count) override { PX_UNUSED(actors); PX_UNUSED(count); }
	virtual void onSleep(PxActor** actors, PxU32 count) override { PX_UNUSED(actors); PX_UNUSED(count); }
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override { PX_UNUSED(pairs); PX_UNUSED(count); }
	virtual void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {}
};

class MainGame : public GameCore
{
public:
	MainGame(Framework* frameWork);
	~MainGame();

	void LoadContent() override;

	virtual void Update(float deltaTime) override;

protected:
	PhysXWorld* m_physicsWorld = nullptr;
	MainGameContactReportCallback m_ContactCallback;
};