#include <iostream>

#include "vector3.h"

namespace remath {

template <typename T>
struct Vector4
{
	union
	{
		T x;
		T r;
	};

	union
	{
		T y;
		T g;
	};

	union
	{
		T z;
		T b;
	};

	union
	{
		T w;
		T a;
	};

	Vector4():x(0),y(0),z(0),w(1) {}
	Vector4(const T& x, const T& y, const T& z, const T& w):x(x),y(y),z(z),w(w) {}
	Vector4(const Vector3<T>& v):x(v.x),y(v.y),z(v.z),w(1) {}
	Vector4(const Vector3<T>& v, const T& w):x(v.x),y(v.y),z(v.z),w(w) {}

	void normalise()
	{
		float norm = sqrt(this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w);

		if ( norm != 0 )
		{
			this.x /= norm;
			this.y /= norm;
			this.z /= norm;
			this.w /= norm;
		}
	}
/*
	Vector4<T> cross(const Vector4<T>& v)
	{
		return Vector4<T>(v.y * v.z - v.z * v.y,
						  v.z * v.x - v.x * v.z,
						  v.x * v.y - v.y * v.x);
	}
*/
	float dot(const Vector4<T>& v)
	{
		return this.x * v.x + this.y * v.y + this.z * v.z + this.w * v.w;
	}

	Vector4<T> operator+(const Vector4<T>& v)
	{
		return Vector4<T>(v.x + this->x,
						  v.y + this->y,
						  v.z + this->z,
						  v.w + this->w);
	}

	Vector4<T> operator-(const Vector4<T>& v)
	{
		return Vector4<T>(this->x - v.x,
						  this->y - v.y,
						  this->z - v.z,
						  this->w - v.w);
	}

	Vector4<T> operator*(float value)
	{
		return Vector4<T>(this->x * value,
						  this->y * value,
						  this->z * value,
						  this->w * value);

	}

	template <typename U>
	friend std::ostream& operator<<(std::ostream& str, const Vector4<U>& v);
};

template <typename T>
Vector4<T> operator+(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector4<T>(v1.x + v2.x,
					  v1.y + v2.y,
					  v1.z + v2.z,
					  v1.w - v2.w);
}

template <typename T>
Vector4<T> operator-(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector4<T>(v1.x - v2.x,
					  v1.y - v2.y,
					  v1.z - v2.z,
					  v1.w - v2.w);
}

template <typename T>
Vector4<T> operator*(const Vector4<T>& v, float value)
{
	return Vector4<T>(v.x * value,
					  v.y * value,
					  v.z * value,
					  v.w * value);
}

template <typename U>
std::ostream& operator<<(std::ostream& str, const Vector4<U>& v)
{
	str << "X: " << v.x << " Y: " << v.y << " Z: " << v.z << " W: " << v.w;
	return str;
}

typedef Vector4<float> Vector4f;
typedef Vector4<int>   Vector4i;
typedef Vector4<float> Color4;

} // end of namespace 'remath'
