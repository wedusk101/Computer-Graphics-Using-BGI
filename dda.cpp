#include <iostream>
#include <climits>
#include "windows.h"
#include "graphics.h"


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
	double slope = 0.0, x = 0.0, y = 0.0;
	int dy = end.y - start.y, dx = end.x - start.x, sgn = 0;
	
	if (dx == 0)
		for (y = start.y; y <= end.y; ++y)
			putpixel((int)x, (int)y, 12);
	else if (dy == 0)
		for (x = start.x; x <= end.x; ++x)
			putpixel((int)x, (int)y, 12);
	else
	{
		if (abs(dy) <= abs(dx)) // gentle slope
		{
			if (start.x > end.x) // swaps the starting point if needed
				swap(start, end);

			putpixel(start.x, start.y, 12); // draws the initial point

			slope = dy /(double) dx;
			sgn = sign(slope);
			
			for (x = start.x, y = start.y; x <= end.x; x += sgn)
			{
				y = y + slope;
				putpixel((int)x, (int)y, 12);
			}
		}

		else // steep slope
		{
			if (start.y > end.y) // swaps the starting point if needed
				swap(start, end);

			putpixel(end.x, end.y, 12); // draws the initial point

			slope = dx /(double) dy;  //	1 / m
			sgn = sign(slope);
			
			for (x = start.x, y = start.y; y <= end.y; y += sgn)
			{
				x = x + slope;
				putpixel((int)x, (int)y, 12);
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