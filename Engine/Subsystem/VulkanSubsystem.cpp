#include "SubsystemPCH.h"


VulkanSubsystem::VulkanSubsystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID)
	: SlitherSubSystem(systemName, engineInstance, instanceID), VulkanRenderer()
{

}

VulkanSubsystem::~VulkanSubsystem()
{
}

void VulkanSubsystem::Init()
{
	VulkanRenderer::InitVulkan();
}

void VulkanSubsystem::Update(float deltaTime)
{
}

void VulkanSubsystem::Draw()
{
}
