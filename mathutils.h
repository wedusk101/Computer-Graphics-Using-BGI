#pragma once
#ifndef MATH_UTILS_H__
#define MATH_UTILS_H__

#include "primitives.h"

#define PI 3.1451926535
#define TWO_PI 6.283185307

Point getTranslatedPoint(const Point&, const int&, const int&); // returns a point P(x,y) translated by tx and ty
Point getRotatedPoint(const Point& input, const float& theta, const Point& pivot); // rotates a point about a given pivot by an angle of theta radians

inline float degreesToRadians(float val)
{
	return val / 57.3248407;
}

inline float radiansToDegrees(float val)
{
	return val * 57.3248407;
}

#endif // MATH_UTILS_H__
