#pragma once

// Matrix stack functionality

#include "math/math3d.h"
#include "frame.h"

namespace render {

enum GLT_STACK_ERROR { GLT_STACK_NOERROR = 0, GLT_STACK_OVERFLOW, GLT_STACK_UNDERFLOW };

class GLMatrixStack
{
public:
	GLMatrixStack(int iStackDepth = 64) {
		stackDepth = iStackDepth;
		pStack = new M3DMatrix44f[iStackDepth];
		stackPointer = 0;
		m3dLoadIdentity44(pStack[0]);
		lastError = GLT_STACK_NOERROR;
	}


	~GLMatrixStack(void) {
		delete [] pStack;
	}


	inline void LoadIdentity(void) {
		m3dLoadIdentity44(pStack[stackPointer]);
	}

	inline void LoadMatrix(const M3DMatrix44f mMatrix) {
		m3dCopyMatrix44(pStack[stackPointer], mMatrix);
	}

	inline void LoadMatrix(GLFrame& frame) {
		M3DMatrix44f m;
		frame.GetMatrix(m);
		LoadMatrix(m);
	}

	inline void MultMatrix(const M3DMatrix44f mMatrix) {
		M3DMatrix44f mTemp;
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mMatrix);
	}

	inline void MultMatrix(GLFrame& frame) {
		M3DMatrix44f m;
		frame.GetMatrix(m);
		MultMatrix(m);
	}

	inline void PushMatrix(void) {
		if (stackPointer < (stackDepth-1)) {
			stackPointer++;
			m3dCopyMatrix44(pStack[stackPointer], pStack[stackPointer-1]);
		}
		else
			lastError = GLT_STACK_OVERFLOW;
	}

	inline void PopMatrix(void) {
		if (stackPointer > 0)
			stackPointer--;
		else
			lastError = GLT_STACK_UNDERFLOW;
	}

	void Scale(GLfloat x, GLfloat y, GLfloat z) {
		M3DMatrix44f mTemp, mScale;
		m3dScaleMatrix44(mScale, x, y, z);
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mScale);
	}


	void Translate(GLfloat x, GLfloat y, GLfloat z) {
		M3DMatrix44f mTemp, mScale;
		m3dTranslationMatrix44(mScale, x, y, z);
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mScale);
	}

	void Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
		M3DMatrix44f mTemp, mRotate;
		m3dRotationMatrix44(mRotate, float(m3dDegToRad(angle)), x, y, z);
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mRotate);
	}


	// I've always wanted vector versions of these
	void Scalev(const M3DVector3f vScale) {
		M3DMatrix44f mTemp, mScale;
		m3dScaleMatrix44(mScale, vScale);
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mScale);
	}

	void Translatev(const M3DVector3f vTranslate) {
		M3DMatrix44f mTemp, mTranslate;
		m3dLoadIdentity44(mTranslate);
		memcpy(&mTranslate[12], vTranslate, sizeof(GLfloat) * 3);
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mTranslate);
	}


	void Rotatev(GLfloat angle, M3DVector3f vAxis) {
		M3DMatrix44f mTemp, mRotation;
		m3dRotationMatrix44(mRotation, float(m3dDegToRad(angle)), vAxis[0], vAxis[1], vAxis[2]);
		m3dCopyMatrix44(mTemp, pStack[stackPointer]);
		m3dMatrixMultiply44(pStack[stackPointer], mTemp, mRotation);
	}


	// I've also always wanted to be able to do this
	void PushMatrix(const M3DMatrix44f mMatrix) {
		if (stackPointer < stackDepth) {
			stackPointer++;
			m3dCopyMatrix44(pStack[stackPointer], mMatrix);
		}
		else
			lastError = GLT_STACK_OVERFLOW;
	}

	void PushMatrix(GLFrame& frame) {
		M3DMatrix44f m;
		frame.GetMatrix(m);
		PushMatrix(m);
	}

	// Two different ways to get the matrix
	const M3DMatrix44f& GetMatrix(void) { return pStack[stackPointer]; }
	void GetMatrix(M3DMatrix44f mMatrix) { m3dCopyMatrix44(mMatrix, pStack[stackPointer]); }


	inline GLT_STACK_ERROR GetLastError(void) {
		GLT_STACK_ERROR retval = lastError;
		lastError = GLT_STACK_NOERROR;
		return retval;
	}

protected:
	GLT_STACK_ERROR		lastError;
	int					stackDepth;
	int					stackPointer;
	M3DMatrix44f		*pStack;
};

} // end of namespace 'render'
