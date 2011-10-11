#include "glwindowx.h"
#include "keyboardx.h"
#include "inputmanager.h"
#include "render/glhelper.h"
#include "util/debug.h"

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

void GLWindowX::EarlyInitGLXfnPointers()
{
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
					GLX_SAMPLE_BUFFERS_ARB, 1,
					GLX_SAMPLES_ARB,   4,
					0 };

	EarlyInitGLXfnPointers();

	// Tell X we are going to use the display
	m_display = XOpenDisplay(NULL);

	// Get Version info
	glXQueryVersion(m_display, &nMajorVer, &nMinorVer);
	reutil::g_debug << "Supported GLX version - " << nMajorVer << "." << nMinorVer << std::endl;

	if (nMajorVer == 1 && nMinorVer < 2)
	{
		reutil::g_debug << "ERROR: GLX 1.2 or greater is necessary" << std::endl;
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

	// Do we need a full-screen window?
	if (m_fullscreen)
		goFullscreen();

	// Context creation
	if (glXCreateContextAttribsARB != NULL)
	{
		GLint attribs[] = {
		    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 3,
			0 };

		m_context = glXCreateContextAttribsARB(m_display, fbConfigs[0], 0, True, attribs);
	}
	else
	{
		m_context = glXCreateContext(m_display, visualInfo, None, true);
	}

	if (m_context == None)
	{
		XFree(fbConfigs);
		destroy();
		return false;
	}
	if (!glXMakeCurrent(m_display, m_window, m_context))
	{
		XFree(fbConfigs);
		destroy();
		return false;
	}

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
