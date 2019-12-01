#include "ECSPCH.h"
#include "../World/WorldPCH.h"

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

bool BaseObject::Kill()
{
	m_IsActive = false;
	return true;
}

SlitherWorld* BaseObject::GetWorld()
{
	if (m_Scene)
	{
		if (m_Scene->GetWorld())
		{
			return m_Scene->GetWorld();
		}
		else
			return nullptr;
	}
	else
		return nullptr;

	return nullptr;
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

void BaseObject::UpdateAttachedObjects()
{
	for (BaseObject* obj : m_AttachedObjects)
	{
		obj->SetTransform(m_Transform + obj->m_OffsetFromAttachedObject);
	}
}

void BaseObject::AttachTo(BaseObject* attachingObject, bool useAttachingTransform, Transform3D offset)
{
	m_AttachedObjects.push_back(attachingObject);
	m_OffsetFromAttachedObject = offset;

	if (useAttachingTransform)
	{
		SetTransform(m_Transform + offset);
	}
}

BaseComponent* BaseObject::GetComponentByName(std::string name)
{
	auto it = m_ObjectComponents.find(name);
	if (it == m_ObjectComponents.end())
		return nullptr;

	return it->second;
}
