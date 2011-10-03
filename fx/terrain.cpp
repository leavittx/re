#include "terrain.h"
#include "core/system.h"
#include "core/time.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;
using namespace reutil;
using namespace std;

void TerrainScene::init()
{
	m_wireframe = false;
	m_points = false;
	m_texture = false;

	m_landscapeSize = 0.8f;
	m_step = 0.01f;
	m_turbOmega = 1;
	m_turbK = 1;
	m_texCoordK = 1.0f;

	SetTab2();
	SetColormap();

	UploadTerrainBatch();
	MakeCube(m_cubeBatch, 0.3f);

	InputManager::inst().acceptKeyboardEvents(this);

	m_projectionMatrix.LoadMatrix(Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()).ptr());
	m_transformPipeline.SetMatrixStacks(m_modelViewMatrix, m_projectionMatrix);

	float size = m_landscapeSize;
	float step = m_step;
	int count = 0;

	int count_my = (2 * size / step) * (2 * size / step);

	for (float j = -size; j < size; j += step)
	for (float i = -size; i < size; i += step, count++)
	{
//		g_debug << i << endl;
	}

	g_debug << "count: " << count << endl;
	g_debug << "my count: " << count_my << endl;
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

	// Save the current modelview matrix (the identity matrix)
	m_modelViewMatrix.PushMatrix();

	M3DMatrix44f mCamera;
	m_cameraFrame.GetCameraMatrix(mCamera);
	m_modelViewMatrix.PushMatrix(mCamera);

	Matrix4f mScale, mTranslate, mRotate, mModelview, mModelViewProjection;
	mScale = Matrix4f::Scaling(2.9f, 2.9f, 2.9f);
	mTranslate = Matrix4f::Translation(0.0f, 2.0f, -6.0f);
	mRotate = Matrix4f::RotationWithAxis(Vector3f(0.0f, 0.0f, 1.0f), 35.0f * Time::gets()*0.01f /*remath::deg2rad(30*sin(t::gets() * 10.0f))*/);
	mRotate = mRotate +  Matrix4f::RotationWithAxis(Vector3f(1.0f, 0.0f, 0.0f), deg2rad(-60.0f));
	mModelview = mScale * mRotate * mTranslate;
	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;

	ShaderManager::inst().UseStockShader(
				GLT_SHADER_SHADED,
				StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr()));

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NEVER, 0, 0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glFrontFace(GL_CW);

	glLineWidth(1.2f);
	glPointSize(8.0f);

	m_terrainBatch.Draw();

	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDisable(GL_STENCIL_TEST);


//	float sine = fabs(sin(t::gets()));
//	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
	GLfloat vRed[] = { 0.0f, 0.7f, 0.0f, 0.0f };
	ShaderManager::inst().UseStockShader(
				GLT_SHADER_FLAT,
				StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vRed));

//	ShaderManager::inst().UseStockShader(
//				GLT_SHADER_FLAT,
//				StockShaderUniforms(Matrix4f(m_transformPipeline.GetModelViewProjectionMatrix()).ptr(), vRed));
//	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT, StockShaderUniforms(mModelViewProjection.ptr(), vRed));

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

	m_cubeBatch.Draw();

	// Restore the previous modleview matrix (the identity matrix)
	m_modelViewMatrix.PopMatrix();
	m_modelViewMatrix.PopMatrix();
}

void TerrainScene::UploadTerrainBatch()
{
	int nVerts = //(2*m_landscapeSize/m_step) +
			(2*m_landscapeSize/m_step) * (2*m_landscapeSize/m_step) * 6 * 2;

	float *vVerts = new float[nVerts * 3];
	float *vCols = new float[nVerts * 4];

	int idxV = 0;
	int idxC = 0;
	for (float i = -m_landscapeSize; i < m_landscapeSize; i += m_step)
	{
		for (float j = -m_landscapeSize; j < m_landscapeSize; j += m_step, idxV += 18, idxC += 24)
		{
//			if (m_texture)
//				glTexCoord2d((i +  0.8) / 1.6 * m_texCoordK, (j +  0.8) / 1.6 * m_texCoordK);

			float h = noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega);
			Color3 col = get_color(h);

//			col = Color3((i +  m_landscapeSize) / (2 * m_landscapeSize),
//						 (j +  m_landscapeSize) / (2 * m_landscapeSize), 0.0f);

			////////////////////////////////////////////////////////////////////
			//		First triangle
			////////////////////////////////////////////////////////////////////

			// Save the color of 1st vertex
			vCols[idxC +  0] = col.r;
			vCols[idxC +  1] = col.g;
			vCols[idxC +  2] = col.b;
			vCols[idxC +  3] = 1.0f;
			// Save the color of 2nd vertex
			vCols[idxC +  4] = col.r;
			vCols[idxC +  5] = col.g;
			vCols[idxC +  6] = col.b;
			vCols[idxC +  7] = 1.0f;
			// Save the color of 3rd vertex
			vCols[idxC +  8] = col.r;
			vCols[idxC +  9] = col.g;
			vCols[idxC + 10] = col.b;
			vCols[idxC + 11] = 1.0f;

			// Save the first vertex
			vVerts[idxV +  0] = j;
			vVerts[idxV +  1] = i;
			vVerts[idxV +  2] = h;
			// Save the second vertex
			vVerts[idxV +  3] = j;
			vVerts[idxV +  4] = i +  m_step;
			vVerts[idxV +  5] = noise2d_turb((i +  m_step) * m_turbK, j * m_turbK, m_turbOmega);

			vVerts[idxV +  6] = j +  m_step;
			vVerts[idxV +  7] = i;
			vVerts[idxV +  8] = noise2d_turb(i * m_turbK, (j +  m_step) * m_turbK, m_turbOmega);

			////////////////////////////////////////////////////////////////////
			//		Second triangle
			////////////////////////////////////////////////////////////////////

			// Save the color of 1st vertex
			vCols[idxC + 12] = col.r;
			vCols[idxC + 13] = col.g;
			vCols[idxC + 14] = col.b;
			vCols[idxC + 15] = 1.0f;
			// Save the color of 2nd vertex
			vCols[idxC + 16] = col.r;
			vCols[idxC + 17] = col.g;
			vCols[idxC + 18] = col.b;
			vCols[idxC + 19] = 1.0f;
			// Save the color of 3rd vertex
			vCols[idxC + 20] = col.r;
			vCols[idxC + 21] = col.g;
			vCols[idxC + 22] = col.b;
			vCols[idxC + 23] = 1.0f;

			// Save the first vertex
			vVerts[idxV +  9] = j +  m_step;
			vVerts[idxV + 10] = i;
			vVerts[idxV + 11] = noise2d_turb(i * m_turbK, (j +  m_step) * m_turbK, m_turbOmega);
			// Save the second vertex
			vVerts[idxV + 12] = j +  m_step;
			vVerts[idxV + 13] = i +  m_step;
			vVerts[idxV + 14] = noise2d_turb((i +  m_step) * m_turbK, (j +  m_step) * m_turbK, m_turbOmega);

			vVerts[idxV + 15] = j;
			vVerts[idxV + 16] = i +  m_step;
			vVerts[idxV + 17] = noise2d_turb((i +  m_step) * m_turbK, j * m_turbK, m_turbOmega);


//			if (idxV +  5 >= nVerts * 3)
//			{
//				exit(0);
//			}
		}
	}

	g_debug << "i really needed " << idxV / 6 << " verticles, you counted " << nVerts / 2 << endl;

	// Triangle strip goes to hell!
//	m_terrainBatch.Begin(GL_TRIANGLE_STRIP, idxV / 3);
	m_terrainBatch.Begin(GL_TRIANGLES, idxV / 3);
	m_terrainBatch.CopyVertexData3f(vVerts);
	m_terrainBatch.CopyColorData4f(vCols);
	m_terrainBatch.End();

	delete [] vVerts;
	delete [] vCols;
}


void TerrainScene::handleKeyboardEvent(Key key)
{
	float linear = 0.1f;
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
//		m_turbK++;
//		UploadTerrainBatch();
	case KeyDown:		m_cameraFrame.MoveForward(-linear); break;
//		m_turbK--;
//		UploadTerrainBatch();
	case KeyRight:		m_cameraFrame.MoveRight(-linear); break;
//		if (m_turbOmega < 7)
//			m_turbOmega++;
//		UploadTerrainBatch();
	case KeyLeft:		m_cameraFrame.MoveRight(linear); break;
//		if (m_turbOmega > -1)
//			m_turbOmega--;
//		UploadTerrainBatch();
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
			m_landscapeSize -= 0.1;
		UploadTerrainBatch();
		break;
	}
	case KeyRCtrl:
	{
		m_landscapeSize += 0.1;
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
//	return Color3(1 * h, 1 * h, 1 * h);

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
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
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


// Make a cube, centered at the origin, and with a specified "radius"
void TerrainScene::MakeCube(GLBatch& cubeBatch, GLfloat fRadius)
{
	cubeBatch.Begin(GL_TRIANGLES, 36, 1);

	/////////////////////////////////////////////
	// Top of cube
	cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, fRadius);


	////////////////////////////////////////////
	// Bottom of cube
	cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, fRadius);

	///////////////////////////////////////////
	// Left side of cube
	cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, fRadius);

	// Right side of cube
	cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, fRadius);

	cubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, -fRadius);

	// Front and Back
	// Front
	cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, fRadius);

	// Back
	cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	cubeBatch.Vertex3f(-fRadius, -fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
	cubeBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	cubeBatch.Vertex3f(-fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	cubeBatch.Vertex3f(fRadius, fRadius, -fRadius);

	cubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
	cubeBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	cubeBatch.Vertex3f(fRadius, -fRadius, -fRadius);
	cubeBatch.End();
}
