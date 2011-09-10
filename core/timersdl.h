#pragma once

#include <SDL/SDL.h>
#include "timer.h"

namespace recore {

class TimerSDL : public Timer
{
public:
	TimerSDL() {}
	virtual ~TimerSDL() {}

private:
	virtual TimeT osGetTimeMsec()
	{
		return SDL_GetTicks();
	}
};

} // end of namespace 'recore'
