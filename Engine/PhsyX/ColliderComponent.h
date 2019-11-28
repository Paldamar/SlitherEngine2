#pragma once

enum class CollisionShape;
class BaseComponent;

using namespace physx;
using namespace Ps;
using namespace physx::shdfnd;

class ColliderComponent// : public BaseComponent
{
public:
	ColliderComponent();
	~ColliderComponent();
	

	virtual void Init(MyMatrix World, CollisionShape shapeType, void* userData, bool IsTrigger, bool IsStatic, DirectX::XMFLOAT4 size, DirectX::XMFLOAT4 center, float radius, float height);

protected:

private:
	class RigidBody* m_RigidBody;
};