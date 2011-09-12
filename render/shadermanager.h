#pragma once

#include "../globals.h"
#include "shader.h"

namespace render {

class Shader;

class ShaderManager : public reutil::Singleton<ShaderManager>
{
public:
	void init();

	void bindShader(const std::string& name);
	Shader& getShader(const std::string& name);
	void unbindShader();
	void loadShaders();
	void freeShaders();
	void dumpUnusedShaders();

private:
	friend class Singleton;
	ShaderManager();
	~ShaderManager();

	char* loadShaderSource(std::string& filename);
	std::map<std::string, Shader*> m_shaders;
};

} // end  of namespace 'render'
