#pragma once

#include "globals.h"
#include "util/singleton.h"
#include "core/config.h"
#include "core/glwindow.h"
#include "core/timer.h"

namespace recore {

//TODO
//typedef System::inst g_system;

//template <typename TimeT = unsigned int>
//	class System : public reutil::Singleton< System >
class System : public reutil::Singleton< System >
{
	//TODO
	//friend class Singleton;
public:
	void init();
	void kill();

	bool isRunning();

	void updateFPS();
	float getFPS();

	bool initOpenGL(Config &cfg);
	void swapBuffers();
	void setWindowTitle(const char* title);

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
	Timer* m_timer;
};

} // end of namespace 'recore'
