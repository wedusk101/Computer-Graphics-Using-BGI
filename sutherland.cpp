#include <iostream>
#include "graphics.h"
#include "conio.h"

typedef struct 
{
        int x;
        int y;
        int regionCode;
} Point;

inline int getRegionCode(const Point &p, const Point &min, const Point &max)
{
       return 1000 * (p.x < min.x) + 100 * (p.x > max.x) + 10 * (p.y > max.y) + 1 * (p.y < min.y);
}

bool checkTrivialAccept(const Point &src, const Point &dst)
{
     if(src.regionCode | dst.regionCode == 0)
                       return true;
     else
                       return false;
}

bool checkTrivialReject(const Point &src, const Point &dst)
{
     if(src.regionCode & dst.regionCode != 0)
                       return true;
     else
                       return false;
}

/*void sutherlandCohen(Point &src, Point &dst)
{
     
}*/
 

int main()
{
    Point src, dst, min, max;
    min.x = 200, min.y = 200, max.x = 400, max.y = 400;
    
    
    std::cout << "Please enter the coordinates (x,y) for the source point." << std::endl;
    std::cin >> src.x;
    std::cin >> src.y;
    std::cout << "Please enter the coordinates (x,y) for the destination point." << std::endl;
    std::cin >> dst.x;
    std::cin >> dst.y;
    
    
    src.regionCode = getRegionCode(src, min, max);
    dst.regionCode = getRegionCode(dst, min, max);
    
    std::cout << "SRC Code: " << src.regionCode << " DST Code: " << dst.regionCode <<std::endl;
    std::cout << (src.regionCode | dst.regionCode) << " " << (src.regionCode & dst.regionCode) << std::endl;
    
    
    initwindow(640,480, "Clipping");
    
    setcolor(12);
    
    rectangle(min.x, min.y, max.x, max.y); // viewport
    
    if(checkTrivialAccept(src, dst))
            line(src.x, src.y, dst.x, dst.y);
    else if(checkTrivialReject(src, dst))
            std::cout << "Line outside viewport!" <<  std::endl;
    else
         //sutherlandCohen(src, dst);
         std::cout << "Test!" << std::endl;
         
    getch();
                                 
    return 0;
}
