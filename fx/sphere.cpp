#include "sphere.h"
#include "util/debug.h"
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

const float SphereScene::ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
const float SphereScene::diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
const float SphereScene::cameraPos[]    = { 800.0f, 800.0f, 800.0f };

void SphereScene::init()
{
	m_useBufferObject = true;
	m_mapBufferObject = true;
	m_animating = true;

	m_numSphereVertices = 3000;//30000;

	m_sphereVertexArray = (GLfloat *)malloc(sizeof(GLfloat) * m_numSphereVertices * 3);

	// Generate a buffer object
	glGenBuffers(1, &m_bufferID);
	// Create the data store
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numSphereVertices * 3, NULL, GL_STATIC_DRAW);

	// Set up vertex arrays
	RegenerateSphere();
	SetRenderingMethod();
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// Black background
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f );

	// Hidden surface removal
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set up some lighting state that never changes
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
}

void SphereScene::release()
{
	if (m_sphereVertexArray)
		free(m_sphereVertexArray);
}

void SphereScene::update()
{
}

void SphereScene::draw()
{
	// Track camera angle
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45.0f, 1.0f, 10.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

//	glUtil::pushMatrix();
//	glUtil::rotate(g_system.getTime()*0.1, 1, 1, 1);

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_animating)
	{
		RegenerateSphere();
		SetRenderingMethod();
	}

	// Draw objects in the scene
	DrawModels();

//	glUtil::popMatrix();
}

// Called to regenerate points on the sphere
void SphereScene::RegenerateSphere(void)
{
	GLint i;

	if (m_mapBufferObject && m_useBufferObject)
	{
		// Delete old vertex array memory
		if (m_sphereVertexArray)
			free(m_sphereVertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);

		// Avoid pipeline flush during glMapBuffer by
		// marking data store as empty
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numSphereVertices * 3, NULL,
					 m_animating ? GL_STREAM_DRAW : GL_STATIC_DRAW);

		m_sphereVertexArray = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (!m_sphereVertexArray)
		{
			g_debug << "Unable to map buffer object!" << endl;
			return;
		}
	}
	else if (!m_sphereVertexArray)
	{
		// We need our old vertex array memory back
		m_sphereVertexArray = (GLfloat *)malloc(sizeof(GLfloat) * m_numSphereVertices * 3);
	}

	for (i = 0; i < m_numSphereVertices; i++)
	{
		GLfloat r1, r2, r3, scaleFactor;

		// pick a random vector
		r1 = (GLfloat)(rand() - (RAND_MAX/2));
		r2 = (GLfloat)(rand() - (RAND_MAX/2));
		r3 = (GLfloat)(rand() - (RAND_MAX/2));

		// determine normalizing scale factor
		//scaleFactor = (/*g_pitch.matchFreqInterval(0, 100)*/g_pitch.isKick() ? 1.3f : 1.0f) / sqrt(r1*r1 + r2*r2 + r3*r3);
		scaleFactor = 1.0f;

		m_sphereVertexArray[(i*3)+0] = r1 * scaleFactor;
		m_sphereVertexArray[(i*3)+1] = r2 * scaleFactor;
		m_sphereVertexArray[(i*3)+2] = r3 * scaleFactor;
	}

	if (m_mapBufferObject && m_useBufferObject)
	{
		if (!glUnmapBuffer(GL_ARRAY_BUFFER))
		{
			// Some window system event has trashed our data...
			// Try, try again!
			RegenerateSphere();
		}
		m_sphereVertexArray = NULL;
	}
}

// Switch between buffer objects and plain old vertex arrays
void SphereScene::SetRenderingMethod(void)
{
	if (m_useBufferObject)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		// No stride, no offset
		glNormalPointer(GL_FLOAT, 0, 0);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		if (!m_mapBufferObject)
		{
			if (m_animating)
			{
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) *
								m_numSphereVertices * 3, m_sphereVertexArray);
			}
			else
			{
				// If not animating, this gets called once
				// to establish new static buffer object
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *
							 m_numSphereVertices * 3, m_sphereVertexArray,
							 GL_STATIC_DRAW);
			}
		}
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glNormalPointer(GL_FLOAT, 0, m_sphereVertexArray);
		glVertexPointer(3, GL_FLOAT, 0, m_sphereVertexArray);
	}
}

// Called to draw scene objects
void SphereScene::DrawModels(void)
{
	GLint r, g, b;

	// Draw spheres in a color cube
	for (r = 0; r < 4; r++)
	{
		for (g = 0; g < 4; g++)
		{
			for (b = 0; b < 4; b++)
			{
				glColor3f(r * 0.5f, g * 0.5f, b * 0.5f);

				glPushMatrix();
				glTranslatef(100.0f * r - 100.0f,
							 100.0f * g - 100.0f,
							 100.0f * b - 100.0f);
				glScalef(50.0f, 50.0f, 50.0f);
				glDrawArrays(GL_POINTS, 0, m_numSphereVertices);
				glPopMatrix();
			}
		}
	}
}
