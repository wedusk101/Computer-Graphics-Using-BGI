#include <iostream>
#include <climits>
#include "windows.h"
#include "graphics.h"

#define X(n) (getmaxx()/2 + n)
#define Y(n) (getmaxy()/2 - n)

typedef struct point
{
        int x;
        int y;
} Point;

void dda(Point , Point);
void swap(Point &, Point &);

int main()
{
    Point start, end;
    printf("Please enter the coordinates for the starting point of the line.\n");
    scanf("%d%d", &start.x, &start.y);
    printf("Please enter the coordinates for the end point of the line.\n");
	scanf("%d%d", &end.x, &end.y);
	initwindow(1024, 768, "DDA");
	line(getmaxx() / 2, 0, getmaxx() / 2, getmaxy());
	line(0, getmaxy()/2, getmaxx(), getmaxy()/2);
	setcolor(3);
	circle(X(start.x), Y(start.y), 3);
	circle(X(end.x), Y(end.y), 3);
	setcolor(12);
    dda(start, end);
	system("pause");
    closegraph();
    return 0;
}
    
void dda(Point start, Point end)
{
	int dy = end.y - start.y, dx = end.x - start.x;
	double slope = 0.0;
	if (start.x > end.x)
		swap(start, end);
	if (dx == 0)
		for (; start.y != end.y; ++start.y)
			putpixel(X(start.x), Y(start.y), 12);
	else if (dy == 0)
		for (; start.x != end.x; ++start.x)
			putpixel(X(start.x), Y(start.y), 12);
	else
	{
		if (slope > 0 && slope <= 1) 
		{
			slope = dy / dx;
			for (; start.x != end.x; ++start.x)
			{
				start.y += slope;
				putpixel(X(start.x), Y(static_cast<int>(start.y)), 12);
			}
		}
		if (slope < 0 && slope >= -1)
		{
			slope = dy / dx;
			for (; start.x != end.x; --start.x)
			{
				start.y += slope;
				putpixel(X(start.x), Y(static_cast<int>(start.y)), 12);
			}
		}
		/*else
		{
			slope = dx / dy;

		}*/
		// if(slope > 1 &&)
	}
} 

void swap(Point &a, Point &b)
{
	Point tmp = a;
	a = b;
	b = tmp;
}