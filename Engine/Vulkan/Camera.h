#pragma once
#include "../ECS/ECSPCH.h"

class Camera :
    public BaseGameObject
{

private:
    float m_FOV;

public:
    Camera();
    ~Camera();

    inline void SetFOV(float inFOV) { m_FOV = inFOV; }
    inline float GetFOV() { return m_FOV; }
};

