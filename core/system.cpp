#include "system.h"
#include "glwindowsdl.h"
#include "timersdl.h"

using namespace std;
using namespace recore;

System::System()
{
}

System::~System()
{
}

void System::init()
{
	m_glWindow = new GLWindowSDL();
	m_timer = new TimerSDL();

	m_endTime = 1000000;

	m_frameCount = 0;
	m_frameTimer = 0;
	m_framePreviousTime = 0;
	m_FPS = 0.0f;

	InputManager::inst().acceptKeyboardEvents(this);
}

void System::kill()
{
	m_glWindow->destroy();
}

bool System::isRunning()
{
	// Running if not pressed esc and the song hasn't ended
	return !escape && !(m_timer->getTime() >= m_endTime);
}

void System::updateFPS()
{
	int currentTime = getTime();
	int dt = currentTime - m_framePreviousTime;
	m_framePreviousTime = currentTime;

	if (dt < 0)
		dt = -dt;

	m_frameCount++;
	m_frameTimer += dt;

	//update per second
	if (m_frameTimer > 1000)
	{
		m_FPS = 1000.0f * (m_frameCount / (float)m_frameTimer);
		m_frameCount = 0;
		m_frameTimer = 0;
	}

}

float System::getFPS()
{
	return m_FPS;
}

bool System::initOpenGL(Config &cfg)
{
	// Create main window with opengl rendering support
	if (!m_glWindow->create(cfg.getScreenWidth(), cfg.getScreenHeight(), 32, cfg.getFullscreen()))
	{
		g_debug << "ERROR! Could not create window!" << endl;
		return false;
	}
	// Initialize glew
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		g_debug << "ERROR! Cannot initialize GLEW!" << endl;
	}
	//TODO
	m_timer->init();
	return true;
}

bool System::pollEvents()
{
	return m_glWindow->pollEvents();
}

void System::swapBuffers()
{
	m_glWindow->swapBuffers();
}

void System::setWindowTitle(const char* title)
{
	m_glWindow->setTitle(title);
}

void System::update()
{
	//TODO
	//m_timer->update(getAudioPosition(), m_song->getLength());
	m_timer->update(m_timer->osGetTimeMsec(), 1000000);
	updateFPS();
}

int System::getTime()
{
	return m_timer->getTime();
}

void System::handleKeyboardEvent(Key key)
{
	switch (key)
	{
	case KeyEsc:
		escape = true;
		break;

	case KeyLeft:
		//addTime(-adjust);
		break;

	case KeyRight:
		//addTime(adjust);
		break;

	case KeySpace:
		// Pause/resume
		//toggleSongPause();
		break;

	case KeyS:
		//toggleSound();
		break;

	default:
		return;
	}
}

// explicit instantiation
//template class System<>;
