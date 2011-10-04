#include "glwindowx.h"
#include "keyboardx.h"
#include "inputmanager.h"
#include "render/glhelper.h"

//#include <cstring>

using namespace recore;

GLWindowX::GLWindowX()
{
	m_display = 0;
	m_window = 0;
	m_context = 0;
	m_deleteMessage = 0;
	m_stateMessage = 0;
	m_fullscreenMessage = 0;
}

GLWindowX::~GLWindowX()
{
}

void EarlyInitGLXfnPointers()
{
	glGenVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");
	glBindVertexArrayAPPLE = (void(*)(const GLuint))glXGetProcAddressARB((GLubyte*)"glBindVertexArray");
	glDeleteVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");
	glXCreateContextAttribsARB = (GLXContext(*)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list))glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
	glXChooseFBConfig = (GLXFBConfig*(*)(Display *dpy, int screen, const int *attrib_list, int *nelements))glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");
	glXGetVisualFromFBConfig = (XVisualInfo*(*)(Display *dpy, GLXFBConfig config))glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
}

bool GLWindowX::create(int width, int height, int bpp, bool fullscreen)
{
	// Avoid compiler warning
	m_bpp = bpp;
	// Remember if full screen mode (for window destroy)
	m_fullscreen = fullscreen;
	// Remember width & height
	m_width = width;
	m_height = height;

#if 0
	Window RootWindow = 0;
	XVisualInfo* VisualInfo = None;
	Colormap CurrentColorMap = 0;
	XSetWindowAttributes CurrentSetWindowAttibutes;
	int fbcCount = 0;
	GLXFBConfig* fbc = None;

	int visualAttribList[] = {
		GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE,
		GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, True, None };

	m_display = XOpenDisplay(None);

	if (m_display == None)
	{
		return false;
	}

	RootWindow = DefaultRootWindow(m_display);

	if (!(fbc = glXChooseFBConfig(m_display, DefaultScreen(m_display), visualAttribList, &fbcCount)))
	{
		destroy();
		return false;
	}

	VisualInfo = glXGetVisualFromFBConfig(m_display, fbc[0]);

	if (VisualInfo == None)
	{
		XFree(fbc);
		destroy();
		return false;
	}

	CurrentColorMap = XCreateColormap(m_display, RootWindow, VisualInfo->visual, AllocNone);

	CurrentSetWindowAttibutes.colormap = CurrentColorMap;
	CurrentSetWindowAttibutes.event_mask =
			KeyPressMask |
			KeyReleaseMask |
			ButtonPressMask |
			ButtonReleaseMask |
			StructureNotifyMask;

	m_window = XCreateWindow(m_display, RootWindow, 0, 0, m_width, m_height, 0,
							 VisualInfo->depth, InputOutput, VisualInfo->visual,
							 CWColormap | CWEventMask, &CurrentSetWindowAttibutes);

	if (m_window == 0)
	{
		XFree(fbc);
		destroy();
		return false;
	}

	//TODO
	// Turn off resizing if needed
//	setResizeMode();

	// Retrieve some useful atoms
	m_deleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
	m_stateMessage = XInternAtom(m_display, "_NET_WM_STATE", False);
	m_fullscreenMessage = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);

	XSetWMProtocols(m_display, m_window, &m_deleteMessage, 1);

	XMapWindow(m_display, m_window);

	XStoreName(m_display, m_window, "X11 window");

	// Go full screen if needed
	if (m_fullscreen)
	{
		XEvent xev;

		Cursor InvisibleCursor = None;
		char cursorData[32];
		XColor cursorColor;
		Pixmap cursorPixmap;

		memset(cursorData, 0, sizeof(cursorData));

		memset(&cursorColor, 0, sizeof(cursorColor));

		cursorPixmap = XCreateBitmapFromData(m_display, RootWindow, cursorData, 16, 16);

		if (cursorPixmap != None)
		{
			InvisibleCursor = XCreatePixmapCursor(m_display, cursorPixmap, cursorPixmap, &cursorColor, &cursorColor, 0, 0);

			XFreePixmap(m_display, cursorPixmap);

			if (InvisibleCursor != None)
			{
				XDefineCursor(m_display, m_window, InvisibleCursor);
			}
		}

		//		m_stateMessage = XInternAtom(m_display, "_NET_WM_STATE", False);
		//		m_fullscreenMessage = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);

		memset(&xev, 0, sizeof(xev));

		xev.xclient.type = ClientMessage;
		xev.xclient.window = m_window;
		xev.xclient.message_type = m_stateMessage;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 1;
		xev.xclient.data.l[1] = m_fullscreenMessage;

		XSendEvent(m_display, RootWindow, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
	}

	m_context = glXCreateContext(m_display, VisualInfo, None, true);

	if (m_context == None)
	{
		XFree(fbc);
		destroy();
		return false;
	}

	if (!glXMakeCurrent(m_display, m_window, m_context))
	{
		XFree(fbc);
		destroy();
		return false;
	}

	// Create our window's GLX context
	GLXContext Context = None;

	int attribList[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_FLAGS_ARB, 0, 0 };

//	attribList[1] = 3;//g_major;
//	attribList[3] = 3;//g_minor;
//	attribList[5] = 0;//g_flags;

	glXCreateContextAttribsARB = (GLXContext(*)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list))glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");

	if (!(Context = glXCreateContextAttribsARB(m_display, fbc[0], 0, True, attribList)))
	{
		XFree(fbc);
		destroy();
		return false;
	}

	XFree(fbc);

	if (!glXMakeCurrent(m_display, None, None))
	{
		glXDestroyContext(m_display, Context);
		destroy();
		return false;
	}

	glXDestroyContext(m_display, m_context);

	m_context = Context;

	if (!glXMakeCurrent(m_display, m_window, m_context))
	{
		destroy();
		return false;
	}
#endif

	XSetWindowAttributes winAttribs;
	GLint winmask;
	GLint nMajorVer = 0;
	GLint nMinorVer = 0;
	XVisualInfo *visualInfo;
	GLXFBConfig *fbConfigs;
	int numConfigs = 0;
	static int fbAttribs[] = {
					GLX_RENDER_TYPE,   GLX_RGBA_BIT,
					GLX_X_RENDERABLE,  True,
					GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
					GLX_DOUBLEBUFFER,  True,
					GLX_RED_SIZE,      8,
					GLX_BLUE_SIZE,     8,
					GLX_GREEN_SIZE,    8,
					0 };

	EarlyInitGLXfnPointers();

	// Tell X we are going to use the display
	m_display = XOpenDisplay(NULL);

	// Get Version info
	glXQueryVersion(m_display, &nMajorVer, &nMinorVer);
//	printf("Supported GLX version - %d.%d\n", nMajorVer, nMinorVer);

	if (nMajorVer == 1 && nMinorVer < 2)
	{
//		printf("ERROR: GLX 1.2 or greater is necessary\n");
		XCloseDisplay(m_display);
		return false;
	}
	// Get a new fb config that meets our attrib requirements
	fbConfigs = glXChooseFBConfig(m_display, DefaultScreen(m_display), fbAttribs, &numConfigs);
	visualInfo = glXGetVisualFromFBConfig(m_display, fbConfigs[0]);

	// Now create an X window
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask |
							KeyPressMask | PointerMotionMask    |
							StructureNotifyMask;

	winAttribs.border_pixel = 0;
	winAttribs.bit_gravity = StaticGravity;
	winAttribs.colormap = XCreateColormap(m_display,
										  RootWindow(m_display, visualInfo->screen),
										  visualInfo->visual, AllocNone);
	winmask = CWBorderPixel | CWBitGravity | CWEventMask| CWColormap;

	m_window = XCreateWindow(m_display, DefaultRootWindow(m_display), 20, 20,
				 m_width, m_height, 0,
				 visualInfo->depth, InputOutput,
				 visualInfo->visual, winmask, &winAttribs);

	XMapWindow(m_display, m_window);

	// Retrieve some useful atoms
	m_deleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
	m_stateMessage = XInternAtom(m_display, "_NET_WM_STATE", False);
	m_fullscreenMessage = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);

	// Make it full screen if needed
	if (m_fullscreen)
	{
		goFullscreen();
	}

	// Also create a new GL context for rendering
	GLint attribs[] = {
	  GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
	  GLX_CONTEXT_MINOR_VERSION_ARB, 3,
	  0 };

	m_context = glXCreateContextAttribsARB(m_display, fbConfigs[0], 0, True, attribs);
	glXMakeCurrent(m_display, m_window, m_context);

	return true;
}

void GLWindowX::goFullscreen()
{
	// Hide cursor
	Cursor InvisibleCursor = None;
	char cursorData[32];
	XColor cursorColor;
	Pixmap cursorPixmap;

	memset(cursorData, 0, sizeof(cursorData));

	memset(&cursorColor, 0, sizeof(cursorColor));

	cursorPixmap = XCreateBitmapFromData(m_display, DefaultRootWindow(m_display), cursorData, 16, 16);

	if (cursorPixmap != None)
	{
		InvisibleCursor = XCreatePixmapCursor(m_display, cursorPixmap, cursorPixmap, &cursorColor, &cursorColor, 0, 0);

		XFreePixmap(m_display, cursorPixmap);

		if (InvisibleCursor != None)
		{
			XDefineCursor(m_display, m_window, InvisibleCursor);
		}
	}

	//		m_stateMessage = XInternAtom(m_display, "_NET_WM_STATE", False);
	//		m_fullscreenMessage = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", False);

	XEvent xev;
	memset(&xev, 0, sizeof(xev));

	xev.xclient.type = ClientMessage;
	xev.xclient.window = m_window;
	xev.xclient.message_type = m_stateMessage;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 1;
	xev.xclient.data.l[1] = m_fullscreenMessage;

	XSendEvent(m_display, DefaultRootWindow(m_display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

void GLWindowX::destroy()
{
	if (m_fullscreen && m_display && m_window)
	{
		XEvent xev;

		memset(&xev, 0, sizeof(xev));

		XUndefineCursor(m_display, m_window);

		xev.xclient.type = ClientMessage;
		xev.xclient.window = m_window;
		xev.xclient.message_type = m_stateMessage;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 0;
		xev.xclient.data.l[1] = m_fullscreenMessage;

		XSendEvent(m_display, DefaultRootWindow(m_display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
	}

	if (m_display)
	{
		glXMakeCurrent(m_display, None, None);
	}

	if (m_display && m_context)
	{
		glXDestroyContext(m_display, m_context);
		m_context = None;
	}

	if (m_display && m_window)
	{
		XDestroyWindow(m_display, m_window);
	}

	if (m_display)
	{
		XCloseDisplay(m_display);
		m_display = None;
	}
}

bool GLWindowX::pollEvents()
{
	XEvent event;
	if (XPending(m_display))
	{
		XNextEvent(m_display, &event);

		switch (event.type) {
		case KeyPress:
			InputManager::inst().sendKeyboardEvent(KeyboardX::resolveKeyCode(XLookupKeysym(&event.xkey, 0)));
			break;

		case KeyRelease:
			break;

		case ClientMessage:
			// Have We Received a Delete Message
			if ((unsigned)event.xclient.data.l[0] == m_deleteMessage)
			{
				return false;
			}
			break;

		case ConfigureNotify:
			m_width = event.xconfigure.width;
			m_height = event.xconfigure.height;
			render::gl::resize(getWidth(), getHeight(), render::gl::ASPECTRATIO_16_10);

//			glViewport(0, 0, width, height);
			//do reshape here (not needed)
			break;

		case MapNotify:
//			active = true;
			break;

		case UnmapNotify:
//			active = false;
			break;

		default:
			// Unknown event
			break;
		}
	}
	return true;
}

void GLWindowX::swapBuffers()
{
	glXSwapBuffers(m_display, m_window);
}

void GLWindowX::setTitle(const char* title)
{
	XStoreName(m_display, m_window, title);
	XFlush(m_display);
}
