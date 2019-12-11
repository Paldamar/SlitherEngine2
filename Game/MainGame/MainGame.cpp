#include "GamePCH.h"
#include "PhsyXPCH.h"
#include "MainGame.h"
#include "PhsyXSubSystem.h"
#include "PhysXWorld.h"
#include "RigidBodyComponent.h"
#include "BoxCollider.h"

MainGame::MainGame(Framework* frameWork) : GameCore(frameWork)
{

}

MainGame::~MainGame()
{
}

void MainGame::LoadContent()
{
	GameCore::LoadContent();
	//m_physicsWorld = new PhysXWorld();
	//m_physicsWorld->Init(&m_ContactCallback);

	PhsyXSubSystem* physXSub = static_cast<PhsyXSubSystem*>(GetSubSystemManager()->GetSubSystemByType(SubSystemID::PhysX));
	PhysXWorld* physXWorld = nullptr;
	if (physXSub)
	{
		physXWorld = physXSub->CreateWorld("Test");
		physXWorld->Init();
	}

	/*BaseScene* mainGameScene = m_World->GetScene("Main Scene");

	BaseGameObject* DynamicObject = new BaseGameObject();
	
	mainGameScene->AddObjectToScene(DynamicObject);

	BaseGameObject* StaticObject = new BaseGameObject();
	StaticObject->SetName("StaticObject");
	RigidBodyComponent* staticRigidBody = StaticObject->CreateComponent<RigidBodyComponent>("RigidBody");
	staticRigidBody->Init(m_physicsWorld, ColliderShape::BoxCollider, Vector3(0.0f, -20.0f, 0.0f), Vector3::Zero(), true);
	mainGameScene->AddObjectToScene(StaticObject);*/

	// Get the world Subsystem and make our world
	WorldsSubSystem* worldSystem =	static_cast<WorldsSubSystem*>(GetSubSystemManager()->GetSubSystemByType(SubSystemID::World));

	// Give the world a name. If you wish you may pass a int as the second param on number of scenes the 'CreateWorld' will make as well.
	m_World = worldSystem->CreateWorld<SlitherWorld>("Main");
	if (m_World)
	{
		// By default, all objects you spawn with "SpawnObject" will go into the "Main Scene" (We do not need to then call 'AddObjectToScene')
		BaseGameObject* dynamicObject = m_World->SpawnObject<BaseGameObject>("DynamicObject");
		RigidBodyComponent* rigidBody = dynamicObject->CreateComponent<RigidBodyComponent>("RigidBody");
		rigidBody->Init(physXWorld, ColliderShape::BoxCollider, Vector3(0.0f, 5.0f, 0.0f), Vector3::Zero(), false, Vector3(10.0f, 10.0f, 10.0f));

		BaseGameObject* staticObject = m_World->SpawnObject<BaseGameObject>("StaticObject");
		RigidBodyComponent* staticRigidBody = staticObject->CreateComponent<RigidBodyComponent>("RigidBody");
		staticRigidBody->Init(physXWorld, ColliderShape::BoxCollider, Vector3(0.0f, -20.0f, 0.0f), Vector3::Zero(), true, Vector3(10.0f, 10.0f, 10.0f));
	}
	
}

void MainGame::Update(float deltaTime)
{
	GameCore::Update(deltaTime);

	BaseGameObject* dynamicObjec = m_World->GetScene("Main Scene")->GetSceneObjectByName("DynamicObject");
	Vector3 objectLoc = dynamicObjec->GetObjectLocation();
	OutputMessage("DynamicObjectLocation x: %f, y: %f, z: %f\n", objectLoc.x, objectLoc.y, objectLoc.z);
	Vector3 objectRot = dynamicObjec->GetObjectRotation();
	OutputMessage("DynamicObjectRotation x: %f, y: %f, z: %f\n", objectRot.x, objectRot.y, objectRot.z);

	RigidBodyComponent* rigidBodyComp = dynamic_cast<RigidBodyComponent*>(m_World->GetScene("Main Scene")->GetSceneObjectByName("DynamicObject")->GetComponentByName("RigidBody"));
	if (rigidBodyComp)	
	{
		PxTransform transform = rigidBodyComp->GetPhysXDynamic()->getGlobalPose();
		OutputMessage("DynamicObjectRigidBodyTransform\nPostion: x: %f, y: %f, z: %f\nRotation: x: %f, y: %f, z: %f, w: %f", transform.p.x, transform.p.y, transform.p.z, transform.q.x, transform.q.y, transform.q.z, transform.q.w);
	}
}
