#pragma once

#include <SDL/SDL.h>
#include "timer.h"

namespace recore {

template <typename TimeT>
	class TimerSDL : public Timer<TimeT>
{
public:
	TimerSDL() {}
	~TimerSDL() {}

private:
	virtual TimeT osGetTimeMsec()
	{
		return SDL_GetTicks();
	}
};

} // end of namespace 'recore'
