#include "ECSPCH.h"

BaseGameObject::BaseGameObject() : BaseObject()
{
}

BaseGameObject::~BaseGameObject()
{
}

void BaseGameObject::Startup()
{
	BaseObject::Startup();
}

void BaseGameObject::Update(float deltaTime)
{
	BaseObject::Update(deltaTime);
}

void BaseGameObject::Draw()
{
	BaseObject::Draw();
}