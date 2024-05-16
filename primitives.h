//primitives.h
#ifndef PRIMITIVES__H_
#define PRIMITIVES__H_

#include <cmath>
#include "graphics.h"

struct Vec2
{
	float x;
	float y;

	Vec2() : x(0), y(0) {}		
	Vec2(int x_, int y_) : x(round(x_)), y(round(y_)) {} // member initialization list
	Vec2(float x_, float y_) :  x(x_), y(y_) {}

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

	Vec2 operator* (const float &c) const // scalar multiplication
	{
		Vec2 result;
		result.x = x * c;
		result.y = y * c;
		return result;
	}

	Vec2 operator/ (const float &c) const // scalar division
	{
		Vec2 result;
		result.x = x / c;
		result.y = y / c;
		return result;
	}

	inline float getMagnitude() const
	{
		return sqrt(x * x + y * y);
	}

	inline float dot(const Vec2 &v) const
	{
		return (x * v.x) + (y * v.y);
	}

	Vec2 getNormalized() const
	{
		Vec2 result;
		float mag = this->getMagnitude();

		if (mag > 0)
		{
			result.x = x / mag;
			result.y = y / mag;
		}
		return result;
	}

	inline Vec2 getNormal() // returns the outward unit normal vector to a given vector following the left to right convention (counter-clockwise rotation)
	{
		return Vec2(y, -x).getNormalized();
	}
};

struct Point
{
	int x;
	int y;

	Point() : x(0), y(0) {}
	Point(const int &x_, const int &y_) : x(x_), y(y_) {}

	Point(const Point&) = default;

	Point operator+(const Point& p)
	{
		return Point(x + p.x, y + p.y);
	}

	Point operator*(int c)
	{
		return Point(x * c, y * c);
	}

	Point& operator=(const Point& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	Vec2 operator-(const Point& p) const
	{
		return Vec2(x - p.x, y - p.y);
	}
};

inline Vec2 points2Vec(const Point& src, const Point& dst) // returns a vector from a source point towards a destination point
{
	return Vec2(dst.x - src.x, dst.y - src.y);
}

inline Point vec2Point(const Vec2& v)
{
	return Point(static_cast<int>(round(v.x)), static_cast<int>(round(v.y)));
}

inline Point getMidpoint(const Point& p1, const Point& p2)
{
	Point out = {};
	out.x = static_cast<int>(round(0.5 * (p1.x + p2.x)));
	out.y = static_cast<int>(round(0.5 * (p1.y + p2.y)));
	return out;
}

struct Line
{
	Point src;
	Point dst;

	Line(const Point& src_, const Point& dst_) : src(src_), dst(dst_) {}
	Line(int x1, int y1, int x2, int y2) : src(Point(x1, y1)), dst(Point(x2, y2)) {}

	Line& operator=(const Line& newLine)
	{
		src = newLine.src;
		dst = newLine.dst;
		return *this;
	}

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

struct Ray
{
	Ray(const Point& o_, const Vec2& d_) : o(o_), d(d_) {}

	Point getPointAt(float t)
	{
		return o + vec2Point(d) * t;
	}

	Point o;
	Vec2 d;
};

#endif