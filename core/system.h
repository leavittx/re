#pragma once

#include "../globals.h"
#include "core/config.h"
#include "core/glwindow.h"
#include "core/timer.h"

namespace recore {

class System : public reutil::Singleton<System>, public KeyboardEventsListener
{
public:
	void init();
	void kill();

	bool isRunning();

	void updateFPS();
	float getFPS();

	bool initOpenGL(Config &cfg);
	bool pollEvents();
	void swapBuffers();
	void setWindowTitle(const char* title);

	void update();

	int getTime();

	virtual void handleKeyboardEvent(Key key);

private:
	friend class Singleton;
	System();
	~System();

	// GL stuff
	GLWindow* m_glWindow;
	// Timer
	Timer* m_timer;

	TimeT m_endTime;

	// FPS
	int m_frameCount;
	int m_framePreviousTime;
	int m_frameTimer;
	float m_FPS;

	bool escape;

};

} // end of namespace 'recore'
