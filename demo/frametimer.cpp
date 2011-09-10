#include "frametimer.h"

using namespace redemo;

FrameTimer::FrameTimer()
{
	set(30, 10);
}

FrameTimer::FrameTimer(TimeT updatetime, int maxiterations)
{
	set(updatetime, maxiterations);
}

void FrameTimer::set(TimeT updatetime, int maxiterations)
{
	m_cumultime = 0;
	m_updatetime = updatetime;
	m_maxiterations = maxiterations;
	m_prevtime = 0;
	m_dt = 0;
}

void FrameTimer::update(TimeT currenttime)
{
	//time from the timer
	m_dt = currenttime - m_prevtime;
	m_prevtime = currenttime;

	if (m_dt < 0)
		m_dt = -m_dt;

	m_cumultime += m_dt;

	//clip
	if (m_cumultime > m_maxiterations * m_updatetime)
	{
		m_cumultime = m_maxiterations * m_updatetime;
	}
}

TimeT FrameTimer::getDeltaTime()
{
	return m_dt;
}

bool FrameTimer::stepsLeft()
{
	return (m_cumultime > m_updatetime);
}

void FrameTimer::endStep()
{
	m_cumultime -= m_updatetime;
}
