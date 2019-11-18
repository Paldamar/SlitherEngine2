#pragma once

class SlitherSubSystem;
class MainRenderer;

class RenderingSubSystem :
	public SlitherSubSystem
{
public:
	RenderingSubSystem(std::string systemName, SubSystemID instanceID = NULLSystem);
	~RenderingSubSystem();

	virtual void Init() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	Renderer* GetRenderer() const { return m_Renderer; }

protected:
	Renderer* m_Renderer;
};

