#ifdef __DEPRECATED_PROFILE__

#include "noisy.h"
#include "core/time.h"
#include "render/texturemanager.h"

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

void NoisyScene::init()
{
	m_wireframe = false;
	m_texture = false;

	m_landscapeSize = 2.0f;
	m_step = 0.04f;
	m_turbOmega = 6;//2;
	m_turbK = 3;//1;
	m_texCoordK = 1.0f;

	m_nStars = 20000;
	m_starsSpeed = 1.0f;
	m_stars.resize(m_nStars);
	for (int i = 0; i < m_nStars; i++)
		generateNewStar(i);

	SetTab1();
	SetTab2();
	SetColormap();

	InputManager::inst().acceptKeyboardEvents(this);
}

void NoisyScene::release()
{
}

void NoisyScene::update()
{
}

void NoisyScene::draw()
{
	gl::frustum(2.0f, 1.0f, 150.0f);

//	gl::clearColor(Color4((GLclampf)62 / 255, (GLclampf)92 / 255, (GLclampf)24 / 255, 1.0f));
//	gl::clearColor(Color4(0.0f, 0.4f, 1.0f, 1.0f));
	gl::clearColor(Color4(0.0f, 0.0f, 0.0f, 1.0f));
//	static bool flag = 1;
//	if (flag) {
	gl::clear(gl::ALL);
//	flag = false;
//	}

	// Stars
	gl::pointSize(5.0f);
//	gl::color(Color3(1.0f, 0.0f, 1.0f));
//	gl::pushMatrix();
//	glBegin(GL_POINTS);
//	for (int i = 0; i < (int)m_stars.size(); i++)
//	{
//		glVertex3f(m_stars[i].x, m_stars[i].y, m_stars[i].z);
//		m_stars[i].z += m_starsSpeed;
//		if (m_stars[i].z >= 10)
//			generateNewStar(i);
//	}
//	glEnd();
//	gl::popMatrix();

	// 1-dimensional noise
//	gl::pushMatrix();
//	gl::translate(0.0f, 0.0f, -6.0f);
//	gl::rotate(100.0f, 1.0f, 0.0f, 0.0f);
//	gl::rotate(50.0f * Time::gets(), 0.0f, 0.0f, 1.0f);
//	gl::scale(3.0f, 3.0f, 3.0f);
//	draw_1d_noise(Time::gets() * 10);
//	gl::popMatrix();

	// 2-dimensional noise
	gl::pushMatrix();
	gl::translate(0.0f, 2.0f, -6.0f);
	gl::rotate(130.0f, 1.0f, 0.0f, 0.0f);
	gl::rotate(35.0f * Time::gets()*1.0f, 0.0f, 0.0f, 1.0f);
	gl::scale(2.9f, 2.9f, 2.9f);

//	draw_axes();
//	draw_smth();
	draw_2d_noise(Time::gets());
//  draw_my_image(&image, 0, 0);
	gl::popMatrix();
}

void NoisyScene::handleKeyboardEvent(Key key)
{
	switch (key) {
	case KeyTab:
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case KeyCapsLock:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;

	case KeySpace:
		m_texture = !m_texture;
		break;

	case KeyLShift:
//		if (m_step < 0.2)
			m_step += 0.01;
		break;

	case KeyRShift:
		if (m_step > 0.005)
			m_step -= 0.005;
		break;

	case KeyUp:
		m_turbK++;
		break;

	case KeyDown:
		m_turbK--;
		break;

	case KeyRight:
		if (m_turbOmega < 7)
			m_turbOmega++;
		break;

	case KeyLeft:
		if (m_turbOmega > -1)
			m_turbOmega--;
		break;

	case KeyLCtrl:
		if (m_landscapeSize > 0)
			m_landscapeSize -= 0.1;
		break;

	case KeyRCtrl:
		m_landscapeSize += 0.1;
		break;

	default:
		return;
	}
}

void NoisyScene::draw_1d_noise(TimeT Time)
{
	glPointSize(15.0);
	glBegin(GL_POINTS);
	for (float i = -4; i < 10; i += 0.004)
	{
		glColor3d(18.0 / 255, 0.0 / 255, 255.0 / 255);
		glVertex3d(i / 4, (noise1d(i + Time) - 0.7), -1);
		glColor3d(184.0 / 255, 8.0 / 255, 228.0 / 255);
		glVertex3d(i / 4, (noise1d_turb(i + Time, 4) - 0.7), -1);
		glColor3d(255.0 / 255, 0.0 / 255, 18.0 / 255);
		glVertex3d(i / 12, bezier(i + Time) / 10, -1);
	}
	glEnd();
}

void NoisyScene::draw_2d_noise(TimeT Time)
{
	TextureManager::inst().bindTexture("color_tile.png");

	for (float i = -m_landscapeSize; i < m_landscapeSize; i += m_step)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (float j = -m_landscapeSize; j < m_landscapeSize; j += m_step)
		{
//			if (m_texture)
//				glTexCoord2d((i + 0.8) / 1.6 * m_texCoordK, (j + 0.8) / 1.6 * m_texCoordK);
//			glColor3d(1, 1, noise2d_turb(j, i, 3));
//			glVertex3d(j, i, noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega));
//			glVertex3d(j, i + m_step, noise2d_turb((i + m_step) * m_turbK, j * m_turbK, m_turbOmega));

			if (m_texture)
				glTexCoord2d((i + 0.8) / 1.6 * m_texCoordK, (j + 0.8) / 1.6 * m_texCoordK);

			float h = noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega);
			Color3 col = get_color(h);
			glColor3d(col.r / 255.0, col.g / 255.0, col.b / 255.0);
//			glVertex3d(j + Time, i + Time, noise2d_turb(i * m_turbK, j * m_turbK, m_turbOmega));
//			glVertex3d(j + Time, (i + m_step) + Time, noise2d_turb((i + m_step) * m_turbK, j * m_turbK, m_turbOmega));
			glVertex3d(j, i, h);
			glVertex3d(j, (i + m_step), noise2d_turb((i + m_step) * m_turbK, j * m_turbK, m_turbOmega));
		}
		glEnd();
	}
}

void NoisyScene::draw_2d_noise_(TimeT Time)
{
	float i, j;//, x, y, z;
	//int N = 10, M = 10;

	glColor3d(1, 1, 1);
	glPointSize((GLfloat)1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POINTS);
	for (i = -0.8; i < 0.8; i += 0.006)
		for (j = -0.8; j < 0.8; j += 0.006)
		{
			glTexCoord2d((i + 0.8) / 1.6, (j + 0.8) / 1.6);
//				  glColor3d(/*246.0 / 255*/noise2d(i, j), 6.0 / 255, 158.0 / 255);
			glVertex3d(i, j, noise2d_turb(i * 3, j * 3, 2));
			glVertex3d(i + 0.003, j + 0.006, noise2d_turb(i * 3, j * 3, 2));
		}
	glEnd();
}

void NoisyScene::draw_axes()
{
	glBegin(GL_LINES);

	glColor3d(1, 0, 0);
	glVertex2d(-1, 0);
	glVertex4d(1, 0, 0, 0);
/*  glVertex2d(0, 0);
	glVertex4d(-1, 0, 0, 0);
  */
	glColor3d(0, 1, 0);
	glVertex2d(0, -1);
	glVertex4d(0, 1, 0, 0);
/*  glVertex2d(0, 0);
	glVertex4d(0, -1, 0, 0);
  */
	glColor3d(0, 0, 1);
	glVertex3d(0, 0, -1);
/*  glVertex4d(0, 0, -1, 0);
	glVertex2d(0, 0);    */
	glVertex4d(0, 0, 1, 0);

	glEnd();
}

void NoisyScene::draw_smth()
{
	int i, j, N = 115, M = 115;
	float x, y, z;
	glColor3d(110.0 / 255, 21.0 / 255, 147.0 / 255);
	glNormal3d(0, 1, 0);
	for (i = 0; i < N; i ++)
	{
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= M; j ++)
		{
			x = (float)j / M;
			y = 0;
			z = (float)i / N;
			glVertex3d(x, y, z);
			x = (float)j / M;
			y = 0;
			z = (float)(i + 1) / N;
			glVertex3d(x, y, z);
		}
		glEnd();
	}
}

void NoisyScene::SetColormap()
{
	m_colormap[0].n = 0.3;
	m_colormap[0].col = Color3( 19, 117, 207);
	m_colormap[1].n = 0.4;
	m_colormap[1].col = Color3(  9, 201,  22);
	m_colormap[2].n = 0.5;
	m_colormap[2].col = Color3(133, 106,  42);
	m_colormap[3].n = 0.6;
	m_colormap[3].col = Color3(181, 101, 211);
//	m_colormap[0].n = 0.3;
//	m_colormap[0].col = Color3(0.98*255, 0.625*255, 0.12*255);
//	m_colormap[1].n = 0.4;
//	m_colormap[1].col = Color3(0.98*255, 0.04*255, 0.7*255);
//	m_colormap[2].n = 0.5;
//	m_colormap[2].col = Color3(0.6*255, 0.4*255, 0.7*255);
//	m_colormap[3].n = 0.6;
//	m_colormap[3].col = Color3(181, 101, 211);
}

remath::Color3 NoisyScene::get_color(float h)
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

void NoisyScene::SetTab1()
{
	int i;
	srand((unsigned int)time(NULL));
	for (i = 0; i < SIZE; i ++)
		Tab1[i] = (float)rand() / RAND_MAX;
}

void NoisyScene::SetTab2()
{
	int i, j;
	for (i = 0; i < SIZE; i ++)
		for (j = 0; j < SIZE; j ++)
			Tab2[i][j] = (float)rand() / RAND_MAX;
}

float NoisyScene::fmod1(float t, float x)
{
	if (t < 0)
		return 1 + fmod(t, 1);
	return fmod(t, 1);
}

float NoisyScene::noise(float t)
{
	int ix = (int)floor(t) & MASK;
	float a, b, c, fx = fmod1(t, 1);

	a = Tab1[ix];
	b = Tab1[(ix + 1) & MASK];
	c = a * (1 - fx) + b * fx;
	return c;
}

float NoisyScene::noise1d(float t)
{
	int ix = (int)floor(t) & MASK;
	float a, b, c, fx = fmod1(t, 1);

	a = Tab1[ix];
	b = Tab1[(ix + 1) & MASK];
	fx = fx * fx * (3 - 2 * fx);
	c = a * (1 - fx) + b * fx;
	return c;
}

float NoisyScene::noise2d(float t1, float t2)
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

float NoisyScene::noise1d_turb(float t, int omega)
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

float NoisyScene::noise2d_turb(float t1, float t2, int omega)
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

float NoisyScene::bezier(float x)
{
	int ix = (int)floor(x) & MASK;
	float fx = fmod1(x, 1);
	return Tab1[ix - 1] * (- fx * fx * fx + 3 * fx * fx - 3 * fx + 1) +
		   Tab1[ix] * (3 * fx * fx * fx - 6 * fx * fx + 4) +
		   Tab1[ix + 1] * (-3 * fx * fx * fx + 3 * fx * fx + 3 * fx + 1)+
		   Tab1[ix + 2] * fx * fx * fx;
}


void NoisyScene::generateNewStar(int i)
{
	m_stars[i] = Vector3f(rand() % 400 - 100,
						  rand() % 300 - 100,
						-(rand() % 100 + 50));
}

#endif /* ifdef __DEPRECATED_PROFILE__ */
