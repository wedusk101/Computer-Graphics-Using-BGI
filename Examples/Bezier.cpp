/*The following program render Bezier curves using the Berstein polynomials.*/
#include <iostream>
#include <windows.h>
#include <cmath>
#include "graphics.h"
#include "colors.h"

typedef struct 
{
	double x;
	double y;
} Point;

void drawBezier(Point points[])
{
	setcolor(LIGHTRED);
	circle(points[0].x, points[0].y, 5);
	circle(points[1].x, points[1].y, 5);
	circle(points[2].x, points[2].y, 5);
	circle(points[3].x, points[3].y, 5);

	setcolor(CYAN);
	line(points[0].x, points[0].y, points[1].x, points[1].y);
	line(points[1].x, points[1].y, points[2].x, points[2].y);
	line(points[2].x, points[2].y, points[3].x, points[3].y);

	double t = 0, steps = 1000, inc = 1. / steps;
	Point lerpPoint;
	Point startPoint = points[0];
	setcolor(YELLOW);

	// calculate the interpolated point using the Berstein  polynomials 
	// B(t) = P_0 * (1 - t)^3 + P_1 * 3 * (1 - t)^2 * t + P_2 * 3 * (1 - t) * t^2 + P_3 * t^3
	// where P(0, 1, 2, 3) are the four control points in a cubic Bezier curve,
	// t is the parameter and B(t) gives us the interpolated point at that paramater.
	// B(t) has to be evaluated for (x, y, z) individually for each dimension - here it is calculated for (x, y) as this is a 2D render
	for (t = 0; t < 1.0; t += inc)
	{		
		lerpPoint.x = points[0].x * pow((1 - t), 3) + points[1].x * 3 * pow((1 - t), 2) * t + points[2].x * 3 * (1 - t) * t * t + points[3].x * pow(t, 3);
		lerpPoint.y = points[0].y * pow((1 - t), 3) + points[1].y * 3 * pow((1 - t), 2) * t + points[2].y * 3 * (1 - t) * t * t + points[3].y * pow(t, 3);
		line(startPoint.x, startPoint.y, lerpPoint.x, lerpPoint.y);
		startPoint = lerpPoint;
	}
}

int main()
{
	initwindow(640, 480, "Bezier");
	Point curve[4];
	std::cout << "Please input the control points (x,y)." << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << "Please enter the coordinates for point " << i + 1 << "." << std::endl;
		std::cin >> curve[i].x >> curve[i].y;
	}
	drawBezier(curve);
	system("pause"); // windows only feature
	closegraph();
	return 0;
}