#include <iostream>
#include <climits>
#include <cmath>
#include "graphics.h"

typedef struct point
{
        int x;
        int y;
} Point;

void bresenham(Point, Point);

int main()
{
    Point start, end;
    printf("Please enter the coordinates for the starting point of the line.\n");
    scanf("%d%d", &start.x, &start.y);
    printf("Please enter the coordinates for the end point of the line.\n");
    scanf("%d%d", &end.x, &end.y);
    bresenham(start, end);
    closegraph();
    return 0;
}
    
void bresenham(Point start, Point end)
{
     int dy = end.y - start.y, dx = end.x - start.x;
     double slope = 0.0;
     if(dx == 0)
           slope = INT_MAX;
     else
           slope = abs(dy/dx);
    
