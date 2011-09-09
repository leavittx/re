#pragma once

#include <SDL/SDL.h>
#include "core/glwindow.h"

namespace recore {

//class GLWindow;

class GLWindowSDL : public GLWindow
{
public:
    GLWindowSDL();
	virtual ~GLWindowSDL();

	virtual bool create(int width, int height, int bpp, bool fullscreen);
	virtual void destroy();

	virtual int getWidth() const { return m_screen->w; }
	virtual int getHeight() const { return m_screen->h; }

private:
	SDL_Surface* m_screen; // Framebuffer surface which is main SDL window
};

} // end of namespace 'recore'
