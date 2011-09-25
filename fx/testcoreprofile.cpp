#include "testcoreprofile.h"
#include "core/system.h"
#include "core/time.h"

#include "render/GLTools/GLTools.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

GLBatch	triangleBatch;
GLShaderManager	shaderManager;

void TestCoreProfileScene::init()
{
	shaderManager.InitializeStockShaders();

	// Load up a triangle
	GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
						  0.5f, 0.0f, 0.0f,
						  0.0f, 0.5f, 0.0f };

	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(vVerts);
	triangleBatch.End();
}

void TestCoreProfileScene::release()
{
}

void TestCoreProfileScene::update()
{
}

void TestCoreProfileScene::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	float sine = fabs(sin(t::gets()));
	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
	triangleBatch.Draw();
}
