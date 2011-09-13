#pragma once

#include "../globals.h"
#include "render/fbo.h"
#include "render/texture.h"
#include "util/imagefactory.h"

namespace render {

class TextureManager : public reutil::Singleton<TextureManager>
{
public:
	void init();
	void clear();

	//textures
	void addTextureParameters(const std::string& name, TextureParameters* params);
	void loadImages();
	void uploadImages();
	void bindTexture(const std::string& name, int texunit = GL_TEXTURE0_ARB);
	void clearTextureUnits(); //for lazy progammers

	//images
	void addImage(std::string& name, reutil::Image *image);
	reutil::Image& image(const std::string& name);

	//framebuffer objects
	void bindDepthFBO();
	void bindTextureFBO(const std::string& name);
	void unbindFBO();

private:
	friend class Singleton;
	TextureManager();
	~TextureManager();

	static const int MAX_TEXTURES = 8;
	std::map<std::string, reutil::Image*> m_images;
	std::map<std::string, Texture*> m_textures;
	std::map<std::string, TextureParameters*> m_textureParameters;
	std::string m_lastBoundTexture[8];

	//rendertargets
	FBOManager *m_FBO;
};

} // end of namespace 'render'
