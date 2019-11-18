#include "ECSPCH.h"
#include "BaseObject.h"

BaseObject::BaseObject()
{
	
}

BaseObject::~BaseObject()
{
	for (auto iteratorSystem : m_ObjectComponents)
	{
		delete iteratorSystem.second;
		iteratorSystem.second = nullptr;
	}
	m_ObjectComponents.clear();
}

void BaseObject::Update(float deltaTime)
{
	for (auto Component : m_ObjectComponents)
	{
		reinterpret_cast<BaseComponent*>(Component.second)->Update(deltaTime);
	}
}

void BaseObject::Draw()
{
	for (auto Component : m_ObjectComponents)
	{
		reinterpret_cast<BaseComponent*>(Component.second)->Draw();
	}
}

bool BaseObject::ObjectHasTag(std::string Tag)
{
	for (std::string tag : m_Tags)
	{
		if (tag == Tag)
			return true;
	}

	return false;
}

void BaseObject::Startup()
{
	if (m_MainComponent == nullptr)
	{
		CreateComponent<BaseComponent>("RootComponent");
	}

	for (auto Component : m_ObjectComponents)
	{
		reinterpret_cast<BaseComponent*>(Component.second)->Startup();
	}

	m_HasStarted = true;
}

void BaseObject::AttachTo(BaseObject* attachingObject, bool useAttachingTransform)
{
	AttachTo(attachingObject);

	if (useAttachingTransform)
	{
		SetTransform(attachingObject->GetTransform());
	}
}

BaseComponent* BaseObject::GetComponentByName(std::string name)
{
	auto it = m_ObjectComponents.find(name);
	if (it == m_ObjectComponents.end())
		return nullptr;

	return it->second;
}
