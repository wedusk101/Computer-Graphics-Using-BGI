/*The following program renders Bezier curves in two ways --- using Berstein polynomials and using de Casteljau's algorithm .*/
#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>
#include "graphics.h"
#include "colors.h"

typedef struct
{
	double x;
	double y;
} Point;

double lerp(double start, double end, double t)
{
	return start * (1 - t) + end * t;
}

Point getLerpedPointRecursive(const std::vector<Point> &vec, double t)
{
	if (vec.size() == 1)
		return vec[0];
	else
	{
		std::vector<Point> lerpedPoints;
		for (int i = 0; i < vec.size() - 1; i++)
		{
			Point newPoint;
			newPoint.x = lerp(vec[i].x, vec[i + 1].x, t);
			newPoint.y = lerp(vec[i].y, vec[i + 1].y, t);
			lerpedPoints.push_back(newPoint);
		}
		return getLerpedPointRecursive(lerpedPoints, t);
	}
}

void drawBezierDeCasteljau(Point points[], uint16_t steps)
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

	double t = 0, inc = 1. / steps;
	Point lerpPoint;
	Point startPoint = points[0];
	setcolor(YELLOW);


	std::vector<Point> pointsList(points, points + 4);

	// calculate the interpolated point using de Casteljau's algorithm 
	for (t = 0; t < 1.0; t += inc)
	{
		lerpPoint = getLerpedPointRecursive(pointsList, t);
		line(startPoint.x, startPoint.y, lerpPoint.x, lerpPoint.y);
		delay(10);
		startPoint = lerpPoint;
	}
}

void drawBezierBernstein(Point points[], uint16_t steps)
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

	double t = 0, inc = 1. / steps;
	Point lerpPoint;
	Point startPoint = points[0];
	setcolor(YELLOW);

	// calculate the interpolated point using the Bernstein  polynomials 
	// B(t) = P_0 * (1 - t)^3 + P_1 * 3 * (1 - t)^2 * t + P_2 * 3 * (1 - t) * t^2 + P_3 * t^3
	// where P(0, 1, 2, 3) are the four control points in a cubic Bezier curve,
	// t is the parameter and B(t) gives us the interpolated point at that paramater.
	// B(t) has to be evaluated for (x, y, z) individually for each dimension - here it is calculated for (x, y) as this is a 2D render
	for (t = 0; t < 1.0; t += inc)
	{		
		lerpPoint.x = points[0].x * pow((1 - t), 3) + points[1].x * 3 * pow((1 - t), 2) * t + points[2].x * 3 * (1 - t) * t * t + points[3].x * pow(t, 3);
		lerpPoint.y = points[0].y * pow((1 - t), 3) + points[1].y * 3 * pow((1 - t), 2) * t + points[2].y * 3 * (1 - t) * t * t + points[3].y * pow(t, 3);
		line(startPoint.x, startPoint.y, lerpPoint.x, lerpPoint.y);
		delay(10);
		startPoint = lerpPoint;
	}
}

int main()
{
	initwindow(640, 480, "Bezier");
	int ch = 0;
	uint16_t stepSize = 0;
	Point curve[4];
	while (true)
	{
		std::cout << "This program renders cubic Bezier curves (with 4 control points) with animation." << std::endl;
		std::cout << "Please input the control points (x,y)." << std::endl;
		for (int i = 0; i < 4; i++)
		{
			std::cout << "Please enter the coordinates for point " << i + 1 << "." << std::endl;
			std::cin >> curve[i].x >> curve[i].y;
		}
		std::cout << "Please enter the step size. Higher step sizes result in higher accuracy but render slower." << std::endl;
		std::cin >> stepSize;
		std::cout << "Enter 1 to have the Bezier curve rendered using Bernstein polynomials." << std::endl;
		std::cout << "Enter 2 to have the Bezier curve rendered using de Casteljau's algorithm." << std::endl;
		std::cin >> ch;
		if (ch == 1)
			drawBezierBernstein(curve, stepSize);
		else if (ch == 2)
			drawBezierDeCasteljau(curve, stepSize);
		else
			std::cout << "Invalid input." << std::endl;
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