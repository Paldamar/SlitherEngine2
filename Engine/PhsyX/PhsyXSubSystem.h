#pragma once
#include "SlitherSubSystem.h"

using namespace physx;

class PhsyXSubSystem : public SlitherSubSystem
{
public:
	PhsyXSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID = NULLSystem);
	virtual ~PhsyXSubSystem();

	static PxTransform PxTransformFromMatrix(MyMatrix matrix);

	static PxVec3 PxVec3FromVec3(Vector3 vector);
	static Vector3 Vec3FromPxVec3(PxVec3 vec3);

	static PxQuat PxQuatFromMatrix(MyMatrix matrix);
	static PxQuat PxQuatFromEuler(Vector3 eulerAngles);
	//static Vector3 EulerFromPxQuat(PxQuat quaternion);

	static Vector3 Vec3LocationFromPxTransform(PxTransform transform);
	//static Vector3 Vec3RotationFromPxTransform(PxTransform transform);

protected:

private:

};