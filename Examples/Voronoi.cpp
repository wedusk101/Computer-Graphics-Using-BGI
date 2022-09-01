/*
	The following program renders the Voronoi diagram. It uses the Delaunay triangulation as an intermediate step.
	The Delaunay triangulation is implemented using the Bowyer-Watson algorithm.

	For more information please see:
	
	- https://en.wikipedia.org/wiki/Voronoi_diagram
	- https://en.wikipedia.org/wiki/Delaunay_triangulation
	- https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm

*/

#define NOMINMAX

#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include "graphics.h"
#include "colors.h"

#define WIDTH 640
#define HEIGHT 480
#define PADDING 20

constexpr float EPSILON = 1e-4;
static size_t pointCount = 0;

typedef struct Vec2
{
	float x;
	float y;

	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float x_, float y_) : x(x_), y(y_) {}

	inline Vec2 operator*(float c)
	{
		return Vec2(x * c, y * c);
	}

	inline Vec2 operator/(float c)
	{
		return Vec2(x / c, y / c);
	}

	inline float getMagnitude() const
	{
		return sqrt(x * x + y * y);
	}

	Vec2 getNormalized() const
	{
		float mag = getMagnitude();
		if (fabs(mag) < EPSILON)
			return Vec2();
		return Vec2(x / mag, y / mag);
	}
} Vec2;

typedef struct Point
{
	float x;
	float y;

	size_t pointID;

	Point() : x(0.0f), y(0.0f), pointID(0) {}
	Point(float x_, float y_) : x(x_), y(y_)
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

	void draw(uint8_t color) const
	{
		// putpixel((int)x, (int)y, YELLOW);
		setcolor(color);
		circle((int)x, (int)y, 5);
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

	// returns whether two triangles are neighbors of each other
	bool isNeighbor(const Triangle &candidate) const
	{
		// ignore self
		if (*this == candidate)
			return false;

		int count = 0;

		if (a == candidate.a || a == candidate.b || a == candidate.c)
			count++;
		if (b == candidate.a || b == candidate.b || b == candidate.c)
			count++;
		if (c == candidate.a || c == candidate.b || c == candidate.c)
			count++;

		return (count == 2);
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
	float radius;

	Circle() : center(Point()), radius(0.0f) {}
	Circle(const Point &c, float r) : center(c), radius(r) {}

	void draw(uint8_t color) const
	{
		setcolor(color);
		circle((int)center.x, (int)center.y, radius);
	}
} Circle;

inline float getEuclideanDist(const Point &p1, const Point &p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// used for Possion disk sampling during the generation of sites
bool isValidPos(const Point &p, const float minRadius, const std::vector<Point> &listPoints)
{
	for (const auto& point : listPoints)
		if (getEuclideanDist(p, point) < minRadius)
			return false;
	return true;
}

inline bool isInsideCircle(const Point &p, const Circle &c)
{
	return getEuclideanDist(p, c.center) < c.radius;
}


// calculates the intersection points between two circles of same radii
bool intersects(const Point &c1, const Point &c2, float radius, Point &p1, Point &p2)
{
	float d = getEuclideanDist(c1, c2);
	if (d >= 2 * radius || fabs(d) < EPSILON)
		return false;
	else
	{		
		float oc1 = d / 2;
		float op1 = sqrt(radius * radius - oc1 * oc1);
		Point o = c1 + ((c2 - c1) * (oc1 / d));
		Vec2 oc = c1 - o;
		Vec2 n1 = Vec2(-oc.y, oc.x).getNormalized();
		Vec2 n2 = Vec2(oc.y, -oc.x).getNormalized();
		p1 = o + n1 * op1;
		p2 = o + n2 * op1;
		return true;
	}
}

Circle getCircumCircle(const Triangle &triangle)
{
	// triangle.display();

	float x = 0, y = 0, radius = 0, m1 = 0, m2 = 0, c1 = 0, c2 = 0;
	float num1 = triangle.a.x - triangle.c.x;
	float den1 = triangle.c.y - triangle.a.y;
	float num2 = triangle.b.x - triangle.c.x;
	float den2 = triangle.c.y - triangle.b.y;	

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
	Triangle superTriangle(Point(0, 0),
						   Point(0, 2 * HEIGHT),
						   Point(2 * WIDTH, 0));	

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

		// remove float edges by deleting duplicate triangles
		for (const auto &badTriangle : invalidMeshList)
		{
			meshList.erase(std::remove_if(meshList.begin(),
									meshList.end(),
									[&, badTriangle](auto triangle) 
			{
				return triangle == badTriangle;
			}), meshList.end());
		}

		// fill the polygonal hole by retriangulating it
		for (const auto &edge : boundaryList)
		{
			Triangle t(edge.src, edge.dst, *siteItr);
			meshList.push_back(t);
		}
	}

	meshList.erase(std::remove_if(meshList.begin(),
								  meshList.end(),
								  [&, superTriangle](auto triangle) 
	{
		return (triangle.containsVertex(superTriangle.a) ||
				triangle.containsVertex(superTriangle.b) ||
				triangle.containsVertex(superTriangle.c));
	}), meshList.end());

	return meshList;
}

// this function may be too slow or get stuck in an infinite loop
// if the number of sites to generate is too high with respect 
// to the viewport resolution and minimum radius value
std::vector<Point> generateSites(size_t maxPoints, float radius)
{
	std::vector<Point> siteList;
	std::random_device r;
	std::seed_seq seed{r()};
	std::default_random_engine engine(seed);
	std::uniform_real_distribution<float> rndX(PADDING, WIDTH - PADDING);
	std::uniform_real_distribution<float> rndY(PADDING, HEIGHT - PADDING);

	float x = rndX(engine);
	float y = rndY(engine);
	Point site(x, y);
	siteList.push_back(site);  // add the first point
	site.draw(CYAN);
	while (siteList.size() < maxPoints)
	{
		site.x = rndX(engine);
		site.y = rndY(engine);

		// Poisson disk sampling
		if (isValidPos(site, radius, siteList)) // inefficient rejection sampling
		{
			siteList.push_back(site);			
			site.draw(CYAN);
		}
	}

	return siteList;
}

void drawMesh(const std::vector<Triangle> &meshList)
{
	for (const auto& triangle : meshList)
		triangle.draw(LIGHTRED);
}

/**
	The Vorornoi pattern is the dual graph of the Delaunay triangulation.
	This function generates the resulting Voronoi pettern from the input
	Delaunay triangulated mesh.
*/
void drawVoronoiPattern(const std::vector<Triangle>& mesh)
{
	for (size_t i = 0; i < mesh.size(); ++i)
	{
		Circle circumCircle = getCircumCircle(mesh[i]);
		for (size_t j = 0; j < mesh.size(); ++j)
		{
			if (mesh[i].isNeighbor(mesh[j]))
			{
				Circle circumCircleNeighbor = getCircumCircle(mesh[j]);
				// circumCircle.draw(GREEN); // for debugging
				// circumCircleNeighbor.draw(LIGHTBLUE); // for debugging
				setcolor(YELLOW);
				line(circumCircle.center.x, circumCircle.center.y, circumCircleNeighbor.center.x, circumCircleNeighbor.center.y);
			}
		}
	}
}

int main()
{
	initwindow(WIDTH, HEIGHT, "Voronoi");
	float minSiteDist = 0;
	size_t maxPoints = 0, ch = 0;

	while (true)
	{
		std::cout << "This program creates a Voronoi parition pattern." << std::endl;
		std::cout << "How many sites(points) would you like to have on your partition?" << std::endl;
		std::cin >> maxPoints;

		std::cout << "Please enter the minimum site distance between points(radius)." << std::endl;
		std::cin >> minSiteDist;

		std::cout << "Drawing the randomly generated points using Poisson disk sampling." << std::endl;
		std::vector<Point> sites = generateSites(maxPoints, minSiteDist);	
		std::vector<Triangle> mesh = triangulate(sites);
		std::cout << mesh.size() << " triangles generated after triangulation." << std::endl;

		std::cout << "Do you want to draw the generated mesh? (1 = Yes / 0 = No)" << std::endl;
		std::cin >> ch;
		if (ch)
		{
			std::cout << "Drawing generated mesh." << std::endl;
			drawMesh(mesh);
		}

		std::cout << "Do you want to draw the generated Voronoi partition? (1 = Yes / 0 = No)" << std::endl;
		std::cin >> ch;
		if (ch)
		{
			std::cout << "Drawing the generated Voronoi partition." << std::endl;
			drawVoronoiPattern(mesh);
		}

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
