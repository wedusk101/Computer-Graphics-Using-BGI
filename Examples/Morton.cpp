/*
	The following program renders the Morton space-filling curve.

	For more information please see:

	- https://en.wikipedia.org/wiki/Z-order_curve
*/

#include <algorithm>
#include <iostream>
#include <vector>

#include "primitives.h"

#define WIDTH 1000
#define HEIGHT 1000
#define originX 100
#define originY 100

struct MortonCodeData
{
	Point point;
	uint32_t mortonCode;
};

uint16_t leftInterleaveBy2(uint16_t x)
{
	x = (x | (x << 8)) & 0x00FF00FF;
	x = (x | (x << 4)) & 0x0F0F0F0F;
	x = (x | (x << 2)) & 0x33333333;
	x = (x | (x << 1)) & 0x55555555;

	return x;
}

uint32_t encodeMorton2(uint16_t x, uint16_t y)
{
	return (leftInterleaveBy2(y) << 1 |
			leftInterleaveBy2(x));
}

std::vector<MortonCodeData> getMortonCodes(uint32_t max)
{
	std::vector<MortonCodeData> codeList;

	for (uint32_t y = originY; y <= max; y += 100)
	{
		for (uint32_t x = originX; x <= max; x += 100)
		{
			MortonCodeData mc;
			mc.point = Point(x + 50, y + 50);
			codeList.push_back(mc);
		}
	}

	for (auto& mc : codeList)
		mc.mortonCode = encodeMorton2((uint16_t)mc.point.x, (uint16_t)mc.point.y);

	return codeList;
}

std::vector<Line> getMortonCurveSegments(std::vector<MortonCodeData>& codeList)
{
	std::sort(codeList.begin(), codeList.end(),
		[&](MortonCodeData& a, MortonCodeData& b)
		{
			return a.mortonCode < b.mortonCode;
		});

	for (const auto& code : codeList)
		std::cout << "Code: " << code.mortonCode << " (x,y): " << code.point.x << " " << code.point.y << std::endl;

	std::vector<Line> curveSegments;

	for (int i = 0; i < codeList.size() - 1; ++i)
	{
		Line segment(codeList[i].point, codeList[i + 1].point);
		curveSegments.push_back(segment);
	}

	return curveSegments;
}

void drawGrid(uint32_t size)
{
	for (int x = originX; x <= originX + size; x += 100)
	{
		line(x, originY, x, originY + size);
	}

	for (int y = originY; y <= originY + size; y += 100)
	{
		line(originX, y, originX + size, y);
	}
}

void drawMortonCurve(const std::vector<Line>& segmentList)
{
	setcolor(YELLOW);
	for (const auto& segment : segmentList)
		segment.draw();
}

int main()
{
	initwindow(WIDTH, HEIGHT, "Morton Curve");
	const uint32_t gridSize = 800;
	std::cout << "This program displays the Morton space-filling curve, also known as the Z-order curve." << std::endl;

	drawGrid(gridSize);
	std::vector<Line> curveList = getMortonCurveSegments(getMortonCodes(gridSize));
	drawMortonCurve(curveList);

	system("pause");
	closegraph();
	return 0;
}