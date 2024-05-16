/*
	The following program renders the Morton space-filling curve.

	For more information please see:

	- https://en.wikipedia.org/wiki/Z-order_curve
*/

#include <algorithm>
#include <iostream>
#include <vector>

#include "primitives.h"

// changing these hard-coded values will break the output
#define WIDTH 1000
#define HEIGHT 1000
#define originX 100
#define originY 100
#define CELL_SIZE 25

struct MortonCodeData
{
	Point pixelCoordinate;
	Point zOrderCoordinate;
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

	for (uint32_t y = originY, j = 0; y < originY + max; y += CELL_SIZE, ++j)
	{
		for (uint32_t x = originX, i = 0; x < originX + max; x += CELL_SIZE, ++i)
		{
			MortonCodeData mc;
			mc.pixelCoordinate = Point(x + (CELL_SIZE / 2), y + (CELL_SIZE / 2));
			mc.zOrderCoordinate = Point(i, j);
			mc.mortonCode = encodeMorton2((uint16_t)mc.zOrderCoordinate.x, (uint16_t)mc.zOrderCoordinate.y);
			codeList.push_back(mc);
		}
	}

	return codeList;
}

std::vector<Line> getMortonCurveSegments(std::vector<MortonCodeData>& codeList)
{
	std::sort(codeList.begin(), codeList.end(),
		[](const MortonCodeData& a, const MortonCodeData& b)
		{
			return a.mortonCode < b.mortonCode;
		});

#ifndef NDEBUG
	for (const auto& code : codeList)
	{
		std::cout << "Morton Code: " << code.mortonCode <<
			" Pixel Coordinates: (" << code.pixelCoordinate.x << ", " << code.pixelCoordinate.y << ")" << std::endl;
	}
#endif // DEBUG mode

	std::vector<Line> curveSegments;

	for (int i = 0; i < codeList.size() - 1; ++i)
	{
		Line segment(codeList[i].pixelCoordinate, codeList[i + 1].pixelCoordinate);
		curveSegments.push_back(segment);
	}

	return curveSegments;
}

void drawGrid(uint32_t size)
{
	// draw vertical lines
	for (int x = originX; x <= originX + size; x += CELL_SIZE)
	{
		line(x, originY, x, originY + size);
	}

	// draw horizontal lines
	for (int y = originY; y <= originY + size; y += CELL_SIZE)
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
	std::vector<MortonCodeData> codeList = getMortonCodes(gridSize);
	std::vector<Line> curveList = getMortonCurveSegments(codeList);
	drawMortonCurve(curveList);

	system("pause");
	closegraph();
	return 0;
}