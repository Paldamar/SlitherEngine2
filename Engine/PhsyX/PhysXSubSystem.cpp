#include "PhsyXPCH.h"
#include "PhsyXSubSystem.h"

PhsyXSubSystem::PhsyXSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID) : SlitherSubSystem(systemName, engineInstance, instanceID)
{

}

PhsyXSubSystem::~PhsyXSubSystem()
{

}

physx::PxTransform PhsyXSubSystem::PxTransformFromMatrix(MyMatrix matrix)
{
	PxTransform transform = PxTransform(PxVec3FromVec3(matrix.GetTranslation()), PxQuatFromMatrix(matrix));
	return physx::PxTransform();
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
