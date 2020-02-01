#include <iostream>
#include <cmath>
#include "windows.h"
#include "graphics.h"
#include "colors.h"

#define MAX_ITR 500

void drawMandelbrot(const int &, const int &);
double getMappedScaleX(const int&, const int &); // maps pixel values in the X direction of screen space between (-2.5, 1)
double getMappedScaleY(const int&, const int &); // maps pixel values in the Y direction of screen space between (-1, 1)

struct Complex
{
	double a;
	double b;

	Complex() : a(0), b(0) {}
	Complex(double a_, double b_) : a(a_), b(b_) {}

	double real() const
	{
		return a;
	}

	double imaginary() const
	{
		return b;
	}

	double getModulus()
	{
		return sqrt(a * a + b * b);
	}
};

int main()
{
	int width = 0, height = 0;
	printf("Please enter the desired resolution in pixels. Width, followed by height.\n");
	scanf("%d%d", &width, &height);
	initwindow(width, height, "Mandelbrot");
	drawMandelbrot(width, height);
	system("pause");
	closegraph();
	return 0;
}

double getMappedScaleX(const int &x, const int &xMax)
{
	return ((x / (double) xMax) * 3.5) - 2.5;
}

double getMappedScaleY(const int &y, const int &yMax)
{
	return ((y / (double) yMax) * 2) - 1;
}

void drawMandelbrot(const int &width, const int &height)
{
	int bound = 0;
	for (int y = 0; y < height; y++) // y axis of the image	
	{
		for (int x = 0; x < width; x++) // x axis of the image
		{
			int itr = 0;
			// Complex c(x, y);
			double cX = 0; // real part of C
			double cY = 0; // imaginary part of C
			double mandelScaleX = getMappedScaleX((double)x, width);
			double mandelScaleY = getMappedScaleY((double)y, height);
			while (cX * cX + cX * cY <= 2 * 2 && itr < MAX_ITR)
			{
				double xBak = cX * cX - cY * cY + mandelScaleX;
				cY = 2 * cX * cY + mandelScaleY;
				cX = xBak;
				itr++;
			}
			if (itr < MAX_ITR)
			{
				setcolor(BLACK);
				putpixel(x, y, BLACK);
			}
			else
			{
				setcolor(CYAN);
				putpixel(x, y, CYAN);
			}
		}
	}
}