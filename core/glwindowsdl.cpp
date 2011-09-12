#include "glwindowsdl.h"
#include "keyboardsdl.h"
#include "inputmanager.h"
#include "render/glhelper.h"

using namespace recore;

GLWindowSDL::GLWindowSDL()
{
}

GLWindowSDL::~GLWindowSDL()
{
}

bool GLWindowSDL::create(int width, int height, int bpp, bool fullscreen)
{
	// Form needed flags for SDL initialization
	unsigned int sdlFlags = 0;
	sdlFlags |= SDL_OPENGL;
	if (fullscreen) sdlFlags |= SDL_FULLSCREEN;
	else            sdlFlags |= SDL_RESIZABLE;

	// Set up video mode of main window
	m_screen = SDL_SetVideoMode(width, height, bpp, sdlFlags);
	if (m_screen == NULL)
	{
		return false;
	}

	// Hide mouse cursor
	// TODO: some option for hiding mouse
	if (fullscreen || !fullscreen)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WarpMouse(x, y);
	}

	// Set up some of opengl stuff
	render::gl::init(getWidth(), getHeight(), render::gl::ASPECTRATIO_4_3);

	return true;
}

void GLWindowSDL::destroy()
{
	SDL_Quit();
	// None (else) ... since SDL does everything
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
			// Previous bpp value is stored in m_screen->format->BitsPerPixel, but default is ok
			m_screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, m_screen->flags);
			render::gl::resize(getWidth(), getHeight(), render::gl::ASPECTRATIO_4_3);
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
//	SDL_UpdateRect(m_screen,0,0,0,0);
	SDL_GL_SwapBuffers();
}

void GLWindowSDL::setTitle(const char* title)
{
	SDL_WM_SetCaption(title, NULL);
}
