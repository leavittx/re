#pragma once

#include "../globals.h"
#include "shader.h"
#include "math/math3d.h"

namespace render {

class Shader;

enum GLT_STOCK_SHADER { GLT_SHADER_IDENTITY = 0, GLT_SHADER_FLAT, GLT_SHADER_SHADED, GLT_SHADER_DEFAULT_LIGHT, GLT_SHADER_POINT_LIGHT_DIFF,
						GLT_SHADER_TEXTURE_REPLACE, GLT_SHADER_TEXTURE_MODULATE, GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, GLT_SHADER_TEXTURE_RECT_REPLACE,
						GLT_SHADER_LAST };

enum GLT_SHADER_ATTRIBUTE { GLT_ATTRIBUTE_VERTEX = 0, GLT_ATTRIBUTE_COLOR, GLT_ATTRIBUTE_NORMAL,
							GLT_ATTRIBUTE_TEXTURE0, GLT_ATTRIBUTE_TEXTURE1, GLT_ATTRIBUTE_TEXTURE2, GLT_ATTRIBUTE_TEXTURE3,
							GLT_ATTRIBUTE_LAST};

struct StockShaderUniforms
{
	// Everything
	StockShaderUniforms(float* _mvpMatrix, float* _mvMatrix, float* _pMatrix, float* _vLightPos, float* _vColor, int _textureUnit0 = 0)
		: mvpMatrix(_mvpMatrix), mvMatrix(_mvMatrix), pMatrix(_pMatrix), vLightPos(_vLightPos), vColor(_vColor), textureUnit0(_textureUnit0) {}
	// Just the modelview projection matrix and the color (FLAT shader)
	StockShaderUniforms(float* _mvpMatrix, float* _vColor)
		: mvpMatrix(_mvpMatrix), vColor(_vColor) {}
	// Just the texture place (TEXTURE_REPLACE shader)
	StockShaderUniforms(float* _mvpMatrix, int _textureUnit0)
		: mvpMatrix(_mvpMatrix), textureUnit0(_textureUnit0) {}
	// TEXTURE_MODULATE shader
	StockShaderUniforms(float* _mvpMatrix, float* _vColor, int _textureUnit0)
		: mvpMatrix(_mvpMatrix), vColor(_vColor), textureUnit0(_textureUnit0) {}
	// DEFAULT_LIGHT shader
	StockShaderUniforms(float* _mvMatrix, float* _pMatrix, float* _vColor)
		: mvMatrix(_mvMatrix), pMatrix(_pMatrix), vColor(_vColor) {}
	// POINT_LIGHT_DIFF shader
	StockShaderUniforms(float* _mvMatrix, float* _pMatrix, float* _vLightPos, float* _vColor)
		: mvMatrix(_mvMatrix), pMatrix(_pMatrix), vLightPos(_vLightPos), vColor(_vColor) {}
	// TEXTURE_POINT_LIGHT_DIFF shader
	StockShaderUniforms(float* _mvMatrix, float* _pMatrix, float* _vLightPos, float* _vColor, int _textureUnit0) :
		mvMatrix(_mvMatrix), pMatrix(_pMatrix), vLightPos(_vLightPos), vColor(_vColor), textureUnit0(_textureUnit0) {}
	// Just the modelview projection matrix. Color is an attribute (SHADED shader :))
	StockShaderUniforms(float* _mvpMatrix)
		: mvpMatrix(_mvpMatrix) {}
	// Just the Color (IDENTITY shader)
	//	StockShaderUniforms(float* _vColor) : vColor(_vColor) {}

	float* mvpMatrix;
	float* mvMatrix;
	float* pMatrix;
	float* vLightPos;
	float* vColor;
	int textureUnit0;
};

class ShaderManager : public reutil::Singleton<ShaderManager>
{
public:
	// Initialize and load the stock shaders
	void init();

	void bindShader(const std::string& name);
	Shader& getShader(const std::string& name);
	void unbindShader();
	void loadShaders();
	void freeShaders();
	void dumpUnusedShaders();

	GLuint GetStockShader(GLT_STOCK_SHADER nShaderID);
	GLint UseStockShader(GLT_STOCK_SHADER nShaderID, const StockShaderUniforms& uniforms);

	typedef std::vector< std::pair<int, const char*> > ShaderAttributeList;

private:
	friend class Singleton;
	ShaderManager();
	~ShaderManager();

	char* loadShaderSource(std::string& filename);
	std::map<std::string, Shader*> m_shaders;

	GLuint m_stockShaders[GLT_SHADER_LAST];

	bool InitializeStockShaders(void);
	GLuint LoadShaderPairSrcWithAttributes(const char *szVertexSrc, const char *szFragmentSrc, const ShaderAttributeList& attributeList);
};

} // end  of namespace 'render'
