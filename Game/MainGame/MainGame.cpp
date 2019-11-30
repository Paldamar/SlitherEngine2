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

	BaseGameObject* staticObject = new BaseGameObject();
	staticObject->SetName("DynamicObject");
	RigidBodyComponent* rigidBody = staticObject->CreateComponent<RigidBodyComponent>("RigidBody");
	rigidBody->Init(m_physicsWorld, ColliderShape::BoxCollider, Vector3(0.0f, 5.0f, 0.0f), Vector3::Zero(), false);
	mainGameScene->AddObjectToScene(staticObject);
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
