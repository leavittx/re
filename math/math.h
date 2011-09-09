#pragma once

/**
	A class that contains mathematics-related static helper methods.
*/

namespace remath {

class Math
{
public:
	static float calcPosInt(int value, int limit1, int limit2)
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
