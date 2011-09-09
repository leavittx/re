#include "system.h"

using namespace std;
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

template<typename TimeT>
	bool System<TimeT>::initOpenGL(Config &cfg)
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

template<typename TimeT>
	void System<TimeT>::swapBuffers()
{
	m_glWindow->swapBuffers();
}

template<typename TimeT>
	void System<TimeT>::setWindowTitle(char* title)
{
	m_glWindow->setTitle(title);
}

template<typename TimeT>
	void System<TimeT>::update()
{
//	m_timer->update(getAudioPosition(), m_song->getLength());
//	updateFPS();
}

template<typename TimeT>
	int System<TimeT>::getTime()
{
	return m_timer->getTime();
}

// explicit instantiation
template class System<>;
