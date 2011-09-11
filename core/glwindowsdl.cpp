#include "glwindowsdl.h"
#include "keyboardsdl.h"
#include "inputmanager.h"

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

	sdlFlags |= SDL_OPENGL;

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

bool GLWindowSDL::pollEvents()
{
	SDL_Event event;

	//TODO: if or while
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_VIDEORESIZE:
			break;

		case SDL_KEYDOWN:
			InputManager::inst().sendKeyboardEvent(KeyboardSDL::resolveKeyCode(event.key.keysym.sym));
			break;

//		case SDL_MOUSEMOTION:
//			printf("Mouse moved by %d,%d to (%d,%d)\n",
//			       event.motion.xrel, event.motion.yrel,
//			       event.motion.x, event.motion.y);
//			break;
//		case SDL_MOUSEBUTTONDOWN:
//			printf("Mouse button %d pressed at (%d,%d)\n",
//			       event.button.button, event.button.x, event.button.y);
//			break;

		case SDL_QUIT:
			return false;

		default:
			// Unknown event
			break;
		}
	}
	return true;
}

void GLWindowSDL::swapBuffers()
{
	//SDL_UpdateRect(m_screen,0,0,0,0);
	SDL_GL_SwapBuffers();
}

void GLWindowSDL::setTitle(const char* title)
{
	SDL_WM_SetCaption(title, NULL);
}
