#pragma once

#include "../globals.h"

namespace render {

using namespace remath;

class glHelper
{
public:
	enum CullMode
	{
		POINT,
		LINE,
		FILL,
		NONE, // CULL
		firstCullMode = POINT,
		lastCullMode  = NONE
	};

	enum BlendMode
	{
		ADD,
		SUB,
		SUBR,
		MIN,
		MAX,
		firstBlendMode = ADD,
		lastBlendMode  = MAX
	};

	enum BlendFactor
	{
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

	enum Buffer
	{
		COLOR = 1,
		DEPTH = 2,
		STENCIL = 4,
		ALL = COLOR | DEPTH | STENCIL
	};

	enum AspectRatio
	{
		ASPECTRATIO_4_3 = 0,
		ASPECTRATIO_5_4,
		ASPECTRATIO_16_10,
		ASPECTRATIO_16_9
	};

private:
	static const GLint cullMode_toGL[lastCullMode + 1];
	static const GLint blendMode_toGL[lastBlendMode + 1];
	static const GLint blendFactor_toGL[lastBlendFactor + 1];

	static float m_zNear, m_zFar, m_fov;
	static int m_width, m_height;
	static float m_aspectratio;

public:
	static void init(int w, int h, AspectRatio aspect)
	{
		m_width = w;
		m_height = h;

		m_fov = 45.0f;
		m_zNear = 0.1f;
		m_zFar = 14000.0f;

		resize(w, h, aspect);

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);

		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		glDisable(GL_NORMALIZE);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
	}

	static void resize(int w, int h, AspectRatio aspect)
	{
//		if(height == 0) height = 1;

		switch (aspect)
		{
		case ASPECTRATIO_4_3:
			break;
		case ASPECTRATIO_5_4:
			break;
		case ASPECTRATIO_16_10:
			h = int(w * 0.625f);
			break;
		case ASPECTRATIO_16_9:
			h = int(w * 0.5625f);
			break;
		}

		m_aspectratio = (float)w / h;

		glViewport(0, (m_height - h) / 2, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45.0f, aspect, 0.1f, 14000.0f);
		gluPerspective(m_fov, m_aspectratio, m_zNear, m_zFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

//	static void enable(GLint[] features...)
//	{
//		foreach (GLint i; features) glEnable(i);
//	}

//	static void disable(GLint[] features...)
//	{
//		foreach (GLint i; features) glDisable(i);
//	}

	static GLint matrixMode(GLint mm)
	{
		glMatrixMode(mm);
		return mm;
	}

	static void loadIdentity()
	{
		glLoadIdentity();
	}

	static void loadMatrix(Matrix4f& m)
	{
//		if (glLoadTransposeMatrixf is null)
//		{
//			Matrix4f& tr = m.transposed();
//			glLoadMatrixf(tr.ptr());
//		} else
//		{
			glLoadTransposeMatrixf(m.ptr());
//		}
	}

	static void multMatrix(Matrix4f& m)
	{
//		if (glMultTransposeMatrixf is null)
//		{
//			Matrix4f& tr = m.transposed();
//			glMultMatrixf(tr.ptr());
//		} else
//		{
			glMultTransposeMatrixf(m.ptr());
//		}
	}

	static void lookAt(Vector3 eye, Vector3 target, Vector3 up)
	{
		gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);
	}

	static Matrix4f projectionMatrix()
	{
		Matrix4f m;
		glGetFloatv(GL_PROJECTION_MATRIX, m.ptr());
		m.Transpose();
		return m;
	}

	static Matrix4f projectionMatrix(Matrix4f& m)
	{
		glMatrixMode(GL_PROJECTION);
		loadMatrix(m);
		return m;
	}

	static Matrix4f modelviewMatrix()
	{
		Matrix4f m;
		glGetFloatv(GL_MODELVIEW_MATRIX, m.ptr());
		m.Transpose();
		return m;
	}

	static Matrix4f modelviewMatrix(Matrix4f& m)
	{
		glMatrixMode(GL_MODELVIEW);
		loadMatrix(m);
		return m;
	}

	static void check()
	{
//		version(Release)
//		{
//			// do nothing in release mode !
//		}
//		else
		{
			GLint r = glGetError();
			if (r != GL_NO_ERROR)
			{
			//	trace(format("error %s", r));
				reutil::g_debug << gluErrorString(r) << std::endl;

//				assert(false);
				//throw new GLError(msg);
			}
		}
	}

	static void setCullMode(CullMode front, CullMode back)
	{
		bool cullFront = (NONE == front),
			 cullBack = (NONE == back);

		if ( (!cullFront) && (!cullBack))
		{
			glDisable(GL_CULL_FACE);
		} else
		{
			glEnable(GL_CULL_FACE);
			GLenum toHide = cullFront ? (cullBack ? GL_FRONT_AND_BACK : GL_FRONT) : GL_BACK;
			glCullFace(toHide);

			if ( !cullFront ) glPolygonMode(GL_FRONT, cullMode_toGL[front]);
			if ( !cullBack ) glPolygonMode(GL_BACK, cullMode_toGL[back]);
		}
		glHelper::check();
	}

	static float pointSize(float new_pointSize)
	{
		glPointSize(new_pointSize);
		return new_pointSize;
	}

//	static vec4f clearColor(vec4f c)
//	{
//		glClearColor(c.x, c.y, c.z, c.w);
//		return c;
//	}

//	static Vector3 color(Vector3 c)
//	{
//		glColor3fv(c.ptr());
//		return c;
//	}

//	static vec4f color(vec4f c)
//	{
//		glColor4fv(c.ptr());
//		return c;
//	}



	static float clearDepth(float depth)
	{
		glClearDepth(depth);
		return depth;
	}

	static void setBlend(BlendMode RGBOperation, BlendFactor srcRGBFactor, BlendFactor dstRGBFactor,
						 BlendMode AlphaOperation, BlendFactor srcAlphaFactor, BlendFactor dstAlphaFactor )
	{
//		if ((glBlendEquationSeparate !is null) && (glBlendFuncSeparate !is null))
//		{
			glBlendEquationSeparate(blendMode_toGL[RGBOperation], blendMode_toGL[AlphaOperation]);
			glBlendFuncSeparate( blendFactor_toGL[srcRGBFactor],
								 blendFactor_toGL[dstRGBFactor],
								 blendFactor_toGL[srcAlphaFactor],
								 blendFactor_toGL[dstAlphaFactor] );
//		} else
//		{
//			// try to approximate the desired blend
//			glBlendFunc( blendFactor_toGL[srcRGBFactor], blendFactor_toGL[dstRGBFactor]);
//			glBlendFunc( blendFactor_toGL[srcRGBFactor], blendFactor_toGL[dstRGBFactor]);
//			glBlendEquation( blendMode_toGL[RGBOperation] );
//		}
	}

	/*deprecated */static void setViewport(int x, int y, int width, int height)
	{
		glViewport(x,y,width,height);
		glHelper::check();
	}

	static void viewport(Recti view)
	{
		glViewport(view.x1(), view.y1(), view.width(), view.height());
		glHelper::check();
	}

	static void clear(Buffer b)
	{
		GLint mask = 0;
		if (b & COLOR)   mask |= GL_COLOR_BUFFER_BIT;
		if (b & STENCIL) mask |= GL_STENCIL_BUFFER_BIT;
		if (b & DEPTH)   mask |= GL_DEPTH_BUFFER_BIT;
		glClear(mask);
		glHelper::check();
	}

	static bool glBool(bool b)
	{
		return b ? GL_TRUE : GL_FALSE;
	}

//	static void vertex(vec2f[] vs...)	{	foreach(v; vs) glVertex2fv(v.ptr());	}
//	static void vertex(Vector3[] vs...)	{	foreach(v; vs) glVertex3fv(v.ptr());	}
//	static void vertex(vec4f[] vs...)	{	foreach(v; vs) glVertex4fv(v.ptr());	}

//	static void normal(float x, float y, float z) {	glNormal3f(x, y, z); }
//	static void normal(Vector3 nml)		{	glNormal3fv(nml.ptr());	}

	static void translate(float x, float y, float z){	glTranslatef(x, y, z); }
	static void translate(Vector3 v)					{	glTranslatef(v.x, v.y, v.z);	}

	static void scale(float x, float y, float z)	{	glScalef(x, y, z); }
	static void scale(Vector3 v)						{	glScalef(v.x, v.y, v.z);	}

	static void rotate(float angle, float x, float y, float z)  {	glRotatef(angle, x, y, z); }
//	static void rotate(vec4f v)									{	glRotatef(v.x, v.y, v.z, v.z);	}

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
};

typedef glHelper gl;

} // end of namaspace 'render'
