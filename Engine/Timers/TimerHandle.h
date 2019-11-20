#pragma once
#include <string>

typedef void* (*TimerFunc)();

enum TimerPriority
{
	High, // Timers under High will be updated at the beginning of the frame.
	Low   // Timers under Low will be updated at the end of the frame.
};

class TimerHandle
{
public:
	TimerHandle() {};
	TimerHandle(float tagetDuration, TimerPriority importance, std::string timerName = "", 
		bool isLooping = false, void* owningObject = nullptr, TimerFunc returningFunc = nullptr)
	{
		m_TargetDuration = tagetDuration;
		m_Importance = importance;
		m_TimerName = timerName;
		m_IsLooping = isLooping;
		m_OwningObject = owningObject;
		m_EndFunction = returningFunc;
	}

	inline bool GetIsLooping() const { return m_IsLooping; }
	inline float GetTargetDuration() const { return m_TargetDuration; }
	inline std::string GetTimerName() const { return m_TimerName; }
	inline TimerPriority GetTimerImportance() const { return m_Importance; }

	inline void* GetOwningObject() { return m_OwningObject; }

	bool operator == (const TimerHandle& o) 
	{
		return this->GetIsLooping() == o.GetIsLooping() &&
			   //this->GetOwningObject() == o.GetOwningObject() && // not compatible - cannot compare void pointers.
			   this->GetTargetDuration() == o.GetTargetDuration() &&
			   this->GetTimerImportance() == o.GetTimerImportance() &&
			   this->GetTimerName() == o.GetTimerName();
	}
	bool operator != (const TimerHandle& o)

	{
		return this->GetIsLooping() != o.GetIsLooping() ||
			   //this->GetOwningObject() != o.GetOwningObject() || // not compatible - cannot compare void pointers.
			   this->GetTargetDuration() != o.GetTargetDuration() ||
			   this->GetTimerImportance() != o.GetTimerImportance() ||
			   this->GetTimerName() != o.GetTimerName();
	}

protected:
	bool m_IsLooping = false;
	float m_TargetDuration = 0.001f;
	std::string m_TimerName = "";
	TimerPriority m_Importance = Low;
	TimerFunc m_EndFunction = nullptr;

	void* m_OwningObject = nullptr;

	friend class Timer;
};