#include "PhsyXPCH.h"
#include "SlitherSubSystem.h"
#include "PhsyXSubSystem.h"

PhsyXSubSystem::PhsyXSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID) : SlitherSubSystem(systemName, engineInstance, instanceID)
{

}

PhsyXSubSystem::~PhsyXSubSystem()
{

}

physx::PxTransform PhsyXSubSystem::PxTransformFromMatrix(MyMatrix matrix)
{
	//PxTransform transform = PxTransform(PxVec3FromVec3(matrix.GetTranslation()), PxQuat();
	return physx::PxTransform();
}

PxVec3 PhsyXSubSystem::PxVec3FromVec3(Vector3 vector)
{
	return PxVec3(vector.x, vector.y, vector.z);
}

PxQuat PhsyXSubSystem::PxQuatFromMatrix(MyMatrix matrix)
{
	return PxQuat(PxMat33(PxVec3(matrix.m11, matrix.m21, matrix.m31), PxVec3(matrix.m12, matrix.m22, matrix.m32), PxVec3(matrix.m13, matrix.m23, matrix.m33)));
}
