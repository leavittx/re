#include "system.h"

#include "glwindowsdl.h"
#include "timersdl.h"

using namespace recore;

template<typename TimeT>
	System<TimeT>::System()
{
}

template<typename TimeT>
	System<TimeT>::~System()
{
}

template<typename TimeT>
	void System<TimeT>::init()
{
	m_glWindow = new GLWindowSDL();

	m_frameCount = 0;
	m_frameTimer = 0;
	m_framePreviousTime = 0;
	m_FPS = 0.0f;

	m_timer = new TimerSDL<TimeT>();
}

template<typename TimeT>
	void System<TimeT>::kill()
{
	m_glWindow->destroy();
}

template<typename TimeT>
	bool System<TimeT>::isRunning()
{
	//TODO
	//running if not pressed esc or the song hasn't ended
	bool escape = 0;//getKeyDown(VK_ESCAPE);
	bool demodone = (m_timer->getTime() >= m_endTime);

	return !escape && !demodone;
}

//TODO: separate file templateinstantiations.cpp for all explicit instantiations
template class System<unsigned int>; // explicit instantiation
