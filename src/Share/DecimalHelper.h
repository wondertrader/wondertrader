#pragma once
#include <math.h>

namespace DecimalHelper
{
	const double EPSINON = 1e-6;

	inline bool equal(double a, double b = 0.0)
	{
		return(fabs(a - b) < EPSINON);
	}

	inline bool gt(double a, double b)
	{
		return a - b > EPSINON;
	}

	inline bool lt(double a, double b)
	{
		return b - a > EPSINON;
	}

	inline bool ge(double a, double b)
	{
		return gt(a, b) || equal(a, b);
	}

	inline bool le(double a, double b)
	{
		return lt(a, b) || equal(a, b);
	}
	
};