#pragma once

namespace recore {

class GLWindow
{
public:
	GLWindow() {}
	virtual ~GLWindow() {}

	virtual bool create(int width, int height, int bpp, bool fullscreen) = 0;
	virtual void destroy() = 0;

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};

} // end of namespace 'recore'
