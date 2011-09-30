#pragma once

#include "vector3.h"
#include <cstring>
#include <cmath>
#include <cstdio>

#include "util/debug.h"

//#include "render/GLTools/math3d.h"

/**
 Column major matrix class
 */

namespace remath {

template<typename T>
class Matrix4
{
private:
	T m[4][4]; // First index is column number, second one is row number
public:
	Matrix4() { ZeroMatrix(); }
	Matrix4(const Matrix4& rhs) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] = rhs.m[j][i];
	}
	Matrix4(const T* A) {
		for(int i = 0, k = 0; i < 4; i++)
			for(int j = 0; j < 4; j++, k++)
				m[i][j] = A[k];
	}
	~Matrix4() {}
	void ZeroMatrix() {
		memset(m, 0, 16 * sizeof(T));
	}
	void MakeIdentity() {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] = (i == j);
	}
	static Matrix4 Identity() {
		Matrix4 M;

		M.MakeIdentity();
		return M;
	}
	Matrix4& operator+(const Matrix4& rhs) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] += rhs.m[j][i];
		return *this;
	}
	Matrix4& operator-(const Matrix4& rhs) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] -= rhs.m[j][i];
		return *this;
	}
	Matrix4& operator*(float r) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] *= r;
		return *this;
	}
	Matrix4& operator/(float r) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] /= r;
		return *this;
	}
	friend Matrix4& operator*(float r, Matrix4& M) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				M.m[j][i] *= r;
		return M;
	}
	Matrix4& operator*(const Matrix4& rhs) {
		float t[4][4] = {{0}};

		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				for(int k = 0; k < 4; k++)
					t[j][i] += m[k][i] * rhs.m[j][k];
			}
		//TODO: this sucks: matrix spoils; ok for transformation, etc
		memcpy(m, t, 16 * sizeof(T));
		return *this;
	}
	//	friend Matrix4& operator*(const M3DMatrix44f& lhs, Matrix4& M) {
	//		float t[4][4] = {{0}};

	//		for(int i = 0; i < 4; i++)
	//			for(int j = 0; j < 4; j++)
	//			{
	//				for(int k = 0; k < 4; k++)
	//					t[i][j] += lhs[i * 4 + k] * M.m[k][j];
	//			}
	//		memcpy(M.m, t, 16 * sizeof(T));
	//		return M;
	//	}
	Matrix4& operator=(const Matrix4& rhs) {
		if(this != &rhs)
			memcpy(m, rhs.m, 16 * sizeof(T));
		return *this;
	}
	void Transpose() {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[j][i] = m[i][j];
	}
	static Matrix4 Translation(const Vector3<T> &V)
	{
		Matrix4 M;

		M.MakeIdentity();
		M.m[3][0] = V.x;
		M.m[3][1] = V.y;
		M.m[3][2] = V.z;
		return M;
	}
	static Matrix4 Translation(float tx, float ty, float tz)
	{
		Matrix4 M;

		M.MakeIdentity();
		M.m[3][0] = tx;
		M.m[3][1] = ty;
		M.m[3][2] = tz;
		return M;
	}
	static Matrix4 RotationX(float ang) {
		Matrix4 M;

		M.MakeIdentity();
		float c = cosf(ang), s = sinf(ang);
		M.m[1][1] = c; M.m[2][1] = -s;
		M.m[1][2] = s; M.m[2][2] =  c;
		return M;
	}
	static Matrix4 RotationY(float ang) {
		Matrix4 M;

		M.MakeIdentity();
		float c = cosf(ang), s = sinf(ang);
		M.m[0][0] =  c; M.m[2][0] = s;
		M.m[0][2] = -s; M.m[2][2] = c;
		return M;
	}
	static Matrix4 RotationZ(float ang) {
		Matrix4 M;

		M.MakeIdentity();
		float c = cosf(ang), s = sinf(ang);
		M.m[0][0] = c; M.m[1][0] = -s;
		M.m[0][1] = s; M.m[1][1] =  c;
		return M;
	}
	static Matrix4 RotationWithAxis(const Vector3<T> &axis, float ang) {
		Matrix4 M;

		M.MakeIdentity();
		float c = cosf(ang), s = sinf(ang);
		//		Vector3<T> norm_axis = axis.get_normalized();
		float l = axis.length();
		float ax = axis.x / l, ay = axis.y / l, az = axis.z / l;
		M.m[0][0] = c + ax * ax * (1 - c);
		M.m[1][0] = ax * ay * (1 - c) - az * s;
		M.m[2][0] = ax * az * (1 - c) + ay * s;
		M.m[0][1] = ay * ax * (1 - c) + az * s;
		M.m[1][1] = c + ay * ay * (1 - c);
		M.m[2][1] = ay * az * (1 - c) - ax * s;
		M.m[0][2] = az * ax * (1 - c) - ay * s;
		M.m[1][2] = az * ay * (1 - c) + ax * s;
		M.m[2][2] = c + az * az * (1 - c);
		return M;
	}
	static Matrix4 Scaling(const Vector3<T> &S) {
		Matrix4 M;

		M.MakeIdentity();
		M.m[0][0] = S.x; M.m[1][1] = S.y; M.m[2][2] = S.z;
		return M;
	}
	static Matrix4 Scaling(float sx, float sy, float sz) {
		Matrix4 M;

		M.MakeIdentity();
		M.m[0][0] = sx; M.m[1][1] = sy; M.m[2][2] = sz;
		return M;
	}

	// Create a projection matrix
	// Similiar to the old gluPerspective... fov is in radians btw...
	void MakePerspective(float fFov, float fAspect, float zMin, float zMax) {
		MakeIdentity(); // Fastest way to get most valid values already in place

		float yMax = zMin * tanf(fFov * 0.5f);
		float yMin = -yMax;
		float xMin = yMin * fAspect;
		float xMax = -xMin;

//		m[0]
		m[0][0] = (2.0f * zMin) / (xMax - xMin);
//		m[5]
		m[1][1] = (2.0f * zMin) / (yMax - yMin);
//		m[8]
		m[2][0] = (xMax + xMin) / (xMax - xMin);
//		m[9]
		m[2][1] = (yMax + yMin) / (yMax - yMin);
//		m[10]
		m[2][2] = -((zMax + zMin) / (zMax - zMin));
//		m[11]
		m[2][3] = -1.0f;
//		m[14]
		m[3][2] = -((2.0f * (zMax*zMin))/(zMax - zMin));
//		m[15]
		m[3][3] = 0.0f;
	}

	// Make a orthographic projection matrix
	void MakeOrthographic(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
		MakeIdentity();

		m[0][0] = 2.0f / (xMax - xMin);
		m[1][1] = 2.0f / (yMax - yMin);
		m[2][2] = -2.0f / (zMax - zMin);
		m[3][0] = -((xMax + xMin)/(xMax - xMin));
		m[3][1] = -((yMax + yMin)/(yMax - yMin));
		m[3][2] = -((zMax + zMin)/(zMax - zMin));
		m[3][3] = 1.0f;
	}

	void Invert() {
		T l[4][4] = {{0}}, u[4][4] = {{0}}, inv_m[4][4] = {{0}};

		for(int j = 0; j < 4; j++)
			u[0][j] = m[0][j];
		for(int i = 1; i < 4; i++)
			l[i][0] = m[i][0] / u[0][0];
		for(int i = 0; i < 4; i++)
			l[i][i] = 1;
		for(int i = 1; i < 4; i++)
			for(int j = 1; j < 4; j++)
			{
				if(i <= j)
				{
					u[i][j] = m[j][i];
					T sum = 0;
					for(int k = 0; k < i; k++)
						sum += l[i][k] * u[k][j];
					u[i][j] -= sum;
				}
				else
				{
					l[i][j] = m[j][i];
					T sum = 0;
					for(int k = 0; k < j; k++)
						sum += l[i][k] * u[k][j];
					l[i][j] -= sum;
					l[i][j] /= u[j][j];
				}
			}
		for(int i = 3; i >= 0; i--)
			for(int j = 3; j >= 0; j--)
			{
				if(i == j)
				{
					T sum = 0;
					for(int k = j + 1; k < 4; k++)
						sum += u[j][k] * inv_m[k][j];
					inv_m[j][j] = (1 - sum) / u[j][j];
				}
				else if(i < j) {
					T sum = 0;
					for(int k = i + 1; k < 4; k++)
						sum += u[i][k] * inv_m[k][j];
					inv_m[j][i] = -sum / u[i][i];
				}
				else
				{
					T sum = 0;
					for(int k = j + 1; k < 4; k++)
						sum += inv_m[i][k] * l[k][j];
					inv_m[j][i] = -sum;
				}
			}
		memcpy(m, inv_m, 16 * sizeof(T));
	}
	static Matrix4 Inverse(const Matrix4 &rhs) {
		Matrix4 M;

		M = rhs;
		M.Invert();
		return M;
	}
//	T& operator[](int i) {
//		return (T &)((T *)m[0] + i);
//		return (T &)(m[i][j]);
//	}
	T& operator()(int i, int j) {
		return m[i][j];
	}
	T* ptr() { return (T *)m[0]; }

	void print()
	{
//		for (int i = 0; i < 16; i++)
//			reutil::g_debug << *(float *)m[i] << std::endl;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				reutil::g_debug << m[j][i] << std::endl;
	}
};

typedef Matrix4<float> Matrix4f;

} // end of namespace 'remath'
