#include "glwindowsdl.h"

using namespace recore;

GLWindowSDL::GLWindowSDL()
{
}

GLWindowSDL::~GLWindowSDL()
{
}

bool GLWindowSDL::create(int width, int height, int bpp, bool fullscreen)
{
	unsigned int sdlFlags = 0;

	if (fullscreen)
	{
		sdlFlags |= SDL_FULLSCREEN;
	}
	else
	{
		sdlFlags |= SDL_RESIZABLE;
	}

	m_screen = SDL_SetVideoMode(width, height, bpp, sdlFlags);
	if (m_screen == NULL)
	{
		return false;
	}

	// TODO: some option for that
	// Hide mouse cursor (in both fullscreen and windowed modes)
	if (fullscreen || !fullscreen)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WarpMouse(x, y);
	}
	return true;
}

void GLWindowSDL::destroy()
{
	// None ... since SDL does everything
}

void GLWindowSDL::swapBuffers()
{
	SDL_GL_SwapBuffers();
}

void GLWindowSDL::setTitle(const char* title)
{
	SDL_WM_SetCaption(title, NULL);
}
