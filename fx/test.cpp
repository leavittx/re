#include "test.h"

#include "core/system.h"
#include <cmath>

using namespace redemo;
using namespace recore;

void TestScene::init()
{
}

void TestScene::release()
{
}

void TestScene::update()
{
}

void TestScene::draw()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glDisable(GL_NORMALIZE);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);

	int width = 1024;
	int height = 768;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float size = 1.0f;
	float scale = fabs(sin(System::inst().getTime() / 500.0));
	glColor4f(0.1f, 0.4f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(size*scale, 0);
	glVertex2f(size*scale, size*scale);
	glVertex2f(0, size*scale);
	glEnd();

	float fov = 45.0f;
	float zNear = 0.1f;
	float zFar = 14000.0f;
	float aspect = (float)width/(float)height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
