#include "TimersPCH.h"

Timer::Timer(TimerHandle handle, float startingTime, bool startNow)
{
	m_TimerHandle = handle;
	m_CurrentDuration = startingTime;

	if (startNow)
		Start();
}

Timer::~Timer()
{
}

void Timer::Start()
{
	m_IsRunning = true;
}

void Timer::Pause()
{
	m_IsRunning = false;
}

void Timer::Reset(bool startNow)
{
	m_IsFinished = false;
	m_CurrentDuration = 0.0f;
	
	if (startNow)
		Start();
}

void Timer::Update(float deltaTime)
{
	m_CurrentDuration += deltaTime;

	if (m_CurrentDuration >= m_TimerHandle.GetTargetDuration())
	{
		m_CurrentDuration = m_TimerHandle.GetTargetDuration();
		m_IsFinished = true;

		m_TimerHandle.m_EndFunction();

		if (m_TimerHandle.GetIsLooping())
		{
			Reset(true);
		}
	}
}
