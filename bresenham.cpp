#include <iostream>
#include <cmath>
#include "windows.h"
#include "graphics.h"

typedef struct point
{
	int x;
	int y;
} Point;

void bresenham(Point, Point);
void swap(Point &, Point &);

int main()
{
    Point start, end;
    printf("Please enter the coordinates for the starting point of the line.\n");
    scanf("%d%d", &start.x, &start.y);
    printf("Please enter the coordinates for the end point of the line.\n");
    scanf("%d%d", &end.x, &end.y);
	initwindow(600, 600, "Bresenham");
	setcolor(3);
	circle(start.x, start.y, 3);
	circle(end.x, end.y, 3);
	setcolor(12);
    bresenham(start, end);
	system("pause");
    closegraph();
    return 0;
}

void swap(Point &a, Point &b)
{
	Point tmp = a;
	a = b;
	b = tmp;
}
    
void bresenham(Point start, Point end)
{
	int dy = end.y - start.y, dx = end.x - start.x, dParam = 0;
	double x = 0.0, y = 0.0;

	if (dx == 0)
		for (y = start.y; y <= end.y; ++y)
			putpixel((int)x, (int)y, 12);
	else if (dy == 0)
		for (x = start.x; x <= end.x; ++x)
			putpixel((int)x, (int)y, 12);

	else
	{
		if (abs(dy) <= abs(dx))
		{
			if (start.x > end.x) // swaps the starting point if needed
				swap(start, end);

			putpixel(start.x, start.y, 12); // draws the initial point

			dParam = 2 * dy - dx;
			for (x = start.x, y = start.y; x <= end.x; ++x)
			{
				if (dParam < 0)
					putpixel(x, y, 12);
				else
				{
					y += 1;
					putpixel(x, y, 12);
				}

				dParam += 2 * (dy - dx);
			}
		}
	}
}