/*

The following program renders the Mandelbrot set. This set consists of a set of complex numbers with a specific property. A complex number Z is
a number that has two parts; one real, and one imaginary. It is usually written in the form Z = a + bi, where a is the real part, b is the imaginary part
and i is the square root of -1. The Mandelbrot set consists of all such complex numbers C, such that Z = Z^2 + C [ where C is a given complex number ] and Z 
is bounded under iteration with Z (a complex number itself) starting from 0 (every real number is a complex number with the imaginary part being zero).

Therefore,

		Z_n+1 = Z^2 + C [ let C = a + bi ]
		
		=> Z_0 = 0 + (a + bi) = a + bi

		=> Z_1 = Z_0^2 + C = (a + bi)^2 + (a + bi) = (a^2 - b^2 + 2abi) + (a + bi) = (a^2 - b^2 + a) + i(2ab + b)

		=> Z_2 = Z_1^2 + C and so on ...


The modulus of a complex number Z = (a + bi) is defined as sqrt(a^2 + b^2). This is geometrically the Euclidean distance between the origin of the complex plane 
and Z. If this distance is always less than some value for a given complex number C , under the above iteration, then C is set to be bounded. If it is bounded within
the Mandelbrot set bounds, then the given complex number C is said to be a part of the Mandelbrot set.

The Mandelbrot set is what is known as a fractal pattern. It has repeating patterns visible upon zooming and is self-similar. It is named after the mathematician
Benoit Mandelbrot. It is the most well-known fractal. Computing the Mandelbrot used to be a very compute intensive task to perform once. Nowadays, it can be trivially 
computed by even processors in smart watches. 

To know more about the Mandelbrot set please refer to https://en.wikipedia.org/wiki/Mandelbrot_set

*/

#include <iostream>
#include <cmath>
#include <chrono>
#include "windows.h"
#include "graphics.h"
#include "colors.h"

#define MAX_ITR 500

struct Complex // class to support Complex numbers
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

void drawMandelbrot(const int &, const int &);
double getMappedScaleX(const int &, const int &); // maps pixel values in the X direction of screen space between (-2.5, 1)
double getMappedScaleY(const int &, const int &); // maps pixel values in the Y direction of screen space between (-1, 1)
void evalMandel(Complex &, const Complex &);

int main()
{
	int width = 0, height = 0;
	printf("Please enter the desired resolution in pixels. Width, followed by height.\n");
	scanf("%d%d", &width, &height);
	initwindow(width, height, "Mandelbrot");
	auto start = std::chrono::high_resolution_clock::now();
	drawMandelbrot(width, height);
	auto stop = std::chrono::high_resolution_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	printf("Time taken is %d seconds.\n", diff.count());
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


void evalMandel(Complex &z, const Complex &c)
{
	double zReal = z.a;
	double zImaginary = z.b;
	z.a = zReal * zReal - zImaginary * zImaginary + c.a;
	z.b = 2 * zReal * zImaginary + c.b;
}

void drawMandelbrot(const int &width, const int &height)
{
	int bound = 0;
	for (int y = 0; y < height; y++) // y axis of the image	
	{
		for (int x = 0; x < width; x++) // x axis of the image
		{
			int itr = 0;
			Complex z, c;
			c.a = getMappedScaleX((double)x, width);
			c.b = getMappedScaleY((double)y, height);
			while (z.real() * z.real() + z.imaginary() * z.imaginary() <= 2 * 2 && itr < MAX_ITR)
			{
				evalMandel(z, c);
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