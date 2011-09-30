#include "shadermanager.h"
#include "util/stringutils.h"

#include "stockshaders.h"

using namespace render;
using namespace std;

ShaderManager::ShaderManager()
{
}
ShaderManager::~ShaderManager()
{
}

void ShaderManager::init()
{
	InitializeStockShaders();
}

void ShaderManager::bindShader(const string& name)
{
	m_shaders[name]->bind();
}

void ShaderManager::unbindShader()
{
	glUseProgramObjectARB(0);
}

Shader& ShaderManager::getShader(const string& name)
{
	if (m_shaders.find(name) == m_shaders.end())
	{
		g_debug << "Cannot find shader " << name << "!" << endl;
	}
	return *m_shaders[name];
}

void ShaderManager::loadShaders()
{
	string shaderPath = "data/shaders/";
	string vertexDirectory = "data/shaders/*.vert";
	string fragmentDirectory = "data/shaders/*.frag";

	const int baselen = shaderPath.length();     //base directory name length
	const int extlen = string(".vert").length(); //shader extension length

	vector<string> vertexFilenames;
	vector<string> fragmentFilenames;

	// Fetch the list of shader files
	reutil::StringUtils::iterateDirectory(vertexDirectory, vertexFilenames);
	reutil::StringUtils::iterateDirectory(fragmentDirectory, fragmentFilenames);

	if (vertexFilenames.size() != fragmentFilenames.size())
	{
		g_debug << "Error! Vertex and fragment shader counts do not match!" << endl;
		g_debug << "    " << vertexFilenames.size() << " vertex and "
						<< fragmentFilenames.size() << " fragment shaders" << endl;

		for (int i = 0; i < (int)vertexFilenames.size(); i++)
		{
			string& s = vertexFilenames[i];
			cout << "   vertex shader filename: " << s << endl;
		}
		for (int i = 0; i < (int)fragmentFilenames.size(); i++)
		{
			string& s = fragmentFilenames[i];
			cout << "   fragment shader filename: " << s << endl;
		}
		return;
	}
	g_debug << "" << endl;
	g_debug << "loadShaders()" << endl;
	g_debug << "-------------" << endl;

	int shaderCount = (int)vertexFilenames.size();

	// Load the shaders
	for (int i = 0; i < shaderCount; i++)
	{
		string shadername = vertexFilenames[i].substr(baselen, vertexFilenames[i].length()-baselen-extlen);
		g_debug << "shader #" << i << ": " << shadername << endl;

		// Read in shader files
		string vertexPath = vertexFilenames[i];
		string fragmentPath = fragmentFilenames[i];

		char *vertexSource = loadShaderSource(vertexPath);
		char *fragmentSource = loadShaderSource(fragmentPath);

		// Init shader and add it to the system
		if (vertexSource != 0 && fragmentSource != 0)
		{
			Shader *shader = new Shader();
			shader->init(shadername, vertexSource, fragmentSource);
			m_shaders[shadername] = shader;

			delete [] vertexSource;
			delete [] fragmentSource;
		}
		else
		{
			g_debug << "can't load " << shadername << ", not compiled, will not work, sky will fall etc" << endl;
		}
	}
	g_debug << "" << endl;
}

void ShaderManager::freeShaders()
{
	map<string, Shader*>::iterator it;
	for (it = m_shaders.begin(); it != m_shaders.end(); it++)
	{
		Shader* s = (*it).second;
		s->release();
		delete s;
	}
	m_shaders.clear();
}

char* ShaderManager::loadShaderSource(string& filename)
{
	ifstream file(filename.c_str(), ios::in|ios::binary|ios::ate);

	// Read in the shader file and pad it with zero
	if (file.is_open())
	{
		char *data = 0;
		int size = file.tellg();

		data = new char [size+1]; // One byte extra for the zero in the end
		file.seekg (0, ios::beg);
		file.read (data, size);
		file.close();
		data[size] = 0; // Add null terminator so OpenGL doesn't reject it
		return data;
	}
	else
	{
		g_debug << "Cannot open shader source file " << filename << endl;
		return 0;
	}
}
#if 1
// Initialize and load the stock shaders
bool ShaderManager::InitializeStockShaders(void)
{
	// Be warned, going over 128 shaders may cause a
	// hickup for a reallocation.
	//	shaderTable.reserve(128);

	ShaderAttributeList attrib_list;

	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_VERTEX, "vVertex"));

	m_stockShaders[GLT_SHADER_IDENTITY] = LoadShaderPairSrcWithAttributes(szIdentityShaderVP, szIdentityShaderFP, attrib_list);
	m_stockShaders[GLT_SHADER_FLAT] = LoadShaderPairSrcWithAttributes(szFlatShaderVP, szFlatShaderFP, attrib_list);

	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_COLOR, "vColor"));

	m_stockShaders[GLT_SHADER_SHADED] = LoadShaderPairSrcWithAttributes(szShadedVP, szShadedFP, attrib_list);

	attrib_list.pop_back();
	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_NORMAL, "vNormal"));

	m_stockShaders[GLT_SHADER_DEFAULT_LIGHT] = LoadShaderPairSrcWithAttributes(szDefaultLightVP, szDefaultLightFP, attrib_list);
	m_stockShaders[GLT_SHADER_POINT_LIGHT_DIFF] = LoadShaderPairSrcWithAttributes(szPointLightDiffVP, szPointLightDiffFP, attrib_list);

	attrib_list.pop_back();
	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_TEXTURE0, "vTexCoord0"));

	m_stockShaders[GLT_SHADER_TEXTURE_REPLACE]  = LoadShaderPairSrcWithAttributes(szTextureReplaceVP, szTextureReplaceFP, attrib_list);
	m_stockShaders[GLT_SHADER_TEXTURE_MODULATE] = LoadShaderPairSrcWithAttributes(szTextureModulateVP, szTextureModulateFP, attrib_list);

	attrib_list.pop_back();
	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_NORMAL, "vNormal"));
	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_TEXTURE0, "vTexCoord0"));

	m_stockShaders[GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF] = LoadShaderPairSrcWithAttributes(szTexturePointLightDiffVP, szTexturePointLightDiffFP, attrib_list);

	attrib_list.pop_back();
	attrib_list.pop_back();
	attrib_list.push_back(std::pair<int, const char*>(GLT_ATTRIBUTE_TEXTURE0, "vTexCoord0"));

	m_stockShaders[GLT_SHADER_TEXTURE_RECT_REPLACE] = LoadShaderPairSrcWithAttributes(szTextureRectReplaceVP, szTextureRectReplaceFP, attrib_list);

	if (m_stockShaders[0] != 0)
		return true;

	return false;
}

// Use a specific stock shader, and set the appropriate uniforms
GLint ShaderManager::UseStockShader(GLT_STOCK_SHADER nShaderID, const StockShaderUniforms& uniforms)
{
	// Check for out of bounds
	if (nShaderID >= GLT_SHADER_LAST)
		return -1;

	// Bind to the correct shader
	glUseProgram(m_stockShaders[nShaderID]);

	// Set up the uniforms
	GLint iTransform, iModelMatrix, iProjMatrix, iColor, iLight, iTextureUnit;

	switch(nShaderID) {
	case GLT_SHADER_FLAT:			// Just the modelview projection matrix and the color
		iTransform = glGetUniformLocation(m_stockShaders[nShaderID], "mvpMatrix");
		glUniformMatrix4fv(iTransform, 1, GL_FALSE, uniforms.mvpMatrix);

		iColor = glGetUniformLocation(m_stockShaders[nShaderID], "vColor");
		glUniform4fv(iColor, 1, uniforms.vColor);
		break;

	case GLT_SHADER_TEXTURE_RECT_REPLACE:
	case GLT_SHADER_TEXTURE_REPLACE:	// Just the texture place
		iTransform = glGetUniformLocation(m_stockShaders[nShaderID], "mvpMatrix");
		glUniformMatrix4fv(iTransform, 1, GL_FALSE, uniforms.mvpMatrix);

		iTextureUnit = glGetUniformLocation(m_stockShaders[nShaderID], "textureUnit0");
		glUniform1i(iTextureUnit, uniforms.textureUnit0);
		break;

	case GLT_SHADER_TEXTURE_MODULATE: // Multiply the texture by the geometry color
		iTransform = glGetUniformLocation(m_stockShaders[nShaderID], "mvpMatrix");
		glUniformMatrix4fv(iTransform, 1, GL_FALSE, uniforms.mvpMatrix);

		iColor = glGetUniformLocation(m_stockShaders[nShaderID], "vColor");
		glUniform4fv(iColor, 1, uniforms.vColor);

		iTextureUnit = glGetUniformLocation(m_stockShaders[nShaderID], "textureUnit0");
		glUniform1i(iTextureUnit, uniforms.textureUnit0);
		break;


	case GLT_SHADER_DEFAULT_LIGHT:
		iModelMatrix = glGetUniformLocation(m_stockShaders[nShaderID], "mvMatrix");
		glUniformMatrix4fv(iModelMatrix, 1, GL_FALSE, uniforms.mvMatrix);

		iProjMatrix = glGetUniformLocation(m_stockShaders[nShaderID], "pMatrix");
		glUniformMatrix4fv(iProjMatrix, 1, GL_FALSE, uniforms.pMatrix);

		iColor = glGetUniformLocation(m_stockShaders[nShaderID], "vColor");
		glUniform4fv(iColor, 1, uniforms.vColor);
		break;

	case GLT_SHADER_POINT_LIGHT_DIFF:
		iModelMatrix = glGetUniformLocation(m_stockShaders[nShaderID], "mvMatrix");
		glUniformMatrix4fv(iModelMatrix, 1, GL_FALSE, uniforms.mvMatrix);

		iProjMatrix = glGetUniformLocation(m_stockShaders[nShaderID], "pMatrix");
		glUniformMatrix4fv(iProjMatrix, 1, GL_FALSE, uniforms.pMatrix);

		iLight = glGetUniformLocation(m_stockShaders[nShaderID], "vLightPos");
		glUniform3fv(iLight, 1, uniforms.vLightPos);

		iColor = glGetUniformLocation(m_stockShaders[nShaderID], "vColor");
		glUniform4fv(iColor, 1, uniforms.vColor);
		break;

	case GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF:
		iModelMatrix = glGetUniformLocation(m_stockShaders[nShaderID], "mvMatrix");
		glUniformMatrix4fv(iModelMatrix, 1, GL_FALSE, uniforms.mvMatrix);

		iProjMatrix = glGetUniformLocation(m_stockShaders[nShaderID], "pMatrix");
		glUniformMatrix4fv(iProjMatrix, 1, GL_FALSE, uniforms.pMatrix);

		iLight = glGetUniformLocation(m_stockShaders[nShaderID], "vLightPos");
		glUniform3fv(iLight, 1, uniforms.vLightPos);

		iColor = glGetUniformLocation(m_stockShaders[nShaderID], "vColor");
		glUniform4fv(iColor, 1, uniforms.vColor);

		iTextureUnit = glGetUniformLocation(m_stockShaders[nShaderID], "textureUnit0");
		glUniform1i(iTextureUnit, uniforms.textureUnit0);
		break;


	case GLT_SHADER_SHADED:		// Just the modelview projection matrix. Color is an attribute
		iTransform = glGetUniformLocation(m_stockShaders[nShaderID], "mvpMatrix");
		glUniformMatrix4fv(iTransform, 1, GL_FALSE, uniforms.pMatrix);
		break;

	case GLT_SHADER_IDENTITY:	// Just the Color
		iColor = glGetUniformLocation(m_stockShaders[nShaderID], "vColor");
		glUniform4fv(iColor, 1, uniforms.vColor);
	default:
		break;
	}

	return m_stockShaders[nShaderID];
}


///////////////////////////////////////////////////////////////////////////////
// Lookup a stock shader
GLuint ShaderManager::GetStockShader(GLT_STOCK_SHADER nShaderID)
{
	if (nShaderID >= GLT_SHADER_LAST)
		return 0;

	return m_stockShaders[nShaderID];
}

// Load a pair of shaders, compile, and link together. Specify the complete
// source code text for each shader. Note, there is no support for
// just loading say a vertex program... you have to do both.
GLuint ShaderManager::LoadShaderPairSrcWithAttributes(const char *szVertexSrc, const char *szFragmentSrc, const ShaderAttributeList& attributeList)
{
	// Temporary Shader objects
	GLuint hVertexShader;
	GLuint hFragmentShader;
	GLuint hReturn = 0;
	GLint testVal;

	// Create shader objects
	hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load them.
	glShaderSource(hVertexShader, 1, &szVertexSrc, NULL);
	glShaderSource(hFragmentShader, 1, &szFragmentSrc, NULL);

	// Compile them
	glCompileShader(hVertexShader);
	glCompileShader(hFragmentShader);

	// Check for errors
	glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE)
	{
		glDeleteShader(hVertexShader);
		glDeleteShader(hFragmentShader);
		return (GLuint)NULL;
	}

	// Link them - assuming it works...
	hReturn = glCreateProgram();
	glAttachShader(hReturn, hVertexShader);
	glAttachShader(hReturn, hFragmentShader);


	const char *szNextArg;
	int iArgCount = attributeList.size();	// Number of attributes
	for (int i = 0; i < iArgCount; i++)
	{
		int index = attributeList[i].first;
		szNextArg = attributeList[i].second;
		glBindAttribLocation(hReturn, index, szNextArg);
	}


	glLinkProgram(hReturn);

	// These are no longer needed
	glDeleteShader(hVertexShader);
	glDeleteShader(hFragmentShader);

	// Make sure link worked too
	glGetProgramiv(hReturn, GL_LINK_STATUS, &testVal);
	if (testVal == GL_FALSE)
	{
		glDeleteProgram(hReturn);
		return (GLuint)NULL;
	}

	return hReturn;
}

#endif
