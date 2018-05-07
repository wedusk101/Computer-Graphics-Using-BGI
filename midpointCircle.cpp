#include <iostream>
#include "graphics.h"
#include "windows.h"
#include "primitives.h"

void midpointCircle(const Circle &);
void drawSymmetry(const Point &, const Circle &);

int main()
{
	Point center;
	Circle circle;
	int radius = 0;
	std::cout << "Please enter the coordinates for center of the circle." << std::endl;
	std::cin >> circle.center.x >> circle.center.y;
	std::cout << "Please enter the radius of the circle." << std::endl;
	std::cin >> circle.radius;
	initwindow(960, 600, "Circle");
	setcolor(12);
	midpointCircle(circle);
	system("pause");
	closegraph();
	return 0;
}

void midpointCircle(const Circle &circle)
{
	int eps = 1 - circle.radius;
	Point plot;
	plot.x = 0, plot.y = circle.radius;
	drawSymmetry(plot,circle);
	while (plot.x < plot.y)
	{
		if (eps < 0)
		{
			plot.x += 1;
			eps += 2 * plot.x + 3;
		}

		else
		{
			plot.x += 1;
			plot.y -= 1;
			eps += 2 * plot.x - 2 * plot.y + 5;
		}
		drawSymmetry(plot, circle);
	}
}

void drawSymmetry(const Point &point, const Circle &circle)
{
	putpixel(circle.center.x + point.x, circle.center.y + point.y, 12);
	putpixel(circle.center.x + point.y, circle.center.y + point.x, 12);
	putpixel(circle.center.x + point.x, circle.center.y - point.y, 12);
	putpixel(circle.center.x + point.y, circle.center.y - point.x, 12);
	putpixel(circle.center.x - point.x, circle.center.y - point.y, 12);
	putpixel(circle.center.x - point.x, circle.center.y + point.y, 12);
	putpixel(circle.center.x - point.y, circle.center.y + point.x, 12);
	putpixel(circle.center.x - point.y, circle.center.y - point.x, 12);
}