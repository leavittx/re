#pragma once

#include "../globals.h"
#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL/glu.h>

#include "frame.h"
#include "frustum.h"
#include "matrixstack.h"
#include "geometrytransform.h"
#include "batch/batch.h"
#include "batch/trianglebatch.h"

namespace render {

class glHelper
{
public:
	enum CullMode {
		POINT,
		LINE,
		FILL,
		NONE, // CULL
		firstCullMode = POINT,
		lastCullMode  = NONE
	};
	enum BlendMode {
		ADD,
		SUB,
		SUBR,
		MIN,
		MAX,
		firstBlendMode = ADD,
		lastBlendMode  = MAX
	};
	enum BlendFactor {
		ZERO,
		ONE,
		DST_COLOR,
		SRC_COLOR,
		ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		SRC_ALPHA_SATURATE,
		firstBlendFactor = ZERO,
		lastBlendFactor  = SRC_ALPHA_SATURATE
	};

	enum Buffer {
		COLOR = 1,
		DEPTH = 2,
		STENCIL = 4,
		ALL = COLOR | DEPTH | STENCIL
	};

	enum AspectRatio {
		ASPECTRATIO_4_3 = 0,
		ASPECTRATIO_5_4,
		ASPECTRATIO_16_10,
		ASPECTRATIO_16_9,
		ASPECTRATIO_1_1
	};

private:
	static const GLint cullMode_toGL[lastCullMode + 1];
	static const GLint blendMode_toGL[lastBlendMode + 1];
	static const GLint blendFactor_toGL[lastBlendFactor + 1];

//	static float m_zNear, m_zFar, m_fov;
	static int m_width, m_height;
	static float m_aspectratio;

public:
	static GLFrustum m_viewFrustum;

public:
	//		GLUtesselator

	static void init(int w, int h, AspectRatio aspect)
	{
//		m_fov = 45.0f;
//		m_zNear = 0.1f;
//		m_zFar = 14000.0f;

		resize(w, h, aspect);

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	}

	static void resize(int w, int h, AspectRatio aspect)
	{
		m_width = w;
		m_height = h;

		switch (aspect) {
		case ASPECTRATIO_4_3:
			w = int(h * 4.0 / 3);
			break;
		case ASPECTRATIO_5_4:
			w = int(h * 5.0 / 4);
			break;
		case ASPECTRATIO_16_10:
			h = int(w * 0.625f);
			break;
		case ASPECTRATIO_16_9:
			h = int(w * 0.5625f);
			break;
		case ASPECTRATIO_1_1:
			w = h;
			break;
		}

		m_aspectratio = float(w) / float(h);

		setViewport(0, (m_height - h) / 2, w, h);

		m_viewFrustum.SetPerspective(45.0f, m_aspectratio, 1.0f, 1000.0f);

#ifdef __DEPRECATED_PROFILE__
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45.0f, aspect, 0.1f, 14000.0f);
		gluPerspective(m_fov, m_aspectratio, m_zNear, m_zFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
#endif /* ifdef __DEPRECATED_PROFILE__ */
	}

	static void resetViewport()
	{
		setViewport(0, 0, m_width, m_height);
	}

#ifdef __DEPRECATED_PROFILE__
	static void frustum(float size, float zNear, float zFar)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-size, size, size * (double)-m_height / m_height, size * (double)m_height / m_width, zNear, zFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	static GLint matrixMode(GLint mm)
	{
		glMatrixMode(mm);
		return mm;
	}

	static void loadIdentity()
	{
		glLoadIdentity();
	}

	static void loadMatrix(remath::Matrix4f& m)
	{
		glLoadTransposeMatrixf(m.ptr());
	}

	static void multMatrix(remath::Matrix4f& m)
	{
		glMultTransposeMatrixf(m.ptr());
	}

	static void lookAt(remath::Vector3f& eye, remath::Vector3f& target, remath::Vector3f& up)
	{
		gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);
	}

	static remath::Matrix4f projectionMatrix()
	{
		remath::Matrix4f m;
		glGetFloatv(GL_PROJECTION_MATRIX, m.ptr());
		m.Transpose();
		return m;
	}

	static remath::Matrix4f projectionMatrix(remath::Matrix4f& m)
	{
		glMatrixMode(GL_PROJECTION);
		loadMatrix(m);
		return m;
	}

	static remath::Matrix4f modelviewMatrix()
	{
		remath::Matrix4f m;
		glGetFloatv(GL_MODELVIEW_MATRIX, m.ptr());
		m.Transpose();
		return m;
	}

	static remath::Matrix4f modelviewMatrix(remath::Matrix4f& m)
	{
		glMatrixMode(GL_MODELVIEW);
		loadMatrix(m);
		return m;
	}
#endif /* ifdef __DEPRECATED_PROFILE__ */

#ifdef __DEPRECATED_PROFILE__
	// We assume glu is deprecated
	static void check()
	{
		GLint r = glGetError();
		if (r != GL_NO_ERROR)
		{
			reutil::g_debug << gluErrorString(r) << std::endl;

		}
	}
#endif /* ifdef __DEPRECATED_PROFILE__ */

	// Check for general GL errors that may affect rendering
	static void debug(const std::string& text)
	{
		GLenum error = glGetError();
		switch (error) {
			case GL_NO_ERROR:
	//			reutil::g_debug << text << ": OK!" << std::endl;
				break;
			case GL_INVALID_ENUM:
				reutil::g_debug << "OpenGL error! " << text << " : GL_INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				reutil::g_debug << "OpenGL error! " << text << " : GL_INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				reutil::g_debug << "OpenGL error! " << text << " : GL_INVALID_OPERATION" << std::endl;
				break;
			case GL_STACK_OVERFLOW:
				reutil::g_debug << "OpenGL error! " << text << " : GL_STACK_OVERFLOW" << std::endl;
				break;
			case GL_STACK_UNDERFLOW:
				reutil::g_debug << "OpenGL error! " << text << " : GL_STACK_UNDERFLOW" << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				reutil::g_debug << "OpenGL error! " << text << " : GL_OUT_OF_MEMORY" << std::endl;
				break;
		}
	}

	// Get the OpenGL version number
	void gltGetOpenGLVersion(GLint &nMajor, GLint &nMinor)
	{
		glGetIntegerv(GL_MAJOR_VERSION, &nMajor);
		glGetIntegerv(GL_MINOR_VERSION, &nMinor);
	}

	// This function determines if the named OpenGL Extension is supported
	// Returns 1 or 0
	int gltIsExtSupported(const char *extension)
	{
		GLint nNumExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &nNumExtensions);

		for (GLint i = 0; i < nNumExtensions; i++)
			if (strcmp(extension, (const char *)glGetStringi(GL_EXTENSIONS, i)) == 0)
				return 1;
		return 0;
	}


	static void setCullMode(CullMode front, CullMode back)
	{
		bool cullFront = (NONE == front),
			 cullBack  = (NONE == back);

		if ( (!cullFront) && (!cullBack))
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			GLenum toHide = cullFront ? (cullBack ? GL_FRONT_AND_BACK : GL_FRONT) : GL_BACK;
			glCullFace(toHide);

			if (!cullFront) glPolygonMode(GL_FRONT, cullMode_toGL[front]);
			if (!cullBack)  glPolygonMode(GL_BACK,  cullMode_toGL[back]);
		}

		debug("glHelper::setCullMode");
	}

	static float pointSize(float new_pointSize)
	{
		glPointSize(new_pointSize);
		return new_pointSize;
	}

	static remath::Color4 clearColor(const remath::Color4& c)
	{
		glClearColor(c.x, c.y, c.z, c.w);
		return c;
	}

	static remath::Color3 color(const remath::Color3& c)
	{
		glColor3fv(&c.r);
		return c;
	}

	static remath::Color4 color(const remath::Color4& c)
	{
		glColor4fv(&c.r);
		return c;
	}

	static float clearDepth(float depth)
	{
		glClearDepth(depth);
		return depth;
	}

	static void setBlend(BlendMode RGBOperation, BlendFactor srcRGBFactor, BlendFactor dstRGBFactor,
						 BlendMode AlphaOperation, BlendFactor srcAlphaFactor, BlendFactor dstAlphaFactor )
	{
		glBlendEquationSeparate(blendMode_toGL[RGBOperation], blendMode_toGL[AlphaOperation]);
		glBlendFuncSeparate(blendFactor_toGL[srcRGBFactor],
							blendFactor_toGL[dstRGBFactor],
							blendFactor_toGL[srcAlphaFactor],
							blendFactor_toGL[dstAlphaFactor]);
//		// try to approximate the desired blend
//		glBlendFunc( blendFactor_toGL[srcRGBFactor], blendFactor_toGL[dstRGBFactor]);
//		glBlendFunc( blendFactor_toGL[srcRGBFactor], blendFactor_toGL[dstRGBFactor]);
//		glBlendEquation( blendMode_toGL[RGBOperation] );
	}

	// deprecated
	static void setViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);

		debug("glHelper::setViewport");
	}

	static void viewport(remath::Recti view)
	{
		glViewport(view.x1(), view.y1(), view.width(), view.height());

		debug("glHelper::viewport");
	}

	static void clear(Buffer b)
	{
		GLint mask = 0;
		if (b & COLOR)   mask |= GL_COLOR_BUFFER_BIT;
		if (b & STENCIL) mask |= GL_STENCIL_BUFFER_BIT;
		if (b & DEPTH)   mask |= GL_DEPTH_BUFFER_BIT;
		glClear(mask);

		debug("glHelper::clear");
	}

	static bool glBool(bool b)
	{
		return b ? GL_TRUE : GL_FALSE;
	}

#ifdef __DEPRECATED_PROFILE__
	static void translate(float x, float y, float z)
	{
		glTranslatef(x, y, z);
	}
	static void translate(remath::Vector3f& v)
	{
		glTranslatef(v.x, v.y, v.z);
	}

	static void scale(float x, float y, float z)
	{
		glScalef(x, y, z);
	}
	static void scale(remath::Vector3f& v)
	{
		glScalef(v.x, v.y, v.z);
	}

	static void rotate(float angle, float x, float y, float z)
	{
		glRotatef(angle, x, y, z);
	}

	static void rotate(remath::Vector4f& v)
	{
		glRotatef(v.x, v.y, v.z, v.z);
	}

	static void pushAttrib(int attribs)
	{
		glPushAttrib(attribs);
	}
	static void popAttrib()
	{
		glPopAttrib();
	}

	static void pushMatrix()
	{
		glPushMatrix();
	}
	static void popMatrix()
	{
		glPopMatrix();
	}
#endif /* ifdef __DEPRECATED_PROFILE__ */
};

typedef glHelper gl;

} // end of namaspace 'render'
