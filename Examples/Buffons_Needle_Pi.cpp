/*
	The following program calculates the approximate value of Pi by simulating the Buffon's Needle Problem. This is mostly a naive soltuion.
	There is much room for optimization.

	For more information please see:

	- https://en.wikipedia.org/wiki/Buffon%27s_needle_problem
*/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <windows.h>

#include "graphics.h"
#include "primitives.h"
#include "mathutils.h"

#define WIDTH 1000
#define HEIGHT 1000
#define PADDING 100

#define DRAW_STICKS

std::vector<Line> generateStrips(uint32_t stripWidth)
{
	std::vector<Line> strips;

	// draw vertical lines
	for (int x = 0; x <= WIDTH; x += stripWidth)
	{
		Line s(x, 0, x, HEIGHT);
		strips.push_back(s);
		s.draw();
	}

	return strips;
}

std::vector<Line> generateSticks(uint32_t numSticks, uint32_t stickLength)
{
	std::vector<Line> sticks;
	std::random_device r;
	std::seed_seq seed{ r() };
	std::default_random_engine engine(seed);
	std::uniform_int_distribution<int> rndX(0, WIDTH);
	std::uniform_int_distribution<int> rndY(0, HEIGHT);

	std::uniform_real_distribution<float> rndTheta(0.0f, TWO_PI);

	for (uint32_t i = 0; i < numSticks; ++i)
	{
		Point start;
		start.x = rndX(engine);
		start.y = rndY(engine);

		Point end;
		end.x = start.x + stickLength;
		end.y = start.y;

		float angle = rndTheta(engine);

		Line newStick(start, end);
		newStick.rotate(angle);

		sticks.push_back(newStick);
	}

	return sticks;
}

float calcPi(const std::vector<Line>& strips, uint32_t stripWidth, uint32_t numSticks, uint32_t stickLength, bool renderSticks)
{
	std::vector<Line> sticks = generateSticks(numSticks, stickLength);
	uint32_t crossedSticks = 0;

	if (renderSticks)
	{
		for (uint32_t i = 0; i < sticks.size(); ++i)
		{
			setcolor(YELLOW);
			sticks[i].draw();
		}
	}

	for (uint32_t i = 0; i < sticks.size(); ++i)
	{	
		/*
		
		// binary search to reduce the number of strips we test against
		auto lower = std::lower_bound(strips.begin(), strips.end(), sticks[i].src.x, [&](const Line& strip, int stickX)
		{
			return (strip.src.x <= stickX);
		});

		auto idx = std::distance(strips.begin(), lower) - 1;

		// skip the stick if it is out of bounds of our region of strips
		if (idx == 0 || idx == strips.size() - 1)
			continue;

		if (sticks[i].intersects(strips[idx]) || sticks[i].intersects(strips[idx + 1]))
			++crossedSticks;	

		*/

		for (const auto& strip : strips)
		{
			if (strip.intersects(sticks[i]))
				++crossedSticks;
		}
	}

	std::cout << "Number of intersected sticks: " << crossedSticks << std::endl;
	return numSticks / (float)crossedSticks;
}

int main()
{
	initwindow(WIDTH, HEIGHT, "Buffon's Needle");
	
	uint32_t stripWidth = 0;
	uint32_t numSticks = 0;
	uint32_t stickLength = 0;

	std::cout << "This program calculates Pi by simulating the Buffon's Needle problem." << std::endl;
	std::cout << "Please enter the width of the parallel strips." << std::endl;
	std::cin >> stripWidth;

	stripWidth = min(WIDTH, stripWidth);
	std::vector<Line> strips = generateStrips(stripWidth);

	std::cout << "Please enter the number of sticks to use for the simulation. Higher the number, more accurate the simulation." << std::endl;
	std::cin >> numSticks;

	stickLength = stripWidth / 2;

	std::cout << "Render the sticks? This will affect the performance. (1 / 0)" << std::endl;
	int val = 0;
	std::cin >> val;

	bool renderSticks = (val != 0);

	auto start = std::chrono::high_resolution_clock::now();
	float approx_pi = calcPi(strips, stripWidth, numSticks, stickLength, renderSticks);
	std::cout << "The approximate value of Pi is: " << approx_pi << std::endl;
	auto stop = std::chrono::high_resolution_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time taken is " << diff.count() << " milliseconds." << std::endl;

	system("pause"); // windows only feature
	closegraph();
	return 0;
}