#include "texturemanager.h"
#include "util/stringutils.h"

using namespace render;
using namespace reutil;
using namespace std;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::init()
{
	g_debug << "starting up FBO manager" << endl;
	m_FBO = new FBOManager();
	m_FBO->init();

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		m_lastBoundTexture[i] = "no texture";
	}
}

void TextureManager::clear()
{
	map<string, Image*>::iterator it;
	for (it = m_images.begin(); it != m_images.end(); it++)
	{
		Image *i = (*it).second;
		i->releaseData();
		delete i;
	}
	m_images.clear();
	m_textures.clear();
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		m_lastBoundTexture[i] = "no texture";
	}
}

void TextureManager::addTextureParameters(const string& name, TextureParameters* params)
{
	m_textureParameters[name] = params;
}

void TextureManager::addImage(string& name, Image *image)
{
	m_images[name] = image;
}

void TextureManager::bindTexture(const string& name, int texunit)
{
	int texunitoffset = texunit - GL_TEXTURE0_ARB;
	if (texunitoffset < 0 || texunitoffset > MAX_TEXTURES)
	{
		g_debug << "trying to bind texture " << name << " to an invalid texture unit << " << texunitoffset << "!" << endl;
		return;
	}
	if (m_textures.find(name) != m_textures.end())
	{
//		TextureParameters &oldparams = *m_textureParameters[m_lastBoundTexture[texunitoffset]];
//		TextureParameters &newparams = *m_textureParameters[name];
//		if (newparams.m_linear != oldparams.m_linear)
//		{
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, newparams.m_linear ? GL_LINEAR : GL_NEAREST);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, newparams.m_linear ? GL_LINEAR : GL_NEAREST);
//		}
//		if (newparams.m_repeat != oldparams.m_repeat)
//		{
//			int wrap = newparams.m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
//			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, wrap);
//			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, wrap);
//		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		m_textures[name]->bind(texunit);
		m_lastBoundTexture[texunitoffset] = name;
	}
	else
	{
		g_debug << "trying to bind texture " << name << " that does not exist!" << endl;
	}
}

void TextureManager::clearTextureUnits()
{
	//this is slow but what the hell...
	glActiveTexture(GL_TEXTURE7_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE6_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE5_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE4_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1_ARB);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDisable(GL_TEXTURE_2D);
}

void TextureManager::loadImages()
{
	vector<string> filenames;
	string directory = "data/graphics/";
	reutil::StringUtils::iterateDirectory(directory, filenames, false);

	vector<string>::iterator it;
	for (it = filenames.begin(); it < filenames.end(); it++)
	{
		string filename = *it;
		string path = directory + filename;

		Image* image = ImageFactory::loadAny(path);
		if (image != 0)
		{
			addImage(filename, image);
		}
	}
}

void TextureManager::uploadImages()
{
	g_debug << "uploading textures to OpenGL" << endl;
	int videoMemoryConsumption = 0;
	int count = 0;
	map<string, Image*>::iterator it;

	//add extra texture parameters for default "no texture" image
	TextureParameters *tempparams = new TextureParameters();
	addTextureParameters(std::string("no texture"), tempparams);

	//! DEPRECATED
//	glEnable(GL_TEXTURE_2D);
	//loop through all images and upload the ones that need to be uploaded
	for (it = m_images.begin(); it != m_images.end(); it++)
	{
		const string& name = (*it).first;
		Image *image = (*it).second;
		TextureParameters params;

		//try to find corresponding texture parameters
		map<string, TextureParameters*>::iterator paramiterator;
		paramiterator = m_textureParameters.find(name);
		if (paramiterator != m_textureParameters.end())
		{
			params = *paramiterator->second;
		}
		else
		{
			//upload new default texturaparameters for this image
			TextureParameters *temp = new TextureParameters();
			temp->setDefaults();
			m_textureParameters[name] = temp;
			params = *temp;

		}

		//create the texture and upload it
		if (params.m_upload)
		{
			Texture *texture = new Texture();
			texture->upload(*image, params);
			m_textures[name] = texture;

			videoMemoryConsumption += image->getWidth() * image->getHeight() * 4;
			count++;
		}
		//release data on images after uploading
		if (!params.m_retain)
		{
			image->releaseData();
		}
	}
	g_debug << "uploaded " << count << " textures, total video memory usage: " << (videoMemoryConsumption/1000) << "k" << endl;
}

Image& TextureManager::image(const string& name)
{
	return *m_images[name];
}

void TextureManager::bindDepthFBO()
{
	m_FBO->bindDepthFBO();
}

void TextureManager::bindTextureFBO(const string& name)
{
	//find texture data
	Image& i = *m_images[name];
	Texture& t = *m_textures[name];

	int width = i.getWidth();
	int height = i.getHeight();
	int ID = t.getID();

	//select the correct FBO to bind it to
	if (width == 32 && height == 32)
	{
		m_FBO->bindTextureFBO32(ID);
	}
	else if (width == 64 && height == 64)
	{
		m_FBO->bindTextureFBO64(ID);
	}
	else if (width == 128 && height == 128)
	{
		m_FBO->bindTextureFBO128(ID);
	}
	else if (width == 256 && height == 256)
	{
		m_FBO->bindTextureFBO256(ID);
	}
	else if (width == 512 && height == 512)
	{
		m_FBO->bindTextureFBO512(ID);
	}
	else if (width == 1024 && height == 1024)
	{
		m_FBO->bindTextureFBO1024(ID);
	}
	else if (width == 2048 && height == 2048)
	{
		m_FBO->bindTextureFBO2048(ID);
	}
	else
	{
		g_debug << "Trying to bind texture " << name << " to a FBO but there is no suitable FBO available!" << endl;
	}
}

void TextureManager::unbindFBO()
{
	m_FBO->unbindFBO();
}
