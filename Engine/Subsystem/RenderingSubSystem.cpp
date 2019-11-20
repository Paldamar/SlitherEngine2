#include "SubsystemPCH.h"

RenderingSubSystem::RenderingSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID)
	: SlitherSubSystem(systemName, engineInstance,instanceID), FBXRenderer()
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
