#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include "graphics.h"
#include "colors.h"

typedef struct Vec2
{
	double x;
	double y;

	Vec2() {}
	Vec2(double x_, double y_) : x(x_), y(y_) {}

	inline Vec2 operator*(double c)
	{
		return Vec2(x * c, y * c);
	}

	inline Vec2 operator/(double c)
	{
		return Vec2(x / c, y / c);
	}

	Vec2 getNormalized() const
	{
		double mag = sqrt(x * x + y * y);
		if (fabs(mag) < 1e-4)
			return Vec2();
		return Vec2(x / mag, y / mag);
	}
} Vec2;

typedef struct Point
{
	double x;
	double y;

	Point() {}
	Point(double x_, double y_) : x(x_), y(y_) {}

	inline Vec2 operator-(const Point &p) const
	{
		return Vec2(x - p.x, y - p.y);
	}

	inline Point operator+(const Vec2 &v) const
	{
		return Point(x + v.x, y + v.y);
	}
} Point;

inline double getEuclideanDist(const Point &p1, const Point &p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

bool isValidPos(const Point &p, const double minRadius, const std::vector<Point> &listPoints)
{
	for (const auto point : listPoints)
		if (getEuclideanDist(p, point) < minRadius)
			return false;
	return true;
}

// calculates the intersection between two circles
bool intersects(const Point &c1, const Point &c2, double radius, Point &p1, Point &p2)
{
	double d = getEuclideanDist(c1, c2);
	if (d >= 2 * radius || fabs(d) < 1e-4)
		return false;
	else
	{		
		double oc1 = d / 2;
		double op1 = sqrt(radius * radius - oc1 * oc1);
		Point o = c1 + ((c2 - c1) * (oc1 / d));
		Vec2 oc = c1 - o;
		Vec2 n1 = Vec2(-oc.y, oc.x).getNormalized();
		Vec2 n2 = Vec2(oc.y, -oc.x).getNormalized();
		p1 = o + n1 * op1;
		p2 = o + n2 * op1;
		return true;
	}
}

void drawVoronoi(size_t maxPoints, double radius)
{
	std::vector<Point> siteList;
	std::default_random_engine seed;
	std::uniform_real_distribution<double> rnd(0, 640);
	Point src, dst;

	// generate sites
	while (siteList.size() < maxPoints)
	{
		double x = rnd(seed);
		double y = rnd(seed);
		Point site(x, y);		

		// Poisson disk sampling
		if (isValidPos(site, radius, siteList))
		{
			siteList.push_back(site);
			putpixel((int)site.x, (int)site.y, LIGHTRED);
			setcolor(CYAN);
			circle((int)site.x, (int)site.y, 5);
		}
	}

	setcolor(YELLOW);

	for (double r = radius; r < 2 * radius; r += 2)
	{
		for (const auto point : siteList)
		{
			for (const auto site : siteList)
			{
				if (intersects(site, point, r, src, dst))
					line((int)src.x, (int)src.y, (int)dst.x, (int)dst.y);
			}
		}
	}
}

int main()
{
	initwindow(640, 480, "Voronoi");
	double minSiteDist = 0;
	size_t maxPoints = 0, ch = 0;
	while (true)
	{
		std::cout << "This program creates a Voronoi parition pattern." << std::endl;
		std::cout << "How many sites(points) would you like to have on your partition?" << std::endl;
		std::cin >> maxPoints;
		std::cout << "Please enter the minimum site distance between points(radius)." << std::endl;
		std::cin >> minSiteDist;
		drawVoronoi(maxPoints, minSiteDist);
		std::cout << "Continue? (1 = Yes / 0 = No)" << std::endl;
		std::cin >> ch;
		if (ch == 0)
			break;
		std::cout << "\n\n";
		cleardevice();
	}
	std::cout << "Thank you." << std::endl;
	system("pause");
	closegraph();
	return 0;
}
