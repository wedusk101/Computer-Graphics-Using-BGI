/*
	The following program renders the Morton space-filling curve.

	For more information please see:

	- https://en.wikipedia.org/wiki/Z-order_curve
*/

#include <iostream>
#include <vector>

#include "primitives.h"

struct MortonCodeData
{
	Point point;
	uint32_t mortonCode;
};

uint32_t leftInterleaveBy2(uint32_t x)
{
	return 0;
}

uint32_t encodeMorton2(uint32_t x, uint32_t y)
{
	return 0;
}

std::vector<MortonCodeData> getMortonCodes(uint32_t max)
{
	std::vector<MortonCodeData> codeList;

	return codeList;
}

std::vector<Line> getMortonCurveSegments(const std::vector<MortonCodeData>& codeList)
{
	std::vector<Line> curveSegments;

	return curveSegments;
}

void drawGrid(uint32_t size)
{

}

void drawMortonCurve(const std::vector<Line>& segmentList)
{
	setcolor(YELLOW);
	for (const auto& segment : segmentList)
		segment.draw();
}

int main()
{
	uint32_t gridSize = 0;
	std::cout << "this program displays the Morton space-filling curve, also known as the Z-order curve." << std::endl;
	std::cout << "Please enter a positive integer value for the grid size." << std::endl;
	std::cin >> gridSize;

	drawGrid(gridSize);
	std::vector<Line> curveList = getMortonCurveSegments(getMortonCodes(gridSize));
	drawMortonCurve(curveList);

	return 0;
}