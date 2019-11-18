#pragma once

class BaseObject;
class BaseGameObject;

class BaseComponent : public BaseObject
{
public:
	BaseComponent();
	virtual ~BaseComponent() {};

	virtual void Draw() {}
	virtual void Update(float deltaTime) {}
protected:
	virtual void Startup() override;
	friend BaseObject;
private:	
	friend BaseGameObject;
};

