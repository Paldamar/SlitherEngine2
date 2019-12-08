#include "PhsyXPCH.h"
#include "PhsyXSubSystem.h"
#include "EngineMacros.h"
#include "PhysXWorld.h"

PhsyXSubSystem::PhsyXSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID) : SlitherSubSystem(systemName, engineInstance, instanceID)
{

}

PhsyXSubSystem::~PhsyXSubSystem()
{
	for (auto world : m_PhysXWorlds)
	{
		SafeDelete(world.second);
	}
}

void PhsyXSubSystem::Update(float deltaTime)
{
	for (auto world : m_PhysXWorlds)
	{
		world.second->Update();
	}
}

PhysXWorld* PhsyXSubSystem::CreateWorld(std::string worldName)
{
	assert(m_PhysXWorlds.find(worldName) == m_PhysXWorlds.end());

	return m_PhysXWorlds[worldName] = new PhysXWorld(this);
}

PhysXWorld* PhsyXSubSystem::GetWorld(std::string worldName)
{
	assert(m_PhysXWorlds.find(worldName) != m_PhysXWorlds.end());

	return m_PhysXWorlds[worldName];
}



physx::PxTransform PhsyXSubSystem::PxTransformFromMatrix(MyMatrix matrix)
{
	PxTransform transform = PxTransform(PxVec3FromVec3(matrix.GetTranslation()), PxQuatFromMatrix(matrix));
	return physx::PxTransform();
}

Transform3D PhsyXSubSystem::TransformFromPxTransform(physx::PxTransform transform)
{
	PxMat44 pxMatrix = PxMat44(transform);
	MyMatrix newMatrix = MatrixFromPxMatrix(pxMatrix);
	return Transform3D(newMatrix);
}

MyMatrix PhsyXSubSystem::MatrixFromPxMatrix(PxMat44 pxMatrix)
{
	MyMatrix newMatrix = MyMatrix(
		pxMatrix.column0.x, pxMatrix.column0.y, pxMatrix.column0.z, pxMatrix.column0.w,
		pxMatrix.column1.x, pxMatrix.column1.y, pxMatrix.column1.z, pxMatrix.column1.w,
		pxMatrix.column2.x, pxMatrix.column2.y, pxMatrix.column2.z, pxMatrix.column2.w,
		pxMatrix.column3.x, pxMatrix.column3.y, pxMatrix.column3.z, pxMatrix.column3.w
	);
	return newMatrix;
}



PxVec3 PhsyXSubSystem::PxVec3FromVec3(Vector3 vector)
{
	return PxVec3(vector.x, vector.y, vector.z);
}

Vector3 PhsyXSubSystem::Vec3FromPxVec3(PxVec3 vec3)
{
	return Vector3(vec3.x, vec3.y, vec3.z);
}


PxQuat PhsyXSubSystem::PxQuatFromMatrix(MyMatrix matrix)
{
	return PxQuat(PxMat33(PxVec3(matrix.m11, matrix.m21, matrix.m31), PxVec3(matrix.m12, matrix.m22, matrix.m32), PxVec3(matrix.m13, matrix.m23, matrix.m33)));
}

PxQuat PhsyXSubSystem::PxQuatFromEuler(Vector3 eulerAngles)
{
	MyMatrix rotMat;
	rotMat.CreateRotation(eulerAngles);
	return PxQuatFromMatrix(rotMat);
}

//Vector3 PhsyXSubSystem::EulerFromPxQuat(PxQuat quaternion)
//{
//	return quaternion.
//}


Vector3 PhsyXSubSystem::Vec3LocationFromPxTransform(PxTransform transform)
{
	return Vec3FromPxVec3(transform.p);
}

//Vector3 PhsyXSubSystem::Vec3RotationFromPxTransform(PxTransform transform)
//{
//	return Vec3FromPxVec3(transform.q);
//}
