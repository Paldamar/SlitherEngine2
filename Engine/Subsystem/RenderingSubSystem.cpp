#include "SubsystemPCH.h"

RenderingSubSystem::RenderingSubSystem(std::string systemName, SubSystemID instanceID)
	: SlitherSubSystem(systemName, instanceID)
{
}

RenderingSubSystem::~RenderingSubSystem()
{
}

void RenderingSubSystem::Init()
{
//	m_Renderer = new Renderer();
}

void RenderingSubSystem::Update(float deltaTime)
{
//	m_Renderer->Update();
}

void RenderingSubSystem::Draw()
{
//	m_Renderer->Draw();
}
