#pragma once

class SlitherSubSystem;

using namespace physx;

class PhsyXSubSystem : public SlitherSubSystem
{
public:
	PhsyXSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID = NULLSystem);
	virtual ~PhsyXSubSystem();

	static PxTransform PxTransformFromMatrix(MyMatrix matrix);
	static PxVec3 PxVec3FromVec3(Vector3 vector);
	static PxQuat PxQuatFromMatrix(MyMatrix matrix);

protected:

private:

};