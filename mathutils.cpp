#include "mathutils.h"

Point getTranslatedPoint(const Point& p, const int& tx, const int& ty)
{
	Point translatedPoint;
	translatedPoint.x = p.x + tx;
	translatedPoint.y = p.y + ty;
	return translatedPoint;
}

Point getRotatedPoint(const Point& p, const float& theta, const Point& pivot)
{
	Point rotatedPoint;
	rotatedPoint.x = static_cast<int>(pivot.x + (p.x - pivot.x) * cos(theta) - (p.y - pivot.y) * sin(theta));
	rotatedPoint.y = static_cast<int>(pivot.y + (p.x - pivot.x) * sin(theta) + (p.y - pivot.y) * cos(theta));
	return rotatedPoint;
}