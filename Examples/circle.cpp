#include <iostream>
#include <windows.h>
#include "graphics.h"
#include "primitives.h"

int main()
{
	int i = 0;
	Point p;
	std::cout << "Please enter the coordinates for the center (x,y)." << std::endl;
	std::cin >> p.x >> p.y;
	initwindow(400, 300, "Circle");
	setcolor(12);
	for (i = 0; i < 40; i++)
	{
		circle(p.x, p.y, i);
		delay(100);
	}
	system("pause"); // windows only feature
	closegraph();
	return 0;
}