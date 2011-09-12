#pragma once

//#include "../globals.h"
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
	static TimeT gets()
	{
		return System::inst().getTime() * 0.001f;
	}

	//	static TimeT operator () ()
	//	{
	//		return System::inst().getTime();
	//	}
};

typedef Time t;

} // end of namespace 'recore'
