#include "terrain.h"
#include "core/system.h"
#include "core/time.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

static GLBatch triangleBatch;

void TerrainScene::init()
{
	// Load up a triangle
	GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
						  0.5f, 0.0f, 0.0f,
						  0.0f, 0.5f, 0.0f };

	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(vVerts);
	triangleBatch.End();
}

void TerrainScene::release()
{
}

void TerrainScene::update()
{
}

void TerrainScene::draw()
{
	gl::clear(gl::ALL);

	Matrix4f mTranslate, mRotate, mModelview, mModelViewProjection;
	mTranslate = Matrix4f::Translation(0.0f, 0.0f, -2.5f);
	mRotate = Matrix4f::RotationWithAxis(Vector3f(1.0f, 1.0f, -0.5f), remath::deg2rad(30*sin(t::gets() * 10.0f)));
	mModelview = mTranslate * mRotate;
	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;

	float sine = fabs(sin(t::gets()));
	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
//	ShaderManager::inst().UseStockShader(GLT_SHADER_IDENTITY, StockShaderUniforms(vRed));
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT, StockShaderUniforms(mModelViewProjection.ptr(), vRed));
	triangleBatch.Draw();
}
