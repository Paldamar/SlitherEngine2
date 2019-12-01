#include "GamePCH.h"
#include "PhsyXPCH.h"
#include "MainGame.h"
#include "PhsyXSubSystem.h"
#include "RigidBodyComponent.h"
#include "BoxCollider.h"

MainGame::MainGame(Framework* frameWork) : GameCore(frameWork)
{

}

MainGame::~MainGame()
{
	SafeDelete(m_physicsWorld);
}

void MainGame::LoadContent()
{
	GameCore::LoadContent();
	m_physicsWorld = new PhysXWorld();
	m_physicsWorld->Init(&m_ContactCallback);

	BaseScene* mainGameScene = m_World->GetScene("Main Scene");

	BaseGameObject* DynamicObject = new BaseGameObject();
	DynamicObject->SetName("DynamicObject");
	RigidBodyComponent* rigidBody = DynamicObject->CreateComponent<RigidBodyComponent>("RigidBody");
	rigidBody->Init(m_physicsWorld, ColliderShape::BoxCollider, Vector3(0.0f, 5.0f, 0.0f), Vector3::Zero(), false);
	mainGameScene->AddObjectToScene(DynamicObject);

	BaseGameObject* StaticObject = new BaseGameObject();
	StaticObject->SetName("StaticObject");
	RigidBodyComponent* staticRigidBody = StaticObject->CreateComponent<RigidBodyComponent>("RigidBody");
	staticRigidBody->Init(m_physicsWorld, ColliderShape::BoxCollider, Vector3(0.0f, -20.0f, 0.0f), Vector3::Zero(), true);
	mainGameScene->AddObjectToScene(StaticObject);

	/* For Niall
	// Get the world Subsystem and make our world
	WorldsSubSystem* worldSystem =
		GetSubSystemManager()->CreateSubSystem<WorldsSubSystem>("WorldSystem", SubSystemID::World);

	// Give the world a name. If you wish you may pass a int as the second param on number of scenes the 'CreateWorld' will make as well.
	m_World = worldSystem->CreateWorld<SlitherWorld>("Main");
	if (m_World)
	{
		// By default, all objects you spawn with "SpawnObject" will go into the "Main Scene" (We do not need to then call 'AddObjectToScene')
		BaseGameObject* dynamicObject = m_World->SpawnObject<BaseGameObject>("DynamicObject");
		// Any code you want specific to your object here.
	}
	*/
}

void MainGame::Update(float deltaTime)
{
	m_physicsWorld->Update();
	GameCore::Update(deltaTime);

	Vector3 objectLoc = m_World->GetScene("Main Scene")->GetSceneObjectByName("DynamicObject")->GetObjectLocation();
	OutputMessage("DynamicObjectLocation x: %f, y: %f, z: %f\n", objectLoc.x, objectLoc.y, objectLoc.z);

	RigidBodyComponent* rigidBodyComp = dynamic_cast<RigidBodyComponent*>(m_World->GetScene("Main Scene")->GetSceneObjectByName("DynamicObject")->GetComponentByName("RigidBody"));
	if (rigidBodyComp)	
	{
		PxTransform transform = rigidBodyComp->GetPhysXDynamic()->getGlobalPose();
		OutputMessage("RigidBodyTransform x: %f, y: %f, z: %f\n", transform.p.x, transform.p.y, transform.p.z);
	}
}
