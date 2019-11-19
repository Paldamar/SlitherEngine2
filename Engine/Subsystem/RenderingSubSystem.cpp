#include "SubsystemPCH.h"

RenderingSubSystem::RenderingSubSystem(std::string systemName, SubSystemID instanceID)
	: SlitherSubSystem(systemName, instanceID), FBXRenderer()
{
}

RenderingSubSystem::~RenderingSubSystem()
{
}

void RenderingSubSystem::Init()
{
	Renderer::Initialize(m_Callback);
}

void RenderingSubSystem::Update(float deltaTime)
{
//Renderer::Update()
}

void RenderingSubSystem::Draw()
{
//	m_Renderer->Draw();
}
