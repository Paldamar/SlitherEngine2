#include "SubsystemPCH.h"

TimerSubSystem::TimerSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID)
	: SlitherSubSystem(systemName, engineInstance, instanceID)
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

void TimerSubSystem::UpdateLowPriotityTimers(float deltaTime)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsRunning() && timer.second->GetHandle()->GetTimerImportance() == TimerPriority::Low)
			timer.second->Update(deltaTime);
	}
}

void TimerSubSystem::UpdateHighPriotityTimers(float deltaTime)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsRunning() && timer.second->GetHandle()->GetTimerImportance() == TimerPriority::High)
			timer.second->Update(deltaTime);
	}
}

bool TimerSubSystem::MakeNewTimer(TimerHandle* handle, float startTime, bool startNow)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->GetHandle() == handle)
			return false;
	}

	m_ActiveTimers[handle->GetTimerName()] = new Timer(handle, startTime, startNow);

	return true;
}

Timer* TimerSubSystem::GetTimerByName(std::string name)
{
	auto it = m_ActiveTimers.find(name);
	if (it == m_ActiveTimers.end())
		return nullptr;

	return it->second;
}

Timer* TimerSubSystem::GetTimerByHandle(TimerHandle* handle)
{
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->GetHandle() == handle)
			return timer.second;
	}

	return nullptr;
}

TimerFunc TimerSubSystem::CleanupInActiveTimers()
{
#if DESTROY_INACTIVE_TIMERS
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsFinished())
		{
			SafeDelete(timer.second);
			m_ActiveTimers.erase(timer.first);
		}
	}
#else
	for (std::pair<std::string, Timer*> timer : m_ActiveTimers)
	{
		if (timer.second->IsFinished())
		{
			m_InactiveTimers[timer.first] = timer.second;

			m_ActiveTimers.erase(timer.first);
		}
	}
#endif
	return;
}
