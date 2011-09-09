#include "frametimer.h"

using namespace redemo;

template <typename TimeT>
	FrameTimer<TimeT>::FrameTimer()
{
	set(30, 10);
}

template <typename TimeT>
	FrameTimer<TimeT>::FrameTimer(TimeT updatetime, int maxiterations)
{
	set(updatetime, maxiterations);
}

template <typename TimeT>
	void FrameTimer<TimeT>::set(TimeT updatetime, int maxiterations)
{
	m_cumultime = 0;
	m_updatetime = updatetime;
	m_maxiterations = maxiterations;
	m_prevtime = 0;
	m_dt = 0;
}

template <typename TimeT>
	void FrameTimer<TimeT>::update(TimeT currenttime)
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

template <typename TimeT>
	TimeT FrameTimer<TimeT>::getDeltaTime()
{
	return m_dt;
}

template <typename TimeT>
	bool FrameTimer<TimeT>::stepsLeft()
{
	return (m_cumultime > m_updatetime);
}

template <typename TimeT>
	void FrameTimer<TimeT>::endStep()
{
	m_cumultime -= m_updatetime;
}

template class FrameTimer<>; // explicit instantiation
