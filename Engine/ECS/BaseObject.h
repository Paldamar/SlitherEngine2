#pragma once
#include "ECSPCH.h"

class BaseComponent;
class BaseGameObject;
class BaseScene;
class SlitherWorld;

/*
enum TickPriority
{
	High,			// Will update at the begining of the frame.
	Default,		// Updates after physics but before being drawn.
	Low				// Updates at the end of frame.
};
*/

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

	virtual void AttachTo(BaseObject* attachingObject, bool useAttachingTransform = false, Transform3D offset = Transform3D(Vector3(0.0f)));

	BaseComponent* GetComponentByName(std::string name);

	inline virtual Transform3D GetTransform() { return m_Transform; }
	//inline virtual Transform2D Get2DTransform() { return m_Transform.TwoDimensional(); }
	inline virtual void SetTransform(Transform3D newTransform) { m_Transform = newTransform; UpdateAttachedObjects(); }

	// Make a transform with just a position.
	inline virtual void SetTransform(Vector3 pos) { m_Transform = Transform3D(pos); }
	// Make a transform with a position, rotation, and scale.
	inline virtual void SetTransform(Vector3 pos, Vector3 rot, Vector3 scal) { m_Transform = Transform3D(pos, rot, scal); }
	// Get Object's position.
	inline virtual Vector3 GetObjectLocation() { return m_Transform.matrix.GetTranslation(); }
	// Get Object's rotation.
	inline virtual Vector3 GetObjectRotation() { return m_Transform.matrix.GetEulerAngles(); }
	// Get Object's scale.
	inline virtual Vector3 GetObjectScale() { return m_Transform.matrix.GetScale(); }
	// Set Object's position.
	inline virtual void SetObjectLocation(Vector3 pos) { m_Transform.SetLocation(pos); UpdateAttachedObjects();	}
	// Set Object's rotation.
	inline virtual void SetObjectRotation(Vector3 rot) { m_Transform.SetRotation(rot); UpdateAttachedObjects();	}
	// Set Object's scale.
	inline virtual void SetObjectScale(Vector3 scale) {	m_Transform.SetScale(scale); UpdateAttachedObjects(); }

	inline bool HasStarted() { return m_HasStarted; }

	inline std::string GetObjectName() { return m_ObjectName; }
	inline void SetName(std::string newName) { m_ObjectName = newName; }

	// Is the object active in the world it is in.
	inline bool IsActive() const { return m_IsActive; }

	// Returns true if successful.
	virtual bool Kill();

	// Get all objects attached to this object.
	inline std::vector<BaseObject*> GetAttachedObjects() const { return m_AttachedObjects; }

	// Get offset used for attached object
	inline Transform3D GetOffset() const { return m_OffsetFromAttachedObject; }

	inline BaseScene* GetScene() { return m_Scene; }

	SlitherWorld* GetWorld();

	inline void SetCanTick(bool tick) { m_CanTick = tick; }
	inline bool CanTick() { return m_CanTick; }

	template<class component>
	component* CreateComponent(std::string componentName)
	{
		// Assert if we already have a component by the provided name.
		assert(m_ObjectComponents.find(componentName) == m_ObjectComponents.end());

		// Create the component
		component* comp = new component;

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
		component* newComponent = CreateComponent<component>(componentName);

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
	virtual void Startup();

protected:
	// Tags that are on this character.
	std::vector<std::string> m_Tags;
	// The object that owners this object.
	BaseObject* m_Owner = nullptr;
	// The object this object is attached to.
	BaseObject* m_AttachedTo;
	// The main component of this object.
	BaseComponent* m_MainComponent = nullptr;
	// The name of this object.
	std::string m_ObjectName = "";
	// Offset from attached object
	Transform3D m_OffsetFromAttachedObject;
	// Scene that this object is in.
	BaseScene* m_Scene = nullptr;

	friend class BaseScene;

private:
	// Map of all components.
	std::map<std::string, BaseComponent*> m_ObjectComponents;
	// Transform of this object.
	Transform3D m_Transform = Transform3D(Vector3(0));
	// Has loaded completely. 
	bool m_HasStarted = false;
	// Is active in the world.
	bool m_IsActive = true;
	// Can object update
	bool m_CanTick = true;
	// Vector of objects attached to this object.
	std::vector<BaseObject*> m_AttachedObjects;
	// Importance of when it ticks
	//TickPriority m_TickPriority = TickPriority::Default;

private:
	// Update all the attached object's transforms.
	void UpdateAttachedObjects();
};

