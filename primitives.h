//primitives.h
#ifndef PRIMITIVES__H_
#define PRIMITIVES__H_

#include <cmath>
#include "graphics.h"

struct Point
{
	int x;
	int y;

	Point() : x(0), y(0) {}
	Point(const int &x_, const int &y_) : x(x_), y(y_) {}
};

Point getMidpoint(const Point& p1, const Point& p2)
{
	Point out = {};
	out.x = static_cast<int>(round(0.5 * ((double)p1.x + (double)p2.x)));
	out.y = static_cast<int>(round(0.5 * ((double)p1.y + (double)p2.y)));
	return out;
}

struct Line
{
	Point src;
	Point dst;

	Line(const Point& src_, const Point& dst_) : src(src_), dst(dst_) {}
	Line(int x1, int y1, int x2, int y2) : src(Point(x1, y1)), dst(Point(x2, y2)) {}

	void draw() const
	{
		line(src.x, src.y, dst.x, dst.y);
	}
};

typedef struct
{
	Point center;
	int radius;
} Circle;

typedef struct
{
	Point a;
	Point b;
	Point c;
} Triangle;

struct Vec2
{
	double x;
	double y;

	Vec2() : x(0), y(0) {}		
	Vec2(int x_, int y_) : x(round(x_)), y(round(y_)) {} // member initialization list
	Vec2(double x_, double y_) :  x(x_), y(y_) {}

	Vec2 operator+ (const Vec2 &v) const
	{
		Vec2 result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}

	Vec2 operator- (const Vec2 &v) const
	{
		Vec2 result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}

	Vec2 operator* (const double &c) const // scalar multiplication
	{
		Vec2 result;
		result.x = x * c;
		result.y = y * c;
		return result;
	}

	Vec2 operator/ (const double &c) const // scalar division
	{
		Vec2 result;
		result.x = x / c;
		result.y = y / c;
		return result;
	}

	double getMagnitude() const
	{
		return sqrt(x * x + y * y);
	}

	double dot(const Vec2 &v) const
	{
		return ((double)x * (double)v.x) + ((double)y * (double)v.y);
	}

	Vec2 getNormalized() const
	{
		Vec2 result;
		double mag = this->getMagnitude();

		if (mag > 0)
		{
			result.x = x / mag;
			result.y = y / mag;
		}
		return result;
	}

	Vec2 getNormal() // returns the outward unit normal vector to a given vector following the left to right convention (counter-clockwise rotation)
	{
		return Vec2(y, -x).getNormalized();
	}
};

Vec2 points2Vec(const Point &src, const Point &dst) // returns a vector from a source point towards a destination point
{
	return Vec2(dst.x - src.x, dst.y - src.y);
}

Point vec2Point(const Vec2 &v)
{
	return Point(static_cast<int>(round(v.x)), static_cast<int>(round(v.y)));
}

#endif