#pragma once

#include "../globals.h"
#include "util/imagefactory.h"

namespace render {

struct TextureParameters
{
	TextureParameters();
	~TextureParameters();

	bool m_upload;
	bool m_retain;
	bool m_linear;
	bool m_repeat;
	bool m_mipmap;
	void setDefaults();
};

class Texture
{
public:
	Texture();
	~Texture();

	GLuint getID() { return m_ID; }
	void upload(reutil::Image &sourceimage, TextureParameters &params);
	void bind(int texunit = 0);
	bool hasBeenUsed();

private:
	TextureParameters m_params;
	GLuint m_ID;
	bool m_hasBeenBound;
};

} // end of namespace 'render'
