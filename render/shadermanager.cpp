#include "shadermanager.h"
#include "util/stringutils.h"

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
