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

void clipMidpoint(Point &src, Point &dst, const Point &min, const Point &max)
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
		std::cout << "Line outside viewport!" << std::endl;
		return;
	}
	else
	{
		Point mid;
		mid.x = (src.x + dst.x) / 2;
		mid.y = (src.y + dst.y) / 2;
		// mid.regionCode = getRegionCode(mid, min, max);
		clipMidpoint(src, mid, min, max);
		clipMidpoint(dst, mid, min, max);
	}
}
 

int main()
{
    Point src, dst, min, max;
    min.x = 200, min.y = 200, max.x = 400, max.y = 400;
	min.regionCode = 0, max.regionCode = 0;
    
    const int EPSILON = 0.001;
    
    
    std::cout << "Please enter the coordinates (x,y) for the source point." << std::endl;
    std::cin >> src.x;
    std::cin >> src.y;
    std::cout << "Please enter the coordinates (x,y) for the destination point." << std::endl;
    std::cin >> dst.x;
    std::cin >> dst.y;
    
    
    // src.regionCode = getRegionCode(src, min, max);
    // dst.regionCode = getRegionCode(dst, min, max);
    
    // std::cout << "SRC Code: " << src.regionCode << " DST Code: " << dst.regionCode <<std::endl;
    // std::cout << (src.regionCode | dst.regionCode) << " " << (src.regionCode & dst.regionCode) << std::endl;
    
    
    initwindow(800, 800, "Clipping");
    
    setcolor(12);
    
    rectangle(min.x, min.y, max.x, max.y); // viewport
    
    
    clipMidpoint(src, dst, min, max);
             
	system("pause");
                                 
    return 0;
}