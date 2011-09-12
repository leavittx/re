#include "test.h"

#include "core/system.h"
#include <cmath>

using namespace redemo;
using namespace recore;
using namespace render;
using namespace remath;

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
	// Set ortho mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Clear screen
	gl::clear(gl::ALL);

	float size = 1.0f;
	float scale = fabs(sin(System::inst().getTime() / 500.0));
	//glColor4f(0.1f, 0.4f, 1.0f, 0.0f);
	gl::color(Color3(1.0f, 0.0f, 1.0f));
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(size*scale, 0);
		glVertex2f(size*scale, size*scale);
		glVertex2f(0, size*scale);
	glEnd();
}
