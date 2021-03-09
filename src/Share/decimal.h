/*!
 * \file decimal.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 浮点数辅助类,主要用于浮点数据的比较
 */
#pragma once
#include <math.h>

namespace decimal
{
	const double EPSINON = 1e-6;

	inline double rnd(double v, int exp = 1)
	{
		return round(v*exp) / exp;
	}

	inline bool eq(double a, double b = 0.0)
	{
		return(fabs(a - b) < EPSINON);
	}

	inline bool gt(double a, double b = 0.0)
	{
		return a - b > EPSINON;
	}

	inline bool lt(double a, double b = 0.0)
	{
		return b - a > EPSINON;
	}

	inline bool ge(double a, double b = 0.0)
	{
		return gt(a, b) || eq(a, b);
	}

	inline bool le(double a, double b = 0.0)
	{
		return lt(a, b) || eq(a, b);
	}

	inline double mod(double a, double b)
	{
		return a / b - round(a / b);
	}
	
};