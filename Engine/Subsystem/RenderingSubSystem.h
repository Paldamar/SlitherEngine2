#pragma once

class SlitherSubSystem;
class MainRenderer;

class RenderingSubSystem :
	public SlitherSubSystem, public FBXRenderer
{
public:
	RenderingSubSystem(std::string systemName, SubSystemID instanceID = NULLSystem);
	~RenderingSubSystem();

	virtual void Init() override;
	virtual void SetCallBack(MsgProc_Callback callback) { m_Callback = callback; }

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

protected:
	MsgProc_Callback m_Callback;
};

