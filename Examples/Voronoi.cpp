
#define NOMINMAX

#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <limits>
#include "graphics.h"
#include "colors.h"

constexpr double EPSILON = 1e-4;
constexpr double DOUBLE_MIN = std::numeric_limits<double>::lowest();
constexpr double DOUBLE_MAX = std::numeric_limits<double>::max();
static size_t pointCount = 0;

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

	inline double getMagnitude() const
	{
		return sqrt(x * x + y * y);
	}

	Vec2 getNormalized() const
	{
		double mag = getMagnitude();
		if (fabs(mag) < EPSILON)
			return Vec2();
		return Vec2(x / mag, y / mag);
	}
} Vec2;

typedef struct Point
{
	double x;
	double y;

	size_t pointID;

	Point() {}
	Point(double x_, double y_) : x(x_), y(y_)
	{
		pointID = ++pointCount;
	}

	inline Vec2 operator-(const Point &p) const
	{
		return Vec2(x - p.x, y - p.y);
	}

	inline Point operator+(const Vec2 &v) const
	{
		return Point(x + v.x, y + v.y);
	}

	inline bool operator==(const Point &p) const
	{
		return fabs(x - p.x) < EPSILON && fabs(y - p.y) < EPSILON;
	}
} Point;

typedef struct Edge
{
	Point src;
	Point dst;

	Edge(const Point &s_, const Point &d_) : src(s_), dst(d_) {}

	inline bool operator==(const Edge &e) const
	{
		return (src == e.src && dst == e.dst) || (dst == e.src && src == e.dst);
	}
} Edge;

typedef struct Cell
{
	Point site;
	std::vector<Edge> boundaryList;
} Cell;

typedef struct Triangle
{
	Point a;
	Point b;
	Point c;

	Edge e1;
	Edge e2;
	Edge e3;

	Triangle(const Point &a_, const Point &b_, const Point &c_) : a(a_), b(b_), c(c_), e1(Edge(a, b)), e2(Edge(b, c)), e3(Edge(c, a)) {}
	Triangle(const Edge &e1_, const Edge &e2_, const Edge &e3_) : e1(e1_), e2(e2_), e3(e3_), a(e1_.src), b(e2_.src), c(e3_.src) {}

	// t[0] = e1, t[1] = e2, t[2] = e3
	inline Edge& operator[](int i)
	{
		return (i == 0) ? e1 : (i == 1) ? e2 : e3;
	}

	void draw(uint8_t color) const
	{
		setcolor(color);
		line(a.x, a.y, b.x, b.y);
		line(b.x, b.y, c.x, c.y);
		line(c.x, c.y, a.x, a.y);
	}

	void display() const
	{
		std::cout << "A(" << a.x << ", " << a.y << ")" << "	B(" << b.x << ", " << b.y << ")" << "	C(" << c.x << ", " << c.y << ")" << std::endl;
	}

	// returns whether two triangles are nighbors of each other
	bool isNeighbor(const Triangle &candidate) const
	{
		int count = 0;

		if (a == candidate.a || a == candidate.b || a == candidate.c)
			count++;
		if (b == candidate.a || b == candidate.b || b == candidate.c)
			count++;
		if (c == candidate.a || c == candidate.b || c == candidate.c)
			count++;

		return (count >= 2);
	}

	inline bool containsVertex(const Point &v) const
	{
		return (a == v || b == v || c == v);
	}

	inline bool containsEdge(const Edge &e) const
	{
		return (e == e1 || e == e2 || e == e3);
	}

	inline bool operator==(const Triangle &t) const
	{
		bool cond1 = (a == t.a && b == t.b && c == t.c);
		bool cond2 = (a == t.a && b == t.c && c == t.b);

		bool cond3 = (a == t.b && b == t.a && c == t.c);
		bool cond4 = (a == t.b && b == t.c && c == t.a);

		bool cond5 = (a == t.c && b == t.a && c == t.b);
		bool cond6 = (a == t.c && b == t.b && c == t.a);

		return (cond1 || cond2 || cond3 || cond4 || cond5 || cond6);
	}
} Triangle;

typedef struct Circle
{
	Point center;
	double radius;

	Circle() {}
	Circle(const Point &c, double r) : center(c), radius(r) {}
} Circle;

inline double getEuclideanDist(const Point &p1, const Point &p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// used for Possion disk sampling during the generation of sites
bool isValidPos(const Point &p, const double minRadius, const std::vector<Point> &listPoints)
{
	for (const auto point : listPoints)
		if (getEuclideanDist(p, point) < minRadius)
			return false;
	return true;
}

inline bool isInsideCircle(const Point &p, const Circle &c)
{
	return getEuclideanDist(p, c.center) < c.radius;
}

/*

// calculates the intersection between two circles
bool intersects(const Point &c1, const Point &c2, double radius, Point &p1, Point &p2)
{
	double d = getEuclideanDist(c1, c2);
	if (d >= 2 * radius || fabs(d) < EPSILON)
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

*/

Circle getCircumCircle(const Triangle &triangle)
{
	// triangle.display();

	double x = 0, y = 0, radius = 0, m1 = 0, m2 = 0, c1 = 0, c2 = 0;
	double num1 = triangle.a.x - triangle.c.x;
	double den1 = triangle.c.y - triangle.a.y;
	double num2 = triangle.b.x - triangle.c.x;
	double den2 = triangle.c.y - triangle.b.y;	

	if (fabs(num1) < EPSILON && fabs(num2) < EPSILON || fabs(den1) < EPSILON && fabs(den2) < EPSILON)
	{
		std::cerr << "Error! Degenerate triangle detected." << std::endl;
		return Circle();
	}
	
	// degenerate case for m1
	if (fabs(den1) < EPSILON)
	{
		m2 = num2 / den2;
		c2 = (triangle.c.y + triangle.b.y - ((triangle.b.x + triangle.c.x) * m2)) / 2;
		x = (triangle.a.x + triangle.c.x) / 2;
		y = m2 * x + c2;
		Point center(x, y);
		radius = getEuclideanDist(center, triangle.b);
		return Circle(center, radius);
	}

	// degenerate case for m2
	if (fabs(den2) < EPSILON)
	{
		m1 = num1 / den1;
		c1 = (triangle.c.y + triangle.a.y - ((triangle.a.x + triangle.c.x) * m1)) / 2;
		x = (triangle.a.x + triangle.c.x) / 2;
		y = m1 * x + c1;
		Point center(x, y);
		radius = getEuclideanDist(center, triangle.b);
		return Circle(center, radius);
	}

	m1 = num1 / den1;
	m2 = num2 / den2;
	c1 = (triangle.c.y + triangle.a.y - ((triangle.a.x + triangle.c.x) * m1)) / 2;
	c2 = (triangle.c.y + triangle.b.y - ((triangle.b.x + triangle.c.x) * m2)) / 2;
	x = (c2 - c1) / (m1 - m2);
	y = m1 * x + c1;
	Point center(x, y);
	radius = getEuclideanDist(center, triangle.a);
	return Circle(center, radius);
}

// Bowyer-Watson algorithm for Delaunay triangulation
std::vector<Triangle> triangulate(const std::vector<Point> &siteList)
{
	std::vector<Triangle> meshList;

	Triangle superTriangle(Point(DOUBLE_MAX, DOUBLE_MIN), Point(DOUBLE_MIN, DOUBLE_MAX), Point(DOUBLE_MAX, DOUBLE_MAX));	
	meshList.push_back(superTriangle);

	// for each site in the list of sites (i.e. the list of input points)
	for (auto siteItr = siteList.begin(); siteItr != siteList.end(); ++siteItr)
	{
		// list of non-Delaunay triangles
		std::vector<Triangle> invalidMeshList;	

		// for each triangle in the list of triangles to be returned
		for (auto itr = meshList.begin(); itr != meshList.end(); ++itr)
		{
			Circle circumCircle = getCircumCircle(*itr);
			if (isInsideCircle(*siteItr, circumCircle))
				invalidMeshList.push_back(*itr);
		}
		std::vector<Edge> boundaryList;

		// for each triangle in the list of invalid triangles
		for (auto &triangle : invalidMeshList)
		{
			for (int i = 0; i < 3; i++) // for each edge in each triangle
			{
				int match = 0;
				for (const auto &edgeTri : invalidMeshList)
					if (edgeTri.containsEdge(triangle[i]))
						match++;

				if (match == 1) // edge is unique 
					boundaryList.push_back(triangle[i]);
			}
		}

		// remove double edges by deleting duplicate triangles
		for (const auto &badTriangle : invalidMeshList)
			for (auto it = meshList.begin(); it != meshList.end(); ++it)
				if (*it == badTriangle)
					meshList.erase(it);

		// fill the polygonal hole by retriangulating it
		for (const auto &edge : boundaryList)
		{
			Triangle t(edge.src, edge.dst, *siteItr);
			meshList.push_back(t);
		}
	}

	// remove the super triangle
	for (auto it = meshList.begin(); it != meshList.end(); ++it)
		if (it->containsVertex(superTriangle.a) || it->containsVertex(superTriangle.b) || it->containsVertex(superTriangle.c))
			meshList.erase(it);

	return meshList;
}

std::vector<Point> generateSites(size_t maxPoints, double radius)
{
	std::vector<Point> siteList;
	std::default_random_engine seed;
	std::uniform_real_distribution<double> rnd(0, 640);
	Point src, dst;

	double x = rnd(seed);
	double y = rnd(seed);
	Point site(x, y); // add the first point

	while (siteList.size() < maxPoints)
	{
		site.x = rnd(seed);
		site.y = rnd(seed);

		// Poisson disk sampling
		if (isValidPos(site, radius, siteList)) // inefficient rejection sampling
		{
			siteList.push_back(site);
			putpixel((int)site.x, (int)site.y, LIGHTRED);
			setcolor(CYAN);
			circle((int)site.x, (int)site.y, 2);
		}
	}

	return siteList;
}

void drawMesh(const std::vector<Triangle> &meshList)
{
	for (const auto &triangle : meshList)
		triangle.draw(LIGHTGREEN);
}

// void drawVoronoi(std::vector<Triangle> &meshList)

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
		std::vector<Point> sites = generateSites(maxPoints, minSiteDist);
		std::vector<Triangle> mesh = triangulate(sites);
		drawMesh(mesh);
		std::cout << "Continue? (1 = Yes / 0 = No)" << std::endl;
		std::cin >> ch;
		if (ch == 0)
			break;
		std::cout << "\n\n";
		cleardevice();
	}

	/*

	// test getCircumCircle()

	while (true)
	{
		Point p1, p2, p3;
		std::cout << "Enter 3 points." << std::endl;
		std::cin >> p1.x >> p1.y >> p2.x >> p2.y >> p3.x >> p3.y;
		setcolor(CYAN);
		circle((int)p1.x, (int)p1.y, 5);
		circle((int)p2.x, (int)p2.y, 5);
		circle((int)p3.x, (int)p3.y, 5);
		Triangle t(p1, p2, p3);
		Circle c = getCircumCircle(t);
		putpixel(p1.x, p1.y, RED);
		putpixel(p2.x, p2.y, RED);
		putpixel(p3.x, p3.y, RED);
		setcolor(CYAN);
		circle((int)c.center.x, (int)c.center.y, 2);
		setcolor(YELLOW);		
		circle((int)c.center.x, (int)c.center.y, (int)c.radius);
		std::cout << "Center: (" << c.center.x << ", " << c.center.y << ")" << "\nRadius: " << c.radius << std::endl;
		std::cout << "Continue? (1 = Yes / 0 = No)" << std::endl;
		std::cin >> ch;
		if (ch == 0)
			break;
		std::cout << "\n\n";
		cleardevice();
	}

	*/

	std::cout << "Thank you." << std::endl;
	system("pause");
	closegraph();
	return 0;
}
