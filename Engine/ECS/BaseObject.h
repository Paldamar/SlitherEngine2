#pragma once
#include "ECSPCH.h"

class BaseComponent;
class BaseGameObject;

class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	virtual void Update(float deltaTime);

	virtual void Draw();

	virtual bool ObjectHasTag(std::string Tag);

	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}

	inline virtual void SetOwner(BaseObject* newOwner) { m_Owner = newOwner; }

	inline virtual BaseObject* GetOwner() const { return m_Owner; }

	inline virtual void AttachTo(BaseObject* attachingObject) { m_AttachedTo = attachingObject; }
	virtual void AttachTo(BaseObject* attachingObject, bool useAttachingTransform);

	BaseComponent* GetComponentByName(std::string name);

	inline virtual Transform3D GetTransform() { return m_Transform; }
	inline virtual Transform2D Get2DTransform() { return m_Transform.TwoDimensional(); }
	inline virtual void SetTransform(Transform3D newTransform) { m_Transform = newTransform; }
	// Make a transform with just a position
	inline virtual void SetTransform(Vector3 pos) { m_Transform = Transform3D(pos); }
	// Make a transform with a position, rotation, and scale
	inline virtual void SetTransform(Vector3 pos, Vector3 rot, Vector3 scal) { m_Transform = Transform3D(pos, rot, scal); }

	inline bool HasStarted() { return m_HasStarted; }

	inline std::string GetObjectName() { return m_ObjectName; }
	inline void SetName(std::string newName) { m_ObjectName = newName; }

protected:
	virtual void Startup();

	template<class component>
	component* CreateComponent(std::string componentName)
	{
		// Assert if we already have a component by the provided name.
		assert(m_ObjectComponents.find(componentName) == m_ObjectComponents.end());

		// Create the component
		BaseComponent* comp = new component;

		// If the creation failed.
		if (!comp)
			return NULL;

		// Set this object as it's owner.
		comp->SetOwner(this);

		// If our we don't have a main component set this new one as the main component.
		if (m_MainComponent == nullptr)
			m_MainComponent = comp;

		// Add the component to the map of this object's components.
		m_ObjectComponents[componentName] = comp;

		// Return the component
		return comp;
	}

	template<class component>
	component* CreateComponent(std::string componentName, BaseObject* owner)
	{
		component* newComponent = CreateComponent(componentName);

		if (newComponent)
		{
			newComponent->SetOwner(owner);
			newComponent->AttachTo(owner);

			return newComponent;
		}

		return NULL;
	}

	template<class component>
	component* CreateComponent(std::string componentName, BaseObject* owner, BaseObject* attchingObject)
	{
		component* newComponent = CreateComponent(componentName, owner);

		if (newComponent)
		{
			newComponent->AttachTo(attchingObject);

			return newComponent;
		}

		return NULL;
	}

protected:
	std::vector<std::string> m_Tags;
	BaseObject* m_Owner = nullptr;
	BaseObject* m_AttachedTo;
	BaseComponent* m_MainComponent = nullptr;
	friend class BaseScene;
	std::string m_ObjectName = "";
private:
	std::map<std::string, BaseComponent*> m_ObjectComponents;
	Transform3D m_Transform;

	template<class ObjectType>
	static const ObjectType m_ObjectType;
	bool m_HasStarted = false;
};

