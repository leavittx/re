#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

//#include <vector>
//using namespace std;

// Maximum length of shader name
#define MAX_SHADER_NAME_LENGTH	64


enum GLT_STOCK_SHADER { GLT_SHADER_IDENTITY = 0, GLT_SHADER_FLAT, GLT_SHADER_SHADED, GLT_SHADER_DEFAULT_LIGHT, GLT_SHADER_POINT_LIGHT_DIFF,
	GLT_SHADER_TEXTURE_REPLACE, GLT_SHADER_TEXTURE_MODULATE, GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, GLT_SHADER_TEXTURE_RECT_REPLACE,
	GLT_SHADER_LAST };

enum GLT_SHADER_ATTRIBUTE { GLT_ATTRIBUTE_VERTEX = 0, GLT_ATTRIBUTE_COLOR, GLT_ATTRIBUTE_NORMAL, 
	GLT_ATTRIBUTE_TEXTURE0, GLT_ATTRIBUTE_TEXTURE1, GLT_ATTRIBUTE_TEXTURE2, GLT_ATTRIBUTE_TEXTURE3,
	GLT_ATTRIBUTE_LAST};


struct SHADERLOOKUPETRY {
	char szVertexShaderName[MAX_SHADER_NAME_LENGTH];
	char szFragShaderName[MAX_SHADER_NAME_LENGTH];
	GLuint	uiShaderID;
};


class GLShaderManager
{
public:
	GLShaderManager(void);
	~GLShaderManager(void);

	// Call before using
	bool InitializeStockShaders(void);
	
	// Find one of the standard stock shaders and return it's shader handle.
	GLuint GetStockShader(GLT_STOCK_SHADER nShaderID);

	// Use a stock shader, and pass in the parameters needed
	GLint UseStockShader(GLT_STOCK_SHADER nShaderID, ...);

	// Load a shader pair from file, return NULL or shader handle.
	// Vertex program name (minus file extension)
	// is saved in the lookup table
	GLuint LoadShaderPair(const char *szVertexProgFileName, const char *szFragProgFileName);

	// Load shaders from source text.
	GLuint LoadShaderPairSrc(const char *szName, const char *szVertexSrc, const char *szFragSrc);

	// Ditto above, but pop in the attributes
	GLuint LoadShaderPairWithAttributes(const char *szVertexProgFileName, const char *szFragmentProgFileName, ...);
	GLuint LoadShaderPairSrcWithAttributes(const char *szName, const char *szVertexProg, const char *szFragmentProg, ...);

	// Lookup a previously loaded shader
	GLuint LookupShader(const char *szVertexProg, const char *szFragProg = 0);
	
protected:
	GLuint	uiStockShaders[GLT_SHADER_LAST];
	//		vector <SHADERLOOKUPETRY>	shaderTable;

};
