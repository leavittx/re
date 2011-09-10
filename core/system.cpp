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

	m_frameCount = 0;
	m_frameTimer = 0;
	m_framePreviousTime = 0;
	m_FPS = 0.0f;

	m_timer = new TimerSDL();
}

void System::kill()
{
	m_glWindow->destroy();
}

bool System::isRunning()
{
	//TODO
	//running if not pressed esc or the song hasn't ended
	bool escape = 0;//getKeyDown(VK_ESCAPE);
	bool demodone = (m_timer->getTime() >= m_endTime);

	return !escape && !demodone;
}

bool System::initOpenGL(Config &cfg)
{
	// Create main window with opengl rendering support
	if (!m_glWindow->create(cfg.getScreenWidth(), cfg.getScreenHeight(), 32,
	                        cfg.getFullscreen()))
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

// explicit instantiation
//template class System<>;
