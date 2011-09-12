#include "imagefactory.h"

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
	//create image and
	image->m_width = width;
	image->m_height = height;
	image->m_data = new unsigned int [image->m_width * image->m_height];
	memset(image->m_data, 0, image->m_width*image->m_height*sizeof(unsigned int));
	return image;
}

Image* ImageFactory::loadJPG(std::string& filename)
{
	unsigned int imageID;
	unsigned char *ptr = NULL, *dest = NULL;

	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	// Load image from a file
	if(!ilLoadImage((char *)filename.c_str()))
	{
		g_debug << "loading image " << filename << " failed!" << std::endl;
		return 0;
	}

	Image *image = new Image();
	image->m_width = ilGetInteger(IL_IMAGE_WIDTH);
	image->m_height = ilGetInteger(IL_IMAGE_HEIGHT);
	image->m_data = new unsigned int[image->m_width*image->m_height];

	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	ptr = ilGetData();
	dest = (unsigned char *)image->m_data;

	for (int i = 0; i < image->m_width*image->m_height; i++, dest++)
	{
		for (int bytes = 0; bytes <= 16; bytes += 8)
			*dest |= (*ptr++ << bytes);
		*dest |= 0xFF; //no alpha channel
	}

	ilDeleteImages(1, &imageID);

	return image;
}

Image* ImageFactory::loadPNG(std::string& filename)
{
	unsigned int imageID;

	ilInit();
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	// Load image from a file
	if(!ilLoadImage((char *)filename.c_str()))
	{
		g_debug << "loading image " << filename << " failed!" << std::endl;
		return 0;
	}

	Image *image = new Image();
	image->m_width = ilGetInteger(IL_IMAGE_WIDTH);
	image->m_height = ilGetInteger(IL_IMAGE_HEIGHT);
	image->m_data = new unsigned int[image->m_width*image->m_height];

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	unsigned char *source = (unsigned char *)ilGetData();
	unsigned int *dest = image->m_data;
	for (int i = 0; i < image->m_width * image->m_height; i++, dest++)
	{
//		*dest |= (*source++);       // R
//		*dest |= (*source++ << 8);  // G
//		*dest |= (*source++ << 16); // B
//		*dest |= (*source++ << 24); // A
		for (int bytes = 0; bytes <= 24; bytes += 8)
		{
			*dest |= (*source++ << bytes);
		}
	}

	ilDeleteImages(1, &imageID);

	return image;
}
