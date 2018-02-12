#include <iostream>
#include <windows.h>
#include "graphics.h"
#include "primitives.h"

void floodFill(const int&, const int&, const int&, const int&);

int main()
{
	Circle c;
	std::cout << "Please enter the coordinates for the center (x,y)." << std::endl;
	std::cin >> c.center.x >> c.center.y;
	initwindow(640, 480, "Flood Fill");
	setcolor(3);
	circle(c.center.x, c.center.y, 23); // crashes beyond 23 pixels
	floodFill(c.center.x, c.center.y, 0, 12);
	system("pause"); // windows only feature
	closegraph();
	return 0;
}

void floodFill(const int &x, const int&y, const int &oldColor, const int &newColor)
{
	int color = getpixel(x, y);
	if (color == oldColor && color != newColor)
	{
		putpixel(x, y, newColor);
		floodFill(x, y - 1, oldColor, newColor);
		floodFill(x, y + 1, oldColor, newColor);
		floodFill(x + 1, y, oldColor, newColor);
		floodFill(x - 1, y, oldColor, newColor);
	}
}