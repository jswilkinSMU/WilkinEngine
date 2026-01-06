#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Clock.hpp"

Timer::Timer(double period, const Clock* clock)
	:m_period(period), m_clock(clock)
{
	if (m_clock == nullptr)
	{
		m_clock = &m_clock->GetSystemClock();
	}
}

void Timer::Start()
{
	m_startTime = m_clock->GetTotalSeconds();
}

void Timer::Stop()
{
	m_startTime = -1.0;
}

double Timer::GetElapsedTime() const
{
	if (IsStopped())
	{
		return 0.0;
	}
	else
	{
		return (m_clock->GetTotalSeconds() - m_startTime);
	}
}

double Timer::GetElapsedFraction() const
{
	return (GetElapsedTime() / m_period);
}

bool Timer::IsStopped() const
{
	if (m_startTime < 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Timer::HasPeriodElapsed() const
{
	if (IsStopped())
	{
		return false;
	}
	if (GetElapsedTime() >= m_period)
	{
		return true;
	}
	return false;
}

bool Timer::DecrementPeriodIfElapsed()
{
	if (HasPeriodElapsed())
	{
		m_startTime += m_period;
		return true;
	}
	return false;
}
