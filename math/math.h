#pragma once

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"
#include "rect.h"

/**
 A class that contains mathematics-related static helper methods and constants.
*/

namespace remath {

const float pi = 3.14159265358979323846f;
const float pi2 = 2.0f * pi;
const float pi_div_180  = 0.017453292519943296f;
const float inv_pi_div_180  = 57.2957795130823229f;

inline float deg2rad(float x)
{
	return x * pi_div_180;
}

inline float rad2deg(float x)
{
	return x * inv_pi_div_180;
}

// Hour angles
//#define m3dHrToDeg(x)	((x) * (1.0 / 15.0))
//#define m3dHrToRad(x)	m3dDegToRad(m3dHrToDeg(x))

//#define m3dDegToHr(x)	((x) * 15.0))
//#define m3dRadToHr(x)	m3dDegToHr(m3dRadToDeg(x))

class Math
{
public:
	inline static float calcPosInt(int value, int limit1, int limit2)
	{
		if (value <= limit1)
			return 0;
		if (value >= limit2)
			return 1;
		else
			return ((float)(value-limit1))/(limit2-limit1);
	}
};

} // end of namespace 'remath'
