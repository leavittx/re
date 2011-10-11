#include "terrain.h"
#include "core/system.h"
#include "core/time.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;
using namespace reutil;
using namespace std;

GLTriangleBatch		torusBatch;
GLBatch				floorBatch;
GLTriangleBatch     sphereBatch;

void TerrainScene::init()
{
	m_wireframe = false;
	m_points = false;
	m_texture = false;

	m_landscapeSize = 10.0f;
	m_step = 0.05f;
	m_turbOmega = 3;
	m_turbK = 1;
	m_texCoordK = 1.0f;

	SetTab2();
	SetColormap();

	UploadTerrainBatch();

	MakeCube(m_cubeBatch, 0.3f);

	// This makes a torus
	MakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);

	// This make a sphere
	MakeSphere(sphereBatch, 0.1f, 26, 13);

	floorBatch.Begin(GL_LINES, 324);
	for(GLfloat x = -20.0; x <= 20.0f; x+= 0.5) {
		floorBatch.Vertex3f(x, -0.55f, 20.0f);
		floorBatch.Vertex3f(x, -0.55f, -20.0f);

		floorBatch.Vertex3f(20.0f, -0.55f, x);
		floorBatch.Vertex3f(-20.0f, -0.55f, x);
	}
	floorBatch.End();

	m_projectionMatrix.LoadMatrix(Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()).ptr());
	m_transformPipeline.SetMatrixStacks(m_modelViewMatrix, m_projectionMatrix);

	InputManager::inst().acceptKeyboardEvents(this);
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

#if 1
	// Save the current modelview matrix (the identity matrix)
	m_modelViewMatrix.PushMatrix();

	M3DMatrix44f mCamera;
	m_cameraFrame.GetCameraMatrix(mCamera);
	m_modelViewMatrix.PushMatrix(mCamera);

//	Matrix4f mScale, mTranslate, mRotate, mModelview, mModelViewProjection;
//	mScale = Matrix4f::Scaling(2.9f, 2.9f, 2.9f);
//	mTranslate = Matrix4f::Translation(0.0f, 2.0f, -6.0f);
//	mRotate = Matrix4f::RotationWithAxis(Vector3f(0.0f, 0.0f, 1.0f), 35.0f * Time::gets()*0.01f /*remath::deg2rad(30*sin(t::gets() * 10.0f))*/);
//	mRotate = mRotate +  Matrix4f::RotationWithAxis(Vector3f(1.0f, 0.0f, 0.0f), deg2rad(-60.0f));
//	mModelview = mScale * mRotate * mTranslate;
//	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;


	m_modelViewMatrix.Rotate(70.0f, 1.0f, 0.0f, 0.0f);
	m_modelViewMatrix.Translate(0.0f, -15.0f, -5.0f);


	m_modelViewMatrix.PushMatrix();

		m_modelViewMatrix.Translate(0.0f, 2.0f, -2.5f);
		m_modelViewMatrix.Rotate(180.0f, 1.0f, 0.0f, 0.0f);

		ShaderManager::inst().UseStockShader(GLT_SHADER_SHADED,
			StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr()));

		glEnable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NEVER, 0, 0);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);
		glFrontFace(GL_CW);

		glLineWidth(1.2f);
		glPointSize(1.0f);

		m_terrainBatch.Draw();

		glFrontFace(GL_CCW);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_STENCIL_TEST);

	m_modelViewMatrix.PopMatrix();


	//	float sine = fabs(sin(t::gets()));
	//	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
	static GLfloat vRed[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	static GLfloat vFloorColor[] = { 0.0f, 1.0f, 0.0f, 1.0f};

	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vRed));

//	ShaderManager::inst().UseStockShader(GLT_SHADER_DEFAULT_LIGHT,
//		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewMatrix()).ptr(),
//							Matrix4f(m_transformPipeline.GetProjectionMatrix()).ptr(), vRed));

//	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
//		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vRed));
//	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
//		StockShaderUniforms(mModelViewProjection.ptr(), vRed));

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glEnable(GL_STENCIL_TEST);
//	glStencilFunc(GL_NEVER, 0, 0);
//	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
//	glFrontFace(GL_CW);
//	m_cubeBatch.Draw();
//	glFrontFace(GL_CCW);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//	glDisable(GL_STENCIL_TEST);

//	glEnable(GL_BLEND);
//	glEnable(GL_LINE_SMOOTH);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//	m_cubeBatch.Draw();


	// Draw the ground
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
		StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vFloorColor));

//	floorBatch.Draw();

	// Restore the previous modleview matrix (the identity matrix)
	m_modelViewMatrix.PopMatrix();
	m_modelViewMatrix.PopMatrix();
#else

	// Color values
	static GLfloat vFloorColor[] = { 0.0f, 1.0f, 0.0f, 1.0f};
	static GLfloat vTorusColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat vSphereColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	// Time Based animation
	float yRot = t::gets() * 60.0f;

	// Save the current modelview matrix (the identity matrix)
	m_modelViewMatrix.PushMatrix();

//	M3DMatrix44f mCamera;
	m_cameraFrame.GetCameraMatrix(mCamera);
	m_modelViewMatrix.PushMatrix(mCamera);

	// Draw the ground
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
										 StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vFloorColor));
	floorBatch.Draw();

	// Draw the spinning Torus
	m_modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);

	// Save the Translation
	m_modelViewMatrix.PushMatrix();

	// Apply a rotation and draw the torus
	m_modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
										 StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vTorusColor));
	torusBatch.Draw();
	m_modelViewMatrix.PopMatrix(); // "Erase" the Rotation from before

	// Apply another rotation, followed by a translation, then draw the sphere
	m_modelViewMatrix.Rotate(yRot * -2.0f, 0.0f, 1.0f, 0.0f);
	m_modelViewMatrix.Translate(0.8f, 0.0f, 0.0f);
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT,
										 StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vSphereColor));
	sphereBatch.Draw();

	// Restore the previous modleview matrix (the identity matrix)
	m_modelViewMatrix.PopMatrix();
	m_modelViewMatrix.PopMatrix();
#endif
}

void TerrainScene::UploadTerrainBatch()
{
	int nVerts = (2*m_landscapeSize/m_step) * (2*m_landscapeSize/m_step) * 6 * 2;

	float *vVerts = new float[nVerts * 3];
	float *vCols = new float[nVerts * 4];

	int idxV = 0;
	int idxC = 0;
	for (float i = -m_landscapeSize; i < m_landscapeSize; i += m_step)
	{
		for (float j = -m_landscapeSize; j < m_landscapeSize; j += m_step, idxV += 18, idxC += 24)
		{
//			float h = noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega);
//			Color3 col = get_color(h);

//			col = Color3((i +  m_landscapeSize) / (2 * m_landscapeSize),
//						 (j +  m_landscapeSize) / (2 * m_landscapeSize), 0.0f);

			float h1, h2, h3;
			Color3 col1, col2, col3;

			////////////////////////////////////////////////////////////////////
			//		First triangle
			////////////////////////////////////////////////////////////////////

			h1 = noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega);
			h2 = noise2d_turb((i +  m_step) * m_turbK, j * m_turbK, m_turbOmega);
			h3 = noise2d_turb(i * m_turbK, (j +  m_step) * m_turbK, m_turbOmega);

			col1 = get_color(h1);
			col2 = get_color(h2);
			col3 = get_color(h3);

			// Save the color of 1st vertex
			vCols[idxC +  0] = col1.r;
			vCols[idxC +  1] = col1.g;
			vCols[idxC +  2] = col1.b;
			vCols[idxC +  3] = 1.0f;
			// Save the color of 2nd vertex
			vCols[idxC +  4] = col2.r;
			vCols[idxC +  5] = col2.g;
			vCols[idxC +  6] = col2.b;
			vCols[idxC +  7] = 1.0f;
			// Save the color of 3rd vertex
			vCols[idxC +  8] = col3.r;
			vCols[idxC +  9] = col3.g;
			vCols[idxC + 10] = col3.b;
			vCols[idxC + 11] = 1.0f;

			// Save the first vertex
			vVerts[idxV +  0] = j;
			vVerts[idxV +  1] = h1;
			vVerts[idxV +  2] = i;
			// Save the second vertex
			vVerts[idxV +  3] = j;
			vVerts[idxV +  4] = h2;
			vVerts[idxV +  5] = i +  m_step;
			// Save the third vertex
			vVerts[idxV +  6] = j +  m_step;
			vVerts[idxV +  7] = h3;
			vVerts[idxV +  8] = i;

			////////////////////////////////////////////////////////////////////
			//		Second triangle
			////////////////////////////////////////////////////////////////////

			h1 = noise2d_turb(i * m_turbK, (j +  m_step) * m_turbK, m_turbOmega);
			h2 = noise2d_turb((i +  m_step) * m_turbK, j * m_turbK, m_turbOmega);
			h3 = noise2d_turb((i +  m_step) * m_turbK, (j +  m_step) * m_turbK, m_turbOmega);

			col1 = get_color(h1);
			col2 = get_color(h2);
			col3 = get_color(h3);

			// Save the color of 1st vertex
			vCols[idxC + 12] = col1.r;
			vCols[idxC + 13] = col1.g;
			vCols[idxC + 14] = col1.b;
			vCols[idxC + 15] = 1.0f;
			// Save the color of 2nd vertex
			vCols[idxC + 16] = col2.r;
			vCols[idxC + 17] = col2.g;
			vCols[idxC + 18] = col2.b;
			vCols[idxC + 19] = 1.0f;
			// Save the color of 3rd vertex
			vCols[idxC + 20] = col3.r;
			vCols[idxC + 21] = col3.g;
			vCols[idxC + 22] = col3.b;
			vCols[idxC + 23] = 1.0f;

			// Save the first vertex
			vVerts[idxV +  9] = j +  m_step;
			vVerts[idxV + 10] = h1;
			vVerts[idxV + 11] = i;
			// Save the second vertex
			vVerts[idxV + 12] = j;
			vVerts[idxV + 13] = h2;
			vVerts[idxV + 14] = i + m_step;
			// Save the third vertex
			vVerts[idxV + 15] = j +  m_step;
			vVerts[idxV + 16] = h3;
			vVerts[idxV + 17] = i +  m_step;
		}
	}

	m_terrainBatch.Begin(GL_TRIANGLES, idxV / 3);
	m_terrainBatch.CopyVertexData3f(vVerts);
	m_terrainBatch.CopyColorData4f(vCols);
	m_terrainBatch.End();

	delete [] vVerts;
	delete [] vCols;
}


void TerrainScene::handleKeyboardEvent(Key key)
{
	float linear = 0.5f;
	float angular = deg2rad(5.0f);

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
	case KeySpace:		m_texture = !m_texture; break;
	case KeyLShift:
	{
		if (m_step < 0.2)
			m_step += 0.01;
		UploadTerrainBatch();
		break;
	}
	case KeyRShift:
	{
		if (m_step > 0.005)
			m_step -= 0.005;
		UploadTerrainBatch();
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
	case KeyLCtrl:
	{
		if (m_landscapeSize > 0)
			m_landscapeSize -= 0.5;
		UploadTerrainBatch();
		break;
	}
	case KeyRCtrl:
	{
		m_landscapeSize += 0.5;
		UploadTerrainBatch();
		break;
	}
	case Key1:
	{
		m_turbK++;
		UploadTerrainBatch();
		break;
	}
	case Key2:
	{
		m_turbK--;
		UploadTerrainBatch();
		break;
	}
	case Key3:
	{
		if (m_turbOmega < 7)
			m_turbOmega++;
		UploadTerrainBatch();
		break;
	}
	case Key4:
	{
		if (m_turbOmega > -1)
			m_turbOmega--;
		UploadTerrainBatch();
		break;
	}
	default: return;
	}
}

void TerrainScene::SetColormap()
{
	m_colormap[0].n = 0.3;
	m_colormap[0].col = Color3( 19, 117, 207);
	m_colormap[1].n = 0.4;
	m_colormap[1].col = Color3(  9, 201,  22);
	m_colormap[2].n = 0.5;
	m_colormap[2].col = Color3(133, 106,  42);
	m_colormap[3].n = 0.6;
	m_colormap[3].col = Color3(181, 101, 211);
}

remath::Color3 TerrainScene::get_color(float h)
{
	return Color3(1 * h, 1 * h, 1 * h);

	int i;
	for (i = 0; m_colormap[i].n < h && i <= 3; i++);

	float fx = h - m_colormap[i - 1].n;

	Color3 color;
	color.r = ((m_colormap[i - 1].col.r * fx +  m_colormap[i].col.r * (1 - fx)) / (m_colormap[i - 1].col.r +  m_colormap[i].col.r));
	color.g = ((m_colormap[i - 1].col.g * fx +  m_colormap[i].col.g * (1 - fx)) / (m_colormap[i - 1].col.g +  m_colormap[i].col.g));
	color.b = ((m_colormap[i - 1].col.b * fx +  m_colormap[i].col.b * (1 - fx)) / (m_colormap[i - 1].col.b +  m_colormap[i].col.b));

	return color;
}

void TerrainScene::SetTab2()
{
	srand(time(NULL));

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			Tab2[i][j] = (float)rand() / RAND_MAX;
}

float TerrainScene::fmod1(float t, float x)
{
	if (t < 0)
		return 1 +  fmod(t, x);
	return fmod(t, x);
}

float TerrainScene::noise2d(float t1, float t2)
{
	int ix = (int)floor(t1) & MASK, iy = (int)floor(t2) & MASK;
	float a, b, A, B, fx = fmod1(t1, 1), fy = fmod1(t2, 1);

	fx = fx * fx * (3 - 2 * fx);
	fy = fy * fy * (3 - 2 * fy);
	a = Tab2[ix][iy];
	b = Tab2[(ix +  1) & MASK][iy];
	A = a * (1 - fx) +  b * fx;
	a = Tab2[ix][(iy +  1) & MASK];
	b = Tab2[(ix +  1) & MASK][(iy +  1) & MASK];
	B = a * (1 - fx) +  b * fx;
	return A * (1 - fy) +  B * fy;
}

float TerrainScene::noise2d_turb(float t1, float t2, int omega)
{
	int i;
	float turb, e;

	for (i = 0, turb = 0; i < omega; i++)
	{
		e = pow((float)2, i);
		turb += noise2d(t1 * e, t2 * e) / e;
	}
	return turb;
}
