#pragma once

class SlitherSubSystem;
class Timer;


class TimerSubSystem :	public SlitherSubSystem
{
public:
	TimerSubSystem(std::string systemName, Framework* engineInstance, SubSystemID instanceID = NULLSystem);
	virtual ~TimerSubSystem();

	virtual void UpdateLowPriotityTimers(float deltaTime);
	virtual void UpdateHighPriotityTimers(float deltaTime);

	// Returns true is successful
	virtual bool MakeNewTimer(TimerHandle* handle, float startTime = 0.0f, bool startNow = false);

	Timer* GetTimerByName(std::string name);
	Timer* GetTimerByHandle(TimerHandle* handle);

	void CleanupInActiveTimers();
protected:
	std::map<std::string, Timer*> m_ActiveTimers;
#if !DESTROY_INACTIVE_TIMERS
	std::map<std::string, Timer*> m_InactiveTimers;
#endif // !DESTROY_INACTIVE_TIMERS

};