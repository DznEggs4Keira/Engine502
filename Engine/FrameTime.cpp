#include "FrameTime.h"



FrameTime::FrameTime()
{
}

FrameTime::FrameTime(const FrameTime& other)
{
}

FrameTime::~FrameTime()
{
}

bool FrameTime::Initialise()
{
	//check to see if system supports hi performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	//Find out how many times the frequency counter ticks every millisecond
	m_ticksPerMS = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void FrameTime::Frame()
{
	INT64 currentTime;
	float timeDifference;


	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	timeDifference = (float)(currentTime - m_startTime);

	// Calculate the frame time by the time difference over the timer speed resolution.
	m_frameTime = timeDifference / m_ticksPerMS;

	// Restart the timer.
	m_startTime = currentTime;

	return;
}

float FrameTime::GetFPS()
{
	return m_frameTime;
}
