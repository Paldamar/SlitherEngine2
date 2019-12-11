#pragma once
#include "../ECS/ECSPCH.h"

class VulkanCamera :
    public BaseGameObject
{

private:
    float m_FOV;

public:
	VulkanCamera();
    ~VulkanCamera();

    inline void SetFOV(float inFOV) { m_FOV = inFOV; }
    inline float GetFOV() { return m_FOV; }
};

