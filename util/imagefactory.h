#pragma once

#include "../globals.h"

namespace reutil {

class Image
{
	friend class ImageFactory;
public:
	Image();
	~Image();

	void clear();
	void releaseData();

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	unsigned int *getData() { return m_data; }

private:
	int m_width;
	int m_height;
	unsigned int *m_data;

};

class ImageFactory
{
public:
	static class Image* loadPNG(const std::string& filename);
	static class Image* loadJPG(const std::string& filename);
	static class Image* loadBMP(const std::string& filename);
	static class Image* createEmpty(int width, int height);
};

} // end of namespace 'reutil'
