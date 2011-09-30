#include "terrain.h"
#include "core/system.h"
#include "core/time.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

void TerrainScene::init()
{
	m_wireframe = false;
	m_texture = false;

	m_landscapeSize = 5.0f;
	m_step = 0.04f;
//	m_turbOmega = 6;
//	m_turbK = 3;
	m_turbOmega = 2;
	m_turbK = 1;
	m_texCoordK = 1.0f;

	SetTab1();
	SetTab2();
	SetColormap();

	UploadTerrainBatch();
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Matrix4f mScale, mTranslate, mRotate, mModelview, mModelViewProjection;
	mScale = Matrix4f::Scaling(2.9f, 2.9f, 2.9f);
	mTranslate = Matrix4f::Translation(0.0f, 2.0f, -6.0f);
	mRotate = Matrix4f::RotationWithAxis(Vector3f(0.0f, 0.0f, 1.0f), 35.0f * Time::gets()*0.1f /*remath::deg2rad(30*sin(t::gets() * 10.0f))*/);
	mRotate = mRotate + Matrix4f::RotationWithAxis(Vector3f(1.0f, 0.0f, 0.0f), deg2rad(-60.0f));
	mModelview = mScale * mRotate * mTranslate;
	mModelViewProjection = Matrix4f(gl::m_viewFrustum.GetProjectionMatrix()) * mModelview;

	float sine = fabs(sin(t::gets()));
	GLfloat vRed[] = { sine*1.0f, 1-sine*1.0f, 1-sine*0.5f, 1.0f };
	ShaderManager::inst().UseStockShader(GLT_SHADER_FLAT, StockShaderUniforms(mModelViewProjection.ptr(), vRed));
	m_triangleBatch.Draw();
}

void TerrainScene::UploadTerrainBatch()
{
	int nVerts = (2 * m_landscapeSize / m_step)*(2 * m_landscapeSize / m_step)*2 + 1212;

//	float *vVerts = (float *)malloc(sizeof(float) * nVerts * 3);
	float *vVerts = new float[nVerts * 3];

	int idx = 0;
	for (float i = -m_landscapeSize; i < m_landscapeSize; i += m_step)
	{
		for (float j = -m_landscapeSize; j < m_landscapeSize; j += m_step, idx += 6)
		{
//			if (m_texture)
//				glTexCoord2d((i + 0.8) / 1.6 * m_texCoordK, (j + 0.8) / 1.6 * m_texCoordK);

			float h = noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega);
			Color3 col = get_color(h);
//			glColor3d(col.r / 255.0, col.g / 255.0, col.b / 255.0);

			vVerts[idx + 0] = j;
			vVerts[idx + 1] = i;
			vVerts[idx + 2] = h;

			vVerts[idx + 3] = j;
			vVerts[idx + 4] = (i + m_step);
			vVerts[idx + 5] = noise2d_turb((i + m_step) * m_turbK, j * m_turbK, m_turbOmega);

//			if (idx + 5 >= nVerts * 3)
//			{
//				exit(0);
//			}

//			glVertex3d(j, i, h);
//			glVertex3d(j, (i + m_step), noise2d_turb((i + m_step) * m_turbK, j * m_turbK, m_turbOmega));
		}
	}

	m_triangleBatch.Begin(GL_TRIANGLE_STRIP, nVerts);
	m_triangleBatch.CopyVertexData3f(vVerts);
	m_triangleBatch.End();

//	free(vVerts);
	delete [] vVerts;
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
	int i;
	for (i = 0; m_colormap[i].n < h && i <= 3; i++);

	float fx = h - m_colormap[i - 1].n;

	Color3 color;
	color.r = (unsigned char)255 * ((m_colormap[i - 1].col.r * fx + m_colormap[i].col.r * (1 - fx)) / (m_colormap[i - 1].col.r + m_colormap[i].col.r));
	color.g = (unsigned char)255 * ((m_colormap[i - 1].col.g * fx + m_colormap[i].col.g * (1 - fx)) / (m_colormap[i - 1].col.g + m_colormap[i].col.g));
	color.b = (unsigned char)255 * ((m_colormap[i - 1].col.b * fx + m_colormap[i].col.b * (1 - fx)) / (m_colormap[i - 1].col.b + m_colormap[i].col.b));

	return color;
}

void TerrainScene::SetTab1()
{
	int i;
	srand((unsigned int)time(NULL));
	for (i = 0; i < SIZE; i ++)
		Tab1[i] = (float)rand() / RAND_MAX;
}

void TerrainScene::SetTab2()
{
	int i, j;
	for (i = 0; i < SIZE; i ++)
		for (j = 0; j < SIZE; j ++)
			Tab2[i][j] = (float)rand() / RAND_MAX;
}

float TerrainScene::fmod1(float t, float x)
{
	if (t < 0)
		return 1 + fmod(t, x);
	return fmod(t, x);
}

float TerrainScene::noise(float t)
{
	int ix = (int)floor(t) & MASK;
	float a, b, c, fx = fmod1(t, 1);

	a = Tab1[ix];
	b = Tab1[(ix + 1) & MASK];
	c = a * (1 - fx) + b * fx;
	return c;
}

float TerrainScene::noise1d(float t)
{
	int ix = (int)floor(t) & MASK;
	float a, b, c, fx = fmod1(t, 1);

	a = Tab1[ix];
	b = Tab1[(ix + 1) & MASK];
	fx = fx * fx * (3 - 2 * fx);
	c = a * (1 - fx) + b * fx;
	return c;
}

float TerrainScene::noise2d(float t1, float t2)
{
	int ix = (int)floor(t1) & MASK, iy = (int)floor(t2) & MASK;
	float a, b, A, B, fx = fmod1(t1, 1), fy = fmod1(t2, 1);

	fx = fx * fx * (3 - 2 * fx);
	fy = fy * fy * (3 - 2 * fy);
	a = Tab2[ix][iy];
	b = Tab2[(ix + 1) & MASK][iy];
	A = a * (1 - fx) + b * fx;
	a = Tab2[ix][(iy + 1) & MASK];
	b = Tab2[(ix + 1) & MASK][(iy + 1) & MASK];
	B = a * (1 - fx) + b * fx;
	return A * (1 - fy) + B * fy;
}

float TerrainScene::noise1d_turb(float t, int omega)
{
	int i;
	float turb, e;
	for (i = 0, turb = 0; i < omega; i ++)
	{
		e = pow((float)2, i);
		turb += noise1d(t * e) / e;
	}
	return turb;
}

float TerrainScene::noise2d_turb(float t1, float t2, int omega)
{
	int i;
	float turb, e;
	for (i = 0, turb = 0; i < omega; i ++)
	{
		e = pow((float)2, i);
		turb += noise2d(t1 * e, t2 * e) / e;
	}
	return turb;
}

float TerrainScene::bezier(float x)
{
	int ix = (int)floor(x) & MASK;
	float fx = fmod1(x, 1);
	return Tab1[ix - 1] * (- fx * fx * fx + 3 * fx * fx - 3 * fx + 1) +
		   Tab1[ix] * (3 * fx * fx * fx - 6 * fx * fx + 4) +
		   Tab1[ix + 1] * (-3 * fx * fx * fx + 3 * fx * fx + 3 * fx + 1)+
		   Tab1[ix + 2] * fx * fx * fx;
}
