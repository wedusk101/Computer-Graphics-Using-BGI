#include <iostream>
#include <windows.h>
#include "graphics.h"

int main()
{
	initwindow(640, 480);
	outtextxy(100, 100, "Hello, world!");
	system("pause"); // windows only feature
	closegraph();
	return 0;
}