
#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_

#include <windows.h>


class FrameTime
{
public:
	FrameTime();
	FrameTime(const FrameTime&);
	~FrameTime();

	bool Initialise();
	void Frame();

	float GetFPS();

private:
	INT64 m_frequency;
	float m_ticksPerMS;
	INT64 m_startTime;
	float m_frameTime;
};
#endif
