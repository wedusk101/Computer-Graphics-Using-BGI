/* 
	The following program renders the Koch Snowflake using a naive implementation.

	For more information please see https://en.wikipedia.org/wiki/Koch_snowflake
	
*/

#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>
#include <chrono>

#include "graphics.h"
#include "colors.h"
#include "primitives.h"

// trisect a line segment and return the points
std::vector<Point> trisect(const Line& edge)
{
	Point p1;
	double x = ((double)edge.dst.x + 2 * (double)edge.src.x) * 0.3333; // (mx2 + nx1) / (m + n)
	double y = ((double)edge.dst.y + 2 * (double)edge.src.y) * 0.3333; // (my2 + ny1) / (m + n)

	p1.x = static_cast<int>(x);
	p1.y = static_cast<int>(y);

	Point p2 = getMidpoint(p1, edge.dst);
	std::vector<Point> outList = {edge.src, p1, p2, edge.dst};
	return outList;
}

/*
	Given any two points p1 and p2 in an equilateral triangle,
	get the third point.
*/
Point getApex(const Point& p1, const Point& p2)
{	
	Vec2 dir = points2Vec(p1, p2);	// get direction vector from p1 to p2

	Point mid = getMidpoint(p1, p2);
	Vec2 o(mid.x, mid.y);
	double a = dir.getMagnitude(); // get the length of each side of the equilateral triangle
	double h = 0.86605 * a; // height (h) = (sqrt(3) / 2) * a

	Vec2 normal = dir.getNormal();
	Vec2 p = o + normal * h; // walk h units from the mid point in the direction of the apex
	return vec2Point(p);
}

// return a fractal side for a given edge
std::vector<Line> getFractalSide(const Line& edge)
{
	std::vector<Point> splitPoints = trisect(edge);
	Point apex = getApex(splitPoints[1], splitPoints[2]);

	std::vector<Line> out;
	out.push_back(Line(splitPoints[0], splitPoints[1]));
	out.push_back(Line(splitPoints[1], apex));
	out.push_back(Line(apex, splitPoints[2]));
	out.push_back(Line(splitPoints[2], splitPoints[3]));

	return out;
}

// draws each Koch curve of the snowflake
void drawFractal(const std::vector<Line>& edgeList)
{
	for (const auto& edge : edgeList)
		edge.draw();
}

void drawKochSnowflake(std::vector<Line>& fractalEdges, int depth, int maxDepth)
{
	if (depth < maxDepth)
	{
		cleardevice();
		std::cout << "Drawing iteration: " << depth + 1 << std::endl;		

		// draw the existing fractal edges
		for (const auto& edge : fractalEdges)
			edge.draw();

		delay(500);
		std::vector<Line> newEdges;

		// fractalize each existing edge 
		for (const auto& edge : fractalEdges)
		{
			std::vector<Line> generatedEdges = getFractalSide(edge);
			
			// save each generated edge in a buffer storage
			for (const auto& edge : generatedEdges)
				newEdges.push_back(edge);
		}
		fractalEdges.clear(); // clear stale edges
		fractalEdges = newEdges; // update edges with the newly fractalized edges

		cleardevice();
		drawFractal(fractalEdges); // draw the newly created fractalized edges
		drawKochSnowflake(fractalEdges, depth + 1, maxDepth);
	}
}

int main()
{
	initwindow(1000, 1000, "Koch Snowflake");
	int ch = 0;
	uint16_t nItr = 0;

	while (true)
	{
		std::cout << "This program renders the Koch Snowflake with animation." << std::endl;
		std::cout << "Please input the desired number of interations." << std::endl;
		std::cin >> nItr;

		// hard-coded values for initial vertices
		// of the parent equilateral triangle
		std::vector<Line> edges;
		edges.push_back({{ 200, 275 }, { 800, 275 }});
		edges.push_back({{ 800, 275 }, { 500, 795 }});
		edges.push_back({{ 500, 795 }, { 200, 275 }});

		auto start = std::chrono::high_resolution_clock::now();

		drawKochSnowflake(edges, 0, nItr);

		auto stop = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
		std::cout << "Time taken is " << diff.count() << " seconds.\n" << std::endl;

		std::cout << "Continue? (1 = Yes / 0 = No)" << std::endl;
		std::cin >> ch;
		if (ch == 0)
			break;
		std::cout << "\n\n";
		cleardevice();
	}
		
	std::cout << "Thank you." << std::endl;
	system("pause"); // windows only feature
	closegraph();
	return 0;
}