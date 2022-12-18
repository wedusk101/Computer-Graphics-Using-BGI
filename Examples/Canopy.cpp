
#include <iostream>
#include "primitives.h"
#include "mathutils.h"
#include "graphics.h"

#define WIDTH 1000
#define HEIGHT 1000
#define DECAY 0.65

void drawFractalCanopy(const Point& src, const Point& dst, int depth, int maxDepth, float angle, float d)
{
	if (depth == maxDepth)
		return;

	const Vec2 dir = (dst - src).getNormalized();
	Ray rootRay(dst, dir);

	Point rightRoot = getRotatedPoint(rootRay.getPointAt(DECAY * d), angle, dst);
	Point leftRoot = getRotatedPoint(rootRay.getPointAt(DECAY * d), TWO_PI - angle, dst);

	Line rightBranch(dst, rightRoot);
	Line leftBranch(dst, leftRoot);

	rightBranch.draw();
	leftBranch.draw();

	drawFractalCanopy(rightBranch.src, rightBranch.dst, depth + 1, maxDepth, angle, DECAY * d);
	drawFractalCanopy(leftBranch.src, leftBranch.dst, depth + 1, maxDepth, angle, DECAY * d);
}

int main()
{
	int maxDepth = 1;
	float angle = 0.0f;

	std::cout << "This program creates the Fractal Canopy." << std::endl;
	std::cout << "Please enter the desired depth of the generated fractal." << std::endl;
	std::cin >> maxDepth;
	std::cout << "Please enter the angle of the rotation to be used for the fractal." << std::endl;
	std::cin >> angle;
		
	initwindow(WIDTH, HEIGHT, "Canopy");

	Point src(0.5 * WIDTH, 0.9 * HEIGHT);
	Point dst(0.5 * WIDTH, 0.6 * HEIGHT);

	float d = (src - dst).getMagnitude();

	Line root(src, dst);
	root.draw();

	drawFractalCanopy(src, dst, 0, maxDepth, degreesToRadians(angle), DECAY * d);

	system("pause");
	closegraph();
}