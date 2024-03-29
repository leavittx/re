#pragma once

#include <SDL/SDL.h>
#include "core/glwindow.h"

namespace recore {

class GLWindowSDL : public GLWindow
{
public:
    GLWindowSDL();
	virtual ~GLWindowSDL();

	virtual bool create(int width, int height, int bpp, bool fullscreen);
	virtual void destroy();

	virtual bool pollEvents();
	virtual void swapBuffers();
	virtual void setTitle(const char* title);

	virtual int getWidth() const { return m_screen->w; }
	virtual int getHeight() const { return m_screen->h; }

private:
	SDL_Surface* m_screen; // Framebuffer surface which is main SDL window
};

} // end of namespace 'recore'
