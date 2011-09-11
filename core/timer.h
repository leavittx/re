#pragma once

#include "../globals.h"
#include <cstdlib>

namespace recore {

class Timer
{
public:
	Timer() {}
	virtual ~Timer() {}

	void init()
	{
		m_initialTime = osGetTimeMsec();
		m_previousTime = m_initialTime;
		m_paused = false;
		m_elapsedTime = 0;
	}

	void setPaused(bool paused)
	{
		m_paused = paused;
	}

	void addTime(TimeT add)
	{
		m_elapsedTime += add;
//		if (m_elapsedTime < 0)
//		{
//			m_elapsedTime = 0;
//		}
	}

	void setTime(TimeT time)
	{
		m_elapsedTime = time;
//		if (m_elapsedTime < 0)
//		{
//			m_elapsedTime = 0;
//		}
	}

	void update(TimeT audioPosition, TimeT songLength)
	{
		TimeT currentTime = osGetTimeMsec();
		TimeT dt = currentTime - m_previousTime;
		m_previousTime = currentTime;

		if (m_paused)
		{
		}
		else
		{
			m_elapsedTime += dt;

			const TimeT LIMIT = 30;
			if (abs(m_elapsedTime - audioPosition) > LIMIT && m_elapsedTime <= songLength - 40)
			{
				//g_debug << "audio out of sync! timer elapsed time = " << m_elapsedTime << " audio timer = " << audioPosition << " delta = " << (audioPosition - m_elapsedTime) << "\n";
				m_elapsedTime = audioPosition;
			}
		}
	}

	TimeT getTime()
	{
		return m_elapsedTime;
	}

private:
	bool m_paused;
	TimeT m_initialTime;
	TimeT m_previousTime;
	TimeT m_elapsedTime;

protected:
	// Get system time in miliseconds
	virtual TimeT osGetTimeMsec() = 0;

	//TODO
	friend class System;
};

} // end of namespace 'recore'
