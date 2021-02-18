#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <omp.h>
#include "windows.h"
#include "graphics.h"
#include "colors.h"
#include "primitives.h"

void genMidpoint(const Point &a, const Point &b, Point &mp)
{
	mp.x = (a.x + b.x) / 2;
	mp.y = (a.y + b.y) / 2;
}

void drawTriangle(const Triangle &triangle)
{
	line(triangle.a.x, triangle.a.y, triangle.b.x, triangle.b.y); // AB
	line(triangle.b.x, triangle.b.y, triangle.c.x, triangle.c.y); // BC
	line(triangle.c.x, triangle.c.y, triangle.a.x, triangle.a.y); // CA
}

void genSierpinski(const Triangle &triangle, int depth, int maxDepth)
{
	if (depth < maxDepth)
	{
		Triangle inner;
		genMidpoint(triangle.a, triangle.b, inner.a);
		genMidpoint(triangle.b, triangle.c, inner.b);
		genMidpoint(triangle.c, triangle.a, inner.c);
		genSierpinski(inner, depth + 1, maxDepth);
	}
}

int main()
{
	int maxDepth = 0;
	Triangle root;
	std::cout << "Please enter the coordinates for the vertices of the root triangle." << std::endl;
	std::cout << "Please enter the coordinates(x, y) for vertex A." << std::endl;
	std::cin >> root.a.x;
	std::cin >> root.a.y;
	std::cout << "Please enter the coordinates(x, y) for vertex B." << std::endl;
	std::cin >> root.b.x;
	std::cin >> root.b.y;
	std::cout << "Please enter the coordinates(x, y) for vertex C." << std::endl;
	std::cin >> root.c.x;
	std::cin >> root.c.y;
	std::cout << "Please enter the max recursion depth." << std::endl;
	std::cin >> maxDepth;
	std::cout << "Generating the Sierpinski triangle..." << std::endl;
	initwindow(640, 480, "Line");
	drawTriangle(root);
	genSierpinski(root, 0, maxDepth);
	system("pause"); // windows only feature
	closegraph();
}

