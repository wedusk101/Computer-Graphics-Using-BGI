#include <iostream>
#include <climits>
#include "windows.h"
#include "graphics.h"

// #define X(n) (getmaxx()/2 + n)
// #define Y(n) (getmaxy()/2 - n)

typedef struct point
{
        int x;
        int y;
} Point;

void dda(Point , Point);
void swap(Point &, Point &);
int sign(const double &);

int main()
{
    Point start, end;
    printf("Please enter the coordinates for the starting point of the line.\n");
    scanf("%d%d", &start.x, &start.y);
    printf("Please enter the coordinates for the end point of the line.\n");
	scanf("%d%d", &end.x, &end.y);
	initwindow(600, 600, "DDA");
	// line(getmaxx() / 2, 0, getmaxx() / 2, getmaxy());
	// line(0, getmaxy()/2, getmaxx(), getmaxy()/2);
	setcolor(3);
	circle(start.x, start.y, 3);
	circle(end.x, end.y, 3);
	setcolor(12);
    dda(start, end);
	system("pause");
    closegraph();
    return 0;
}
    
void dda(Point start, Point end)
{
	int dy = end.y - start.y, dx = end.x - start.x, sgn = 0;
	std::cout << dy << " " << dx << std::endl;
	double slope = 0.0;
	if (start.x > end.x)
		swap(start, end);
	if (dx == 0)
		for (; start.y != end.y; ++start.y)
			putpixel(start.x, start.y, 12);
	else if (dy == 0)
		for (; start.x != end.x; ++start.x)
			putpixel(start.x, start.y, 12);
	else
	{
		if (abs(dy) <= abs(dx)) // gentle slope
		{
			slope = dy /(double) dx;
			sgn = sign(slope);
			std::cout << "CASE 1 Slope : " << slope << " Sign : " << sgn << std::endl;
			for (; start.x != end.x; start.x += sgn)
			{
				start.y = static_cast<int>(start.y + slope);
				putpixel(start.x, start.y, 12);
			}
		}

		else // steep slope
		{
			slope = dx /(double) dy;
			sgn = sign(slope);
			std::cout << "CASE 2 Slope : " << slope << " Sign : " << sgn << std::endl;
			for (; start.y != end.y; start.y += sgn)
			{
				start.x = static_cast<int>(start.x + slope);
				putpixel(start.x, start.y, 12);
			}
		}

	}
} 

void swap(Point &a, Point &b)
{
	Point tmp = a;
	a = b;
	b = tmp;
}

int sign(const double &val)
{
	if (val == 0)
		return 0;
	else if (val < 0)
		return -1;
	else
		return 1;
}