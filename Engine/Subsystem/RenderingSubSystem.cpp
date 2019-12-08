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
	FBXRenderer::Initialize(m_Callback);
}

void RenderingSubSystem::Update(float deltaTime)
{
}

void RenderingSubSystem::Draw()
{
}
