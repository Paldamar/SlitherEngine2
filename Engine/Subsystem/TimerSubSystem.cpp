#include "SubsystemPCH.h"

TimerSubSystem::TimerSubSystem(std::string systemName, SubSystemID instanceID) : SlitherSubSystem(systemName, instanceID)
{
}

TimerSubSystem::~TimerSubSystem()
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		SafeDelete(timer.second);
		m_ActiveTimers.erase(timer.first);
	}
	m_ActiveTimers.clear();
}

void TimerSubSystem::Init()
{
}

void TimerSubSystem::UpdateLowPriotityTimers(float deltaTime)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsRunning() && timer.second->GetHandle().GetTimerImportance() == TimerPriority::Low)
			timer.second->Update(deltaTime);
	}
}

void TimerSubSystem::UpdateHighPriotityTimers(float deltaTime)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsRunning() && timer.second->GetHandle().GetTimerImportance() == TimerPriority::High)
			timer.second->Update(deltaTime);
	}
}

bool TimerSubSystem::MakeNewTimer(TimerHandle handle, float startTime, bool startNow)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->GetHandle() == handle)
			return false;
	}

	m_ActiveTimers[handle.GetTimerName()] = new Timer(handle, startTime, startNow);

	return true;
}

Timer* TimerSubSystem::GetTimerByName(std::string name)
{
	auto it = m_ActiveTimers.find(name);
	if (it == m_ActiveTimers.end())
		return nullptr;

	return it->second;
}

Timer* TimerSubSystem::GetTimerByHandle(TimerHandle handle)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->GetHandle() == handle)
			return timer.second;
	}

	return nullptr;
}

void TimerSubSystem::CleanupInActiveTimers()
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsFinished())
		{
			SafeDelete(timer.second);
			m_ActiveTimers.erase(timer.first);
		}
	}
}
