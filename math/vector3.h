#pragma once

#include "vector2.h"
#include <cmath>

namespace remath {

/**
	A simple three-dimensional vector class
*/

template <typename T>
struct Vector3
{
public:
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

	Vector3() : x(0), y(0), z(0) {}
	Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
	Vector3(const T& val) : x(val), y(val), z(val) {}
	Vector3(const Vector2<T>& v, const T& z = 0) : x(v.x), y(v.y), z(z) {}
	Vector3(const Vector3<T>& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}

	Vector3 crossProduct(const Vector3<T> &vect) const
	{
		T tx = y*vect.z-z*vect.y;
		T ty = x*vect.z-z*vect.x;
		T tz = x*vect.y-y*vect.x;
		return Vector3(tx, -ty, tz);
	}
	T dotProduct(const Vector3<T> &vect) const
	{
		return x*vect.x + y*vect.y + z*vect.z;
	}

	Vector3<T> minim(const Vector3<T> &vect) const
	{
		return Vector3<T>((vect.x < x ? vect.x : x),
						  (vect.y < y ? vect.y : y),
						  (vect.z < z ? vect.z : z));
	}
	Vector3<T> maxim(const Vector3<T> &vect) const
	{
		return Vector3<T>((vect.x > x ? vect.x : x),
						  (vect.y > y ? vect.y : y),
						  (vect.z > z ? vect.z : z));
	}
	Vector3<T> clamp(const Vector3<T> &min, const Vector3<T> &max) const
	{
		Vector3<T> t = this->minim(min);
		return t.maxim(max);
	}

	Vector3<T> operator + (const Vector3<T> &vect)
	{
		T tx, ty, tz;
		tx = x + vect.x;
		ty = y + vect.y;
		tz = z + vect.z;
		return Vector3<T>(tx, ty, tz);
	}
	Vector3<T> operator - (const Vector3<T> &vect)
	{
		T tx, ty, tz;
		tx = x - vect.x;
		ty = y - vect.y;
		tz = z - vect.z;
		return Vector3<T>(tx, ty, tz);
	}
	Vector3 operator += (const Vector3<T> &vect)
	{
		x += vect.x;
		y += vect.y;
		z += vect.z;
		return Vector3<T>(x, y, z);
	}
	Vector3 operator -= (const Vector3<T> &vect)
	{
		x -= vect.x;
		y -= vect.y;
		z -= vect.z;
		return Vector3<T>(x, y, z);
	}

	Vector3<T> operator * (const float &value)
	{
		float tx, ty, tz;
		tx = x * value;
		ty = y * value;
		tz = z * value;
		return Vector3<T>(tx, ty, tz);
	}
	Vector3<T> operator *= (float const &value)
	{
		x *= value;
		y *= value;
		z *= value;
		return Vector3<T>(x, y, z);
	}
	Vector3<T> operator / (const float &value)
	{
		if (value == 0) return Vector3<T>(0.0f, 0.0f, 0.0f);
		float tx, ty, tz;
		tx = x / value;
		ty = y / value;
		tz = z / value;
		return Vector3<T>(tx, ty, tz);
	}

	bool operator == (const Vector3<T> &vect)
	{
		return (vect.x == x && vect.y == y && vect.z == z);
	}
	Vector3<T>& operator = (const Vector3<T> &vect)
	{
		if (this == &vect)
			return *this;
		x = vect.x;
		y = vect.y;
		z = vect.z;
		return *this;
	}

	Vector3<T> normalize()
	{
		float l = length();
		if (l < 0.00001f && l > -0.00001f)
			return Vector3<T>(0, 0, 0);
		x /= l;
		y /= l;
		z /= l;
		return Vector3<T>(x, y, z);
	}

	Vector3<T> get_normalized() const
	{
		float l = length();
		if (l < 0.00001f && l > -0.00001f)
			return Vector3<T>(0, 0, 0);
		return Vector3<T>(x / l, y / l, z / l);
	}

//	void normalise()
//	{
//		float norm = sqrt(this.x * this.x + this.y * this.y + this.z * this.z);

//		if ( norm != 0 )
//		{
//			this.x /= norm;
//			this.y /= norm;
//			this.z /= norm;
//		}
//	}

	T squaredLength() const
	{
		return (x*x + y*y + z*z);
	}
	T length() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	friend class Matrix;
};

template <typename T>
Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(v1.x + v2.x,
					  v1.y + v2.y,
					  v1.z + v2.z);
}

template <typename T>
Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(v1.x - v2.x,
					  v1.y - v2.y,
					  v1.z - v2.z);
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& v, float value)
{
	return Vector3<T>(v.x * value,
					  v.y * value,
					  v.z * value);
}

template <typename U>
std::ostream& operator<<(std::ostream& str, const Vector3<U>& v)
{
	str << "X: " << v.x << " Y: " << v.y << " Z: " << v.z;
	return str;
}

typedef Vector3<float> Vector3f;
typedef Vector3<int>   Vector3i;
typedef Vector3<float> Color3;

} // end of namespace 'remath'
