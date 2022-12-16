
#include <iostream>
#include "primitives.h"
#include "graphics.h"

#define WIDTH 600
#define HEIGHT 600

void drawFractalCanopy(int depth, int maxDepth, float angle)
{
	if (depth < maxDepth)
	{

	}
}

int main()
{
	initwindow(WIDTH, HEIGHT, "Voronoi");
	int maxDepth = 1;
	float angle = 0.0f;

	std::cout << "This program creates the Fractal Canopy." << std::endl;
	std::cout << "Please enter the desired depth of the generated fractal." << std::endl;
	std::cin >> maxDepth;
	std::cout << "Please enter the angle of the rotation to be used for the fractal." << std::endl;
	std::cin >> angle;

	closegraph();
}