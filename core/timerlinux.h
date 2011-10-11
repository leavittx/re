#pragma once

#include <sys/time.h>
#include "timer.h"

#include "util/debug.h"

namespace recore {

class TimerLinux : public Timer
{
public:
	TimerLinux() {}
	virtual ~TimerLinux() {}

private:
	virtual TimeT osGetTimeMsec()
	{
		//TODO
//		struct timespec ts;
//		clock_gettime(CLOCK_MONOTONIC, &ts);

//		TimeT msec = (TimeT)(ts.tv_nsec / 1000000.0);

//		reutil::g_debug << "time: " << ts.tv_nsec << " (in milliseconds: " << msec << ")" << std::endl;

//		return msec;
		return clock() / 1000.0;
	}
};

} // end of namespace 'recore'



