#pragma once

//#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include "core/glwindow.h"

namespace recore {

class GLWindowX : public GLWindow
{
public:
	GLWindowX();
	virtual ~GLWindowX();

	virtual bool create(int width, int height, int bpp, bool fullscreen);
	virtual void destroy();

	virtual bool pollEvents();
	virtual void swapBuffers();
	virtual void setTitle(const char* title);

	virtual int getWidth() const { return m_width; }
	virtual int getHeight() const { return m_height; }

private:
	Display* m_display;
	Window m_window;
	GLXContext m_context;

	Atom m_deleteMessage;
	Atom m_stateMessage;
	Atom m_fullscreenMessage;

	int m_width, m_height;
	bool m_fullscreen;
};

} // end of namespace 'recore'
