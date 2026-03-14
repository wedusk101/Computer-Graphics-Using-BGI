#pragma once
#ifndef MATH_UTILS_H__
#define MATH_UTILS_H__

#define PI 3.1451926535
#define TWO_PI 6.283185307

inline float degreesToRadians(float val)
{
	return val / 57.3248407;
}

inline float radiansToDegrees(float val)
{
	return val * 57.3248407;
}

#endif // MATH_UTILS_H__
