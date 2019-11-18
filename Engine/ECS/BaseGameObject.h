#pragma once
#include "ECSPCH.h"

class BaseObject;

class BaseGameObject : public BaseObject
{
public:
	BaseGameObject();
	virtual ~BaseGameObject();

	virtual void Update(float deltaTime);

	virtual void Draw();
	inline BaseComponent* GetMainComponent() const { return m_MainComponent; }

	inline virtual void SetMainComponent(BaseComponent* newBaseComponent) { m_MainComponent = newBaseComponent; }

protected:
	virtual void Startup();	
protected:
	std::vector<std::string> m_Tags;
	

};


