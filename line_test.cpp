#include <iostream>
#include <windows.h>
#include "graphics.h"
#include "primitives.h"

int main()
{
	Line l;
	std::cout << "Please enter the end points for the line." << std::endl;
	std::cout << "Please enter the coordinates for the source point (x,y)." << std::endl;
	std::cin >> l.src.x >> l.src.y;
	std::cout << "Please enter the coordinates for the destination point (x,y)." << std::endl;
	std::cin >> l.dst.x >> l.dst.y;
	initwindow(640, 480, "Line");
	line(l.src.x, l.src.y, l.dst.x, l.dst.y);	
	system("pause"); // windows only feature
	closegraph();
	return 0;
}