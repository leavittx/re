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

	int getWidth()			{ return m_width; }
	int getHeight()			{ return m_height; }
	int getComponents()		{ return m_components; }
	int getFormat()			{ return m_format; }
	int getType()			{ return m_type; }
	unsigned int* getData() { return m_data; }

private:
	int m_width;
	int m_height;
	int m_components;
	int m_format;
	int m_type;
	unsigned int* m_data;
};

class ImageFactory
{
public:
	static class Image* loadAny(const std::string& filename);
	static class Image* createEmpty(int width, int height);
};

} // end of namespace 'reutil'
