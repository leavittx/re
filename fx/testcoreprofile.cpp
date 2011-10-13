#include "testcoreprofile.h"
#include "core/time.h"
#include "render/texturemanager.h"
#include "render/shadermanager.h"
#include "util/tga.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

static GLBatch triangleBatch;
static GLBatch cubeBatch;

static GLFrame m_cameraFrame;
static GLMatrixStack m_modelViewMatrix;
static GLMatrixStack m_projectionMatrix;
static GLGeometryTransform m_transformPipeline;

static bool m_wireframe, m_points;

static unsigned int textureID;

// Lighting data
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat lightSpecular[] = { 0.9f, 0.9f, 0.9f };
GLfloat vLightPos[] = { -8.0f, 20.0f, 100.0f, 1.0f };

void TestCoreProfileScene::init()
{
	// Load up a triangle
	GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
						  0.5f, 0.0f, 0.0f,
						  0.0f, 0.5f, 0.0f };

	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(vVerts);
	triangleBatch.End();

	MakeCube(cubeBatch, 1.0f);

	m_projectionMatrix.LoadMatrix(Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()).ptr());
	m_transformPipeline.SetMatrixStacks(m_modelViewMatrix, m_projectionMatrix);

	InputManager::inst().acceptKeyboardEvents(this);

	// Test texturing
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	reutil::LoadTGATexture("data/graphics/wall.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void TestCoreProfileScene::release()
{
}

void TestCoreProfileScene::update()
{
}

void TestCoreProfileScene::draw()
{
	gl::clear(gl::ALL);

	// Save the current modelview matrix (the identity matrix)
	m_modelViewMatrix.PushMatrix();

	M3DMatrix44f mCamera;
	m_cameraFrame.GetCameraMatrix(mCamera);
	m_modelViewMatrix.PushMatrix(mCamera);

	m_modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);

	Matrix4f mTranslate, mRotate, mModelview, mModelViewProjection;
	mTranslate = Matrix4f::Translation(0.0f, 0.0f, -2.5f);
	mRotate = Matrix4f::RotationWithAxis(Vector3f(1.0f, 1.0f, -0.5f), remath::deg2rad(30*sin(t::gets() * 10.0f)));
	mModelview = mTranslate * mRotate;
	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;

	float sine = fabs(sin(t::gets()));
	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
	GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
//		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vRed));

//	triangleBatch.Draw();


//	ShaderManager::inst().UseStockShader(GLT_SHADER_SHADED,
//		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr()));

//	ShaderManager::inst().UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
//		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewMatrix()).ptr(),
//							Matrix4f(m_transformPipeline.GetProjectionMatrix()).ptr(),
//							vLightPos, vRed));

//	ShaderManager::inst().UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
//		StockShaderUniforms(mModelview.ptr(),
//							Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()).ptr(),
//							vLightPos, vWhite, 0));



//	TextureManager::inst().bindTexture("terrain-heightmap.png");
//	TextureManager::inst().bindTexture("terrain-texture.jpg");

//	TextureManager::inst().bindTexture("color_tile.png");
//	TextureManager::inst().bindTexture("rblur000.png");
//	TextureManager::inst().bindTexture("tex1.png");
//	TextureManager::inst().bindTexture("bmtlogo.png");

//	TextureManager::inst().bindTexture("env.tga");

	glEnable(GL_CULL_FACE);
//	gl::setCullMode(gl::FILL, gl::FILL);

//	glBindTexture(GL_TEXTURE_2D, textureID);

	ShaderManager::inst().UseStockShader(GLT_SHADER_TEXTURE_REPLACE,
		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), 0));

//	ShaderManager::inst().UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
//		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewMatrix()).ptr(),
//							Matrix4f(m_transformPipeline.GetProjectionMatrix()).ptr(),
//							vLightPos, vWhite, 0));

	cubeBatch.Draw();

	// Restore the previous modleview matrix (the identity matrix)
	m_modelViewMatrix.PopMatrix();
	m_modelViewMatrix.PopMatrix();
}

void TestCoreProfileScene::handleKeyboardEvent(Key key)
{
	float linear = 0.5f;
	float angular = deg2rad(10.0f);

	switch (key) {
	case KeyTab:
	{
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	case KeyCapsLock:
	{
		m_points = !m_points;
		if (m_points)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	case KeyUp:			m_cameraFrame.MoveForward(linear); break;
	case KeyDown:		m_cameraFrame.MoveForward(-linear); break;
	case KeyRight:		m_cameraFrame.MoveRight(-linear); break;
	case KeyLeft:		m_cameraFrame.MoveRight(linear); break;
	case KeyPageUp:		m_cameraFrame.MoveUp(linear); break;
	case KeyPageDown:	m_cameraFrame.MoveUp(-linear); break;
	case KeyW:			m_cameraFrame.RotateWorld( angular, 0.0f, 1.0f, 0.0f); break;
	case KeyS:			m_cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f); break;
	case KeyA:			m_cameraFrame.RotateWorld( angular, 0.0f, 0.0f, 1.0f); break;
	case KeyD:			m_cameraFrame.RotateWorld(-angular, 0.0f, 0.0f, 1.0f); break;
	case KeyQ:			m_cameraFrame.RotateWorld( angular, 1.0f, 0.0f, 0.0f); break;
	case KeyE:			m_cameraFrame.RotateWorld(-angular, 1.0f, 0.0f, 0.0f); break;
	default: return;
	}
}
