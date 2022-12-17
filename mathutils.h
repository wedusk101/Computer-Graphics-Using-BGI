#pragma once
#ifndef MATH_UTILS_H__
#define MATH_UTILS_H__

#include "primitives.h"

void rotatePoint(Point& p, const float& theta, const int& pivotX, const int& pivotY); // rotates a point about a given pivot by an angle of theta radians
void translatePoint(Point&, const int&, const int&); // translates a point P(x,y) by tx and ty 
void scaleRay(Point&, const int&, const int&); // scales a point P(x,y) by sx and sy
void shearRay(Point&, const int&, const int&); // shears a point P(x,y) by shX and shY

Point getTranslatedPoint(const Point&, const int&, const int&); // returns a point P(x,y) translated by tx and ty
Point getRotatedPoint(const Point&, const float&, const int&, const int&); // rotates a point about a given pivot by an angle of theta radians

#endif // MATH_UTILS_H__
