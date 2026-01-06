#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
// Defining a system clock
static Clock* s_systemClock = new Clock();
// -----------------------------------------------------------------------------
Clock::Clock()
{
	m_parent = s_systemClock;
	if (m_parent)
	{
		m_parent->AddChild(this);
	}
}

Clock::~Clock()
{
	// Unparent ourself and our children
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}

	// Unparent all children
	for (size_t clockIndex = 0; clockIndex < m_children.size(); ++clockIndex)
	{
		m_children[clockIndex]->m_parent = nullptr;
	}
}

Clock::Clock(Clock& parent)
	:m_parent(&parent)
{
	m_parent->AddChild(this);
}

void Clock::Reset()
{
	// Set book keeping variables back to 0.
	m_lastUpdateTimeInSeconds = s_systemClock->GetTotalSeconds();
	m_totalSeconds = 0.0;
	m_deltaSeconds = 0.0;
	m_frameCount = 0;
	m_isPaused = false;
	m_stepSingleStep = false;

	m_maxDeltaSeconds = 0.1;
}

bool Clock::IsPaused() const
{
	return m_isPaused;
}

void Clock::Pause()
{
	m_isPaused = true;
	m_deltaSeconds = 0.0;
}

void Clock::Unpause()
{
	m_isPaused = false;
}

void Clock::TogglePause()
{
	m_isPaused = !m_isPaused;
}

void Clock::StepSingleFrame()
{
	m_stepSingleStep = true;
}

void Clock::SetTimeScale(double timeScale)
{
	m_timeScale = timeScale;
}

double Clock::GetTimeScale() const
{
	return m_timeScale;
}

double Clock::GetDeltaSeconds() const
{
	return m_deltaSeconds;
}

double Clock::GetTotalSeconds() const
{
	return m_totalSeconds;
}

double Clock::GetFrameRate() const
{
	if (m_frameCount > 0)
	{
		return m_frameCount / m_totalSeconds;
	}
	return 0.0;
}

int Clock::GetFrameCount() const
{
	return m_frameCount;
}

Clock& Clock::GetSystemClock()
{
	return *s_systemClock;
}

void Clock::TickSystemClock()
{
	s_systemClock->Tick();
}

void Clock::Tick()
{
	double currentTime = GetCurrentTimeSeconds();

	double deltaTimeSeconds = currentTime - m_lastUpdateTimeInSeconds;
	deltaTimeSeconds = GetClamped(deltaTimeSeconds, 0.0, m_maxDeltaSeconds);

	m_lastUpdateTimeInSeconds = currentTime;

	Advance(deltaTimeSeconds);
}

void Clock::Advance(double deltaTimeSeconds)
{
	if (m_stepSingleStep)
	{
		m_isPaused = false;
	}
	else if (m_isPaused)
	{
		deltaTimeSeconds = 0.0;
	}

	deltaTimeSeconds *= m_timeScale;
	m_deltaSeconds = deltaTimeSeconds;
	m_totalSeconds += m_deltaSeconds;
	m_frameCount += 1;

	for (size_t childClockIndex = 0; childClockIndex < m_children.size(); ++childClockIndex)
	{
		Clock* childClock = m_children[childClockIndex];
		childClock->Advance(deltaTimeSeconds);
	}

	if (m_stepSingleStep)
	{
		m_stepSingleStep = false;
		m_isPaused = true;
	}
}

void Clock::AddChild(Clock* childClock)
{
	m_children.push_back(childClock);
}

void Clock::RemoveChild(Clock* childClock)
{
	for (size_t clockIndex = 0; clockIndex < m_children.size(); ++clockIndex)
	{
		if (m_children[clockIndex] == childClock)
		{
			m_children[clockIndex] = nullptr;
			return;
		}
	}
}
