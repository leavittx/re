#pragma once

#include "util/singleton.h"
#include "core/glwindow.h"
#include "core/timer.h"

namespace recore {

//TODO
//typedef System::inst g_system;

template <typename TimeT = unsigned int>
	class System : public reutil::Singleton< System<TimeT> >
{
	//TODO
	friend class Singleton;
public:
	void init();
	void kill();

	bool isRunning();

	void updateFPS();
	float getFPS();

	void swapBuffers();
	void update();

	int getTime();

public:
	System();
	~System();

	// GL stuff
	GLWindow* m_glWindow;

	// FPS
	int m_frameCount;
	int m_framePreviousTime;
	int m_frameTimer;
	float m_FPS;

	TimeT m_endTime;

	// Timer
	Timer<TimeT>* m_timer;
};

} // end of namespace 'recore'
