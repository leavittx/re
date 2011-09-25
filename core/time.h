#pragma once

#include "core/system.h"

namespace recore {

class Time
{
public:
	// Default is milliseconds
	static TimeT get()
	{
		return System::inst().getTime();
	}

	// Time in milliseconds
	static TimeT getms()
	{
		return System::inst().getTime();
	}

	// Time in seconds
	static float gets()
	{
		return System::inst().getTime() * 0.001f;
	}
};

typedef Time t;

} // end of namespace 'recore'
