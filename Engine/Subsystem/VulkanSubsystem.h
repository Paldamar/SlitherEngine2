#pragma once
#include "../Vulkan/VulkanRenderer.h"


class SlitherSubsystem;


class VulkanSubsystem :
	public SlitherSubSystem, public VulkanRenderer
{
public:
	VulkanSubsystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID = NULLSystem);
	~VulkanSubsystem();

	virtual void Init() override;

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
};