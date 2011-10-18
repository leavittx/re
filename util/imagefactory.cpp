#include "imagefactory.h"
#include <cstring> // for memset()

// Devil library headers
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

using namespace reutil;

/**
  Image class methods
*/

Image::Image()
{
	clear();
}

Image::~Image()
{
}

void Image::clear()
{
	m_width = 0;
	m_height = 0;
	m_data = 0;
}

void Image::releaseData()
{
	if (m_data)
	{
		delete [] m_data;
		m_data = 0;
	}
}

/**
  ImageFactory class methods
*/

Image* ImageFactory::createEmpty(int width, int height)
{
	Image *image = new Image();
	image->m_width = width;
	image->m_height = height;
	image->m_data = new unsigned char [image->m_width * image->m_height];
	memset(image->m_data, 0, image->m_width*image->m_height*sizeof(unsigned char));
	return image;
}

Image* ImageFactory::loadAny(const std::string& filename)
{
	unsigned int imageID;

	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	// Load image from a file
	if (!ilLoadImage((char *)filename.c_str()))
	{
		g_debug << "ERROR: loading image " << filename << " failed" << std::endl;
		return 0;
	}

	int dimH = ilGetInteger(IL_IMAGE_HEIGHT);
	int dimW = ilGetInteger(IL_IMAGE_WIDTH);
	int format = ilGetInteger(IL_IMAGE_FORMAT);
	int type = ilGetInteger(IL_IMAGE_TYPE);
//	int dSize = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

	unsigned int typeSize = 0;
	switch (type)
	{
	case IL_BYTE:
		typeSize = sizeof(ILbyte);
		break;
	case IL_UNSIGNED_BYTE:
		typeSize = sizeof(ILubyte);
		break;
	case IL_SHORT:
		typeSize = sizeof(ILshort);
		break;
	case IL_UNSIGNED_SHORT:
		typeSize = sizeof(ILushort);
		break;
	case IL_INT:
		typeSize = sizeof(ILint);
		break;
	case IL_UNSIGNED_INT:
		typeSize = sizeof(ILuint);
		break;
	case IL_HALF: // map half to float for memory
	case IL_FLOAT:
		typeSize = sizeof(ILfloat);
		break;
	case IL_DOUBLE:
		typeSize = sizeof(ILdouble);
		break;
	default:
		typeSize = 0;
		break;
	}
	if (typeSize == 0)
	{
		g_debug << "ERROR: loading image " << filename << " failed (unknown type)" << std::endl;
		return 0;
	}

	unsigned int channels = 0;
	switch (format)
	{
	case IL_ALPHA:
		channels = 1;
		break;
	case IL_LUMINANCE:
		channels = 1;
		break;
	case IL_LUMINANCE_ALPHA:
		channels = 2;
		break;
	case IL_RGB:
		channels = 3;
		break;
	case IL_BGR:
		channels = 3;
		break;
	case IL_RGBA:
		channels = 4;
		break;
	case IL_BGRA:
		channels = 4;
		break;
	case IL_COLOR_INDEX: // unsupported
		g_debug << "WARNING: Indexed color images are unsupported" << std::endl;
	default:
		channels = 0;
		break;
	}
	if (channels == 0)
	{
		g_debug << "ERROR: loading image " << filename << " failed (unknown format)" << std::endl;
		return 0;
	}

	Image *image = new Image();
	image->m_width  = dimW;
	image->m_height = dimH;
	image->m_components = (channels == 4) ? IL_RGBA : IL_RGB;
	image->m_format = (format == IL_ALPHA) || (format == IL_LUMINANCE) ? 0x1902 /* GL_DEPTH_COMPONENT */ :
					  (format == IL_COLOR_INDEX) ? IL_RGB : format;
	image->m_type = type;
	image->m_data = new unsigned char [dimH * dimW * channels];
	//(unsigned int *)malloc(dimH * dimW * channels * typeSize);

	ilCopyPixels(0, 0, 0, dimW, dimH, 1, format, type, image->m_data);
	ilDeleteImages(1, &imageID);
	return image;
}
