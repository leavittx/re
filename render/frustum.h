#pragma once

// Encapsulates a frustum... works in conjunction
// with GLFrame

#include "../math/math.h"
//#include <GLFrame.h>

namespace render {

class GLFrustum
{
public:
	// Set some Reasonable Defaults
	GLFrustum(void)
	{
		SetOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	}

	// Set the View Frustum
	GLFrustum(GLfloat fFov, GLfloat fAspect, GLfloat fNear, GLfloat fFar)
	{
		SetPerspective(fFov, fAspect, fNear, fFar);
	}

	GLFrustum(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat zMin, GLfloat zMax)
	{
		SetOrthographic(xMin, xMax, yMin, yMax, zMin, zMax);
	}

	// Get the projection matrix for this guy
	const remath::Matrix4f& GetProjectionMatrix(void)
	{
		return projMatrix;
	}

	// Calculates the corners of the Frustum and sets the projection matrix.
	// Orthographics Matrix Projection
	void SetOrthographic(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat zMin, GLfloat zMax)
	{
		projMatrix.MakeOrthographic(xMin, xMax, yMin, yMax, zMin, zMax);
		projMatrix(3,3) = 1.0f;

		// Fill in values for untransformed Frustum corners
		// Near Upper Left
		nearUL.x = xMin; nearUL.y = yMax; nearUL.z = zMin; nearUL.w = 1.0f;

		// Near Lower Left
		nearLL.x = xMin; nearLL.y = yMin; nearLL.z = zMin; nearLL.w = 1.0f;

		// Near Upper Right
		nearUR.x = xMax; nearUR.y = yMax; nearUR.z = zMin; nearUR.w = 1.0f;

		// Near Lower Right
		nearLR.x = xMax; nearLR.y = yMin; nearLR.z = zMin; nearLR.w = 1.0f;

		// Far Upper Left
		farUL.x = xMin; farUL.y = yMax; farUL.z = zMax; farUL.w = 1.0f;

		// Far Lower Left
		farLL.x = xMin; farLL.y = yMin; farLL.z = zMax; farLL.w = 1.0f;

		// Far Upper Right
		farUR.x = xMax; farUR.y = yMax; farUR.z = zMax; farUR.w = 1.0f;

		// Far Lower Right
		farLR.x = xMax; farLR.y = yMin; farLR.z = zMax; farLR.w = 1.0f;
	}


	// Calculates the corners of the Frustum and sets the projection matrix.
	// Perspective Matrix Projection
	void SetPerspective(float fFov, float fAspect, float fNear, float fFar)
	{
		float xmin, xmax, ymin, ymax;       // Dimensions of near clipping plane
		float xFmin, xFmax, yFmin, yFmax;   // Dimensions of far clipping plane

		// Do the Math for the near clipping plane
		ymax = fNear * float(tan( fFov * remath::pi / 360.0 ));
		ymin = -ymax;
		xmin = ymin * fAspect;
		xmax = -xmin;

		// Construct the projection matrix
		projMatrix.MakeIdentity();
		projMatrix(0,0) = (2.0f * fNear)/(xmax - xmin);
		projMatrix(1,1) = (2.0f * fNear)/(ymax - ymin);
		projMatrix(2,0) = (xmax + xmin) / (xmax - xmin);
		projMatrix(2,1) = (ymax + ymin) / (ymax - ymin);
		projMatrix(2,2) = -((fFar + fNear)/(fFar - fNear));
		projMatrix(2,3) = -1.0f;
		projMatrix(3,2) = -((2.0f * fFar * fNear)/(fFar - fNear));
		projMatrix(3,3) = 0.0f;

		// Do the Math for the far clipping plane
		yFmax = fFar * float(tan(fFov * remath::pi / 360.0));
		yFmin = -yFmax;
		xFmin = yFmin * fAspect;
		xFmax = -xFmin;


		// Fill in values for untransformed Frustum corners
		// Near Upper Left
		nearUL.x = xmin; nearUL.y = ymax; nearUL.z = -fNear; nearUL.w = 1.0f;

		// Near Lower Left
		nearLL.x = xmin; nearLL.y = ymin; nearLL.z = -fNear; nearLL.w = 1.0f;

		// Near Upper Right
		nearUR.x = xmax; nearUR.y = ymax; nearUR.z = -fNear; nearUR.w = 1.0f;

		// Near Lower Right
		nearLR.x = xmax; nearLR.y = ymin; nearLR.z = -fNear; nearLR.w = 1.0f;

		// Far Upper Left
		farUL.x = xFmin; farUL.y = yFmax; farUL.z = -fFar; farUL.w = 1.0f;

		// Far Lower Left
		farLL.x = xFmin; farLL.y = yFmin; farLL.z = -fFar; farLL.w = 1.0f;

		// Far Upper Right
		farUR.x = xFmax; farUR.y = yFmax; farUR.z = -fFar; farUR.w = 1.0f;

		// Far Lower Right
		farLR.x = xFmax; farLR.y = yFmin; farLR.z = -fFar; farLR.w = 1.0f;
	}


	// Builds a transformation matrix and transforms the corners of the Frustum,
	// then derives the plane equations
//	void Transform(GLFrame& Camera)
//	{
//		// Workspace
//		M3DMatrix44f rotMat;
//		M3DVector3f vForward, vUp, vCross;
//		M3DVector3f   vOrigin;

//		///////////////////////////////////////////////////////////////////
//		// Create the transformation matrix. This was the trickiest part
//		// for me. The default view from OpenGL is down the negative Z
//		// axis. However, building a transformation axis from these
//		// directional vectors points the frustum the wrong direction. So
//		// You must reverse them here, or build the initial frustum
//		// backwards - which to do is purely a matter of taste. I chose to
//		// compensate here to allow better operability with some of my other
//		// legacy code and projects. RSW
//		Camera.GetForwardVector(vForward);
//		vForward.x = -vForward.x;
//		vForward.y = -vForward.y;
//		vForward.z = -vForward.z;

//		Camera.GetUpVector(vUp);
//		Camera.GetOrigin(vOrigin);

//		// Calculate the right side (x) vector
//		m3dCrossProduct3(vCross, vUp, vForward);

//		// The Matrix
//		// X Column
//		memcpy(rotMat, vCross, sizeof(float)*3);
//		rotMat[3] = 0.0f;

//		// Y Column
//		memcpy(&rotMat[4], vUp, sizeof(float)*3);
//		rotMat[7] = 0.0f;

//		// Z Column
//		memcpy(&rotMat[8], vForward, sizeof(float)*3);
//		rotMat[11] = 0.0f;

//		// Translation
//		rotMat[12] = vOrigin.x;
//		rotMat[13] = vOrigin.y;
//		rotMat[14] = vOrigin.z;
//		rotMat[15] = 1.0f;

//		////////////////////////////////////////////////////
//		// Transform the frustum corners
//		m3dTransformVector4(nearULT, nearUL, rotMat);
//		m3dTransformVector4(nearLLT, nearLL, rotMat);
//		m3dTransformVector4(nearURT, nearUR, rotMat);
//		m3dTransformVector4(nearLRT, nearLR, rotMat);
//		m3dTransformVector4(farULT, farUL, rotMat);
//		m3dTransformVector4(farLLT, farLL, rotMat);
//		m3dTransformVector4(farURT, farUR, rotMat);
//		m3dTransformVector4(farLRT, farLR, rotMat);

//		////////////////////////////////////////////////////
//		// Derive Plane Equations from points... Points given in
//		// counter clockwise order to make normals point inside
//		// the Frustum
//		// Near and Far Planes
//		m3dGetPlaneEquation(nearPlane, nearULT, nearLLT, nearLRT);
//		m3dGetPlaneEquation(farPlane, farULT, farURT, farLRT);

//		// Top and Bottom Planes
//		m3dGetPlaneEquation(topPlane, nearULT, nearURT, farURT);
//		m3dGetPlaneEquation(bottomPlane, nearLLT, farLLT, farLRT);

//		// Left and right planes
//		m3dGetPlaneEquation(leftPlane, nearLLT, nearULT, farULT);
//		m3dGetPlaneEquation(rightPlane, nearLRT, farLRT, farURT);
//	}


// Allow expanded version of sphere test
//	bool TestSphere(float x, float y, float z, float fRadius)
//	{
//		M3DVector3f vPoint;
//		vPoint.x = x;
//		vPoint.y = y;
//		vPoint.z = z;

//		return TestSphere(vPoint, fRadius);
//	}

	// Test a point against all frustum planes. A negative distance for any
	// single plane means it is outside the frustum. The radius value allows
	// to test for a point (radius = 0), or a sphere. Possibly there might
	// be some gain in an alternative function that saves the addition of
	// zero in this case.
	// Returns false if it is not in the frustum, true if it intersects
	// the Frustum.
//	bool TestSphere(M3DVector3f vPoint, float fRadius)
//	{
//		float fDist;

//		// Near Plane - See if it is behind me
//		fDist = m3dGetDistanceToPlane(vPoint, nearPlane);
//		if (fDist + fRadius <= 0.0)
//			return false;

//		// Distance to far plane
//		fDist = m3dGetDistanceToPlane(vPoint, farPlane);
//		if (fDist + fRadius <= 0.0)
//			return false;

//		fDist = m3dGetDistanceToPlane(vPoint, leftPlane);
//		if (fDist + fRadius <= 0.0)
//			return false;

//		fDist = m3dGetDistanceToPlane(vPoint, rightPlane);
//		if (fDist + fRadius <= 0.0)
//			return false;

//		fDist = m3dGetDistanceToPlane(vPoint, bottomPlane);
//		if (fDist + fRadius <= 0.0)
//			return false;

//		fDist = m3dGetDistanceToPlane(vPoint, topPlane);
//		if (fDist + fRadius <= 0.0)
//			return false;

//		return true;
//	}

protected:
	// The projection matrix for this frustum
	remath::Matrix4f projMatrix;

	// Untransformed corners of the frustum
	remath::Vector4f nearUL, nearLL, nearUR, nearLR;
	remath::Vector4f farUL,  farLL,  farUR,  farLR;

	// Transformed corners of Frustum
	remath::Vector4f nearULT, nearLLT, nearURT, nearLRT;
	remath::Vector4f farULT,  farLLT,  farURT,  farLRT;

	// Base and Transformed plane equations
	remath::Vector4f nearPlane, farPlane, leftPlane, rightPlane;
	remath::Vector4f topPlane, bottomPlane;
};

} // end of namespace 'render'
