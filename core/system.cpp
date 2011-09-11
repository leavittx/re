#include "system.h"
#include "glwindowsdl.h"
#include "timersdl.h"
#include "util/debug.h"

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

	m_endTime = 100000;

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
	//	m_timer->update(getAudioPosition(), m_song->getLength());
	//	updateFPS();
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

	case KeyLeftArrow:
		//addTime(-adjust);
		break;

	case KeyRightArrow:
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
