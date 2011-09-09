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

	return true;
}

void GLWindowSDL::destroy()
{
	// None ... since SDL does everything
}
