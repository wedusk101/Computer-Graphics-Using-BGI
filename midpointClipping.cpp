#include <iostream>
#include "graphics.h"

typedef struct 
{
        int x;
        int y;
        int regionCode;
} Point;

inline int getRegionCode(const Point &p, const Point &min, const Point &max)
{
       return 8 * (p.x < min.x) + 4 * (p.x > max.x) + 2 * (p.y > max.y) + 1 * (p.y < min.y);
}

bool checkTrivialAccept(const Point &src, const Point &dst)
{
     if(!(src.regionCode | dst.regionCode))
		return true;
     else
		return false;
}

bool checkTrivialReject(const Point &src, const Point &dst)
{
     if(src.regionCode & dst.regionCode)
		return true;
     else
		return false;
}

double getLineLen(const Point &src, const Point &dst)
{
	return sqrt(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2));
}

void clipMidpoint(Point &src, Point &dst, const Point &min, const Point &max, const double &eps)
{
	src.regionCode = getRegionCode(src, min, max);
	dst.regionCode = getRegionCode(dst, min, max);

	if (checkTrivialAccept(src, dst))
	{
		line(src.x, src.y, dst.x, dst.y);
		return;
	}
	else if (checkTrivialReject(src, dst))
	{
		return;
	}
	else if (getLineLen(src, dst) < eps)
	{
		return;
	}
	else
	{
		Point mid;
		mid.x = (src.x + dst.x) / 2;
		mid.y = (src.y + dst.y) / 2;
		clipMidpoint(src, mid, min, max, eps);
		clipMidpoint(dst, mid, min, max, eps);
	}
}
 
int main()
{
    Point src, dst, min, max;
    min.x = 200, min.y = 200, max.x = 400, max.y = 400;
	min.regionCode = 0, max.regionCode = 0;
    const double EPSILON = 2.0;    

    std::cout << "Please enter the coordinates (x,y) for the source point." << std::endl;
    std::cin >> src.x;
    std::cin >> src.y;
    std::cout << "Please enter the coordinates (x,y) for the destination point." << std::endl;
    std::cin >> dst.x;
    std::cin >> dst.y;
        
    initwindow(800, 800, "Clipping");
    setcolor(12);
    rectangle(min.x, min.y, max.x, max.y); // viewport
	src.regionCode = getRegionCode(src, min, max);
	dst.regionCode = getRegionCode(dst, min, max);
	if (checkTrivialReject(src, dst))
	{
		std::cout << "Line outside viewport!" << std::endl;
		system("pause");
		closegraph();
		return 0;
	}
	setcolor(14);
    clipMidpoint(src, dst, min, max, EPSILON);
    system("pause");
	closegraph();
    return 0;
}