#include "SubsystemPCH.h"

#include "../SlitherEngine/Framework.h"

SlitherSubSystem::SlitherSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID)
	: m_InstanceID(instanceID),
	  m_SystemName(systemName),
	  m_EngineInstance(engineInstance)
{
}

SlitherSubSystem::~SlitherSubSystem()
{
}
