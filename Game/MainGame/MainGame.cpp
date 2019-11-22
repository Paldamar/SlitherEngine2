#include "GamePCH.h"
#include "MainGame.h"
#include "PhsyXSubSystem.h"

MainGame::MainGame(Framework* frameWork) : GameCore(frameWork)
{

}

MainGame::~MainGame()
{

}

void MainGame::LoadContent()
{
	MyMatrix matrix;
	matrix.CreateRotation(Vector3(187.0f, -53.0f, 15.0f));
	physx::PxQuat quat = PhsyXSubSystem::PxQuatFromMatrix(matrix);
	OutputMessage("w: %f, x: %f, y: %f, z: %f\n", quat.w, quat.x, quat.y, quat.z);
}
