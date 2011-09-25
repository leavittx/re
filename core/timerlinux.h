#pragma once

#include <ctime>
#include "timer.h"

namespace recore {

class TimerLinux : public Timer
{
public:
	TimerLinux() {}
	virtual ~TimerLinux() {}

private:
	virtual TimeT osGetTimeMsec()
	{
		static struct timespec time;
		clock_gettime(CLOCK_REALTIME, &time);
		return (int)(time.tv_nsec / 1000000.0);
	}
};

} // end of namespace 'recore'



