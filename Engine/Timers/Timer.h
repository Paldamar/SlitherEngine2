#pragma once

class TimerHandle;

class Timer
{
public:
	Timer(TimerHandle* handle, float startingTime = 0.0f, bool startNow = false);
	~Timer();

	TimerHandle* GetHandle() const { return m_TimerHandle; }

	void Start();
	void Pause();
	void Reset(bool startNow = false);

	void Update(float deltaTime);

	inline bool IsFinished() const { return m_IsFinished; }
	inline bool IsRunning() const { return m_IsRunning; }
protected:
	bool m_IsFinished = false;
	bool m_IsRunning = false;
	float m_CurrentDuration = 0.0f;
private:
	TimerHandle* m_TimerHandle;
};