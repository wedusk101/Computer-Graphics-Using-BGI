/* The following program renders the Koch Snowflake .*/

#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>

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
	double t = 0.86605 * a; // height (h) = (sqrt(3) / 2) * a

	Vec2 normal = dir.getNormal();
	Vec2 p = o + normal * t; // walk h units from the mid point in the direction of the apex
	return vec2Point(p);
}

// return a Fractal side for a given edge
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
	// setcolor(BLACK);
	// line(edgeList[0].dst.x, edgeList[0].dst.y, edgeList[3].src.x, edgeList[3].src.y); // remove split edge from triangle
	// setcolor(WHITE);
	for (const auto& edge : edgeList)
		edge.draw();
}

void drawKochSnowflake(std::vector<Line>& fractalEdges, int depth, int maxDepth)
{
	if (depth < maxDepth)
	{
		cleardevice();

		// draw the existing fractal edges
		for (const auto& edge : fractalEdges)
			edge.draw();

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
		drawFractal(fractalEdges);
		drawKochSnowflake(fractalEdges, depth + 1, maxDepth);
	}
}

int main()
{
	initwindow(640, 480, "Koch Snowflake");
	int ch = 0;
	uint16_t nItr = 0;

	std::cout << "This program renders the Koch Snowflake with animation." << std::endl;
	std::cout << "Please input the desired number of interations." << std::endl;
	std::cin >> nItr;

	std::vector<Line> edges;
	edges.push_back({{ 220, 120 }, { 420, 120 }});
	// edges.push_back({{ 420, 120 }, { 320, 294 }});
	// edges.push_back({{ 320, 294 }, { 220, 120 }});

	drawKochSnowflake(edges, 0, nItr);			
		
	std::cout << "Thank you." << std::endl;
	system("pause"); // windows only feature
	closegraph();
	return 0;
}