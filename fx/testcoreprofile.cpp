#include "testcoreprofile.h"
#include "core/system.h"
#include "core/time.h"

//#include "render/GLTools/GLTools.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

static GLBatch triangleBatch;
//static GLShaderManager shaderManager;

void TestCoreProfileScene::init()
{
//	shaderManager.InitializeStockShaders();

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

//	// Matrix Variables
//	M3DMatrix44f mTranslate, mRotate, mModelview, mModelViewProjection;

//	// Create a translation matrix to move the torus back and into sight
//	m3dTranslationMatrix44(mTranslate, 0.0f, 0.0f, -2.5f);

//	// Create a rotation matrix based on the current value of yRot
//	m3dRotationMatrix44(mRotate, m3dDegToRad(t::gets() * 200.0f), 1.0f, 1.0f, -0.5f);

//	// Add the rotation to the translation, store the result in mModelView
//	m3dMatrixMultiply44(mModelview, mTranslate, mRotate);

//	// Add the modelview matrix to the projection matrix,
//	// the final matrix is the ModelViewProjection matrix.
//	m3dMatrixMultiply44(mModelViewProjection, gl::m_viewFrustum.GetProjectionMatrix(), mModelview);


	Matrix4f mTranslate, mRotate, mModelview, mModelViewProjection;
	mTranslate = Matrix4f::Translation(0.0f, 0.0f, -2.5f);
	mRotate = Matrix4f::RotationWithAxis(Vector3f(1.0f, 1.0f, -0.5f), remath::deg2rad(30*sin(t::gets() * 10.0f)));
	mModelview = mTranslate * mRotate;
	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;


//	M3DMatrix44f mTranslate, mRotate;
//	Matrix4f mModelview, mModelViewProjection;

//	m3dTranslationMatrix44(mTranslate, 0.0f, 0.0f, -2.5f);
//	m3dRotationMatrix44(mRotate, m3dDegToRad(t::gets() * 200.0f), 1.0f, 1.0f, -0.5f);

//	mModelview = Matrix4f(mTranslate) * Matrix4f(mRotate);
//	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;

//	mModelViewProjection.print();

	float sine = fabs(sin(t::gets()));
	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
//	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
//	shaderManager.UseStockShader(GLT_SHADER_FLAT, mModelViewProjection.ptr(), vRed);
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT, StockShaderUniforms(mModelViewProjection.ptr(), vRed));
	//606/180
	//640/180
//	for (int i = 0; i < 1000; i++)
	triangleBatch.Draw();
}
