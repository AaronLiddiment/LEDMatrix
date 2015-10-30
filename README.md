cLEDMatrix Instructions
========================


Overview
--------
This class aims to make using a matrix of LED's much simpler by allowing you to use
x and y to access it. It also copes with many wiring schemes while still allowing
(0,0) to be defined as any of the corners. Plus it adds several mirroring and
primitive line and shape drawing functions.



Initialise
----------
In order to use the class you must have the following header files included in your program:-

	#include <FastLED.h>
	#include <LEDMatrix.h>

You must declare an instance of the cLEDMatrix class as this is used to modify the
LED data according to the matrix dimensions and layout. Personally I use defines to
make this clearer in the code, such as:-

	#define MATRIX_WIDTH   68
	#define MATRIX_HEIGHT  7
	#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

	cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

There are four matrix types which are delcared at the top of the LEDMatrix header file.
HORIZONTAL_MATRIX, VERTICAL_MATRIX, HORIZONTAL_ZIGZAG_MATRIX and VERTICAL_ZIGZAG_MATRIX.
If your wired origin is not in the corner that you would like as (0, 0) you can make the
appropriate dimension negative to reverse that axis.
The LED array is allocated in the matrix class but the address of the array can be obtained by
using '[0]' after the matrix variable name and '.Size()' to obtain the number of LED's:-

	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());



Functions
=========

int Size();
-----------
Returns the total number of LEDs.


int Width();
------------
Returns the matrix width.


CRGB *matrix[x]
---------------
Returns the memory address of the LED x.


CRGB &matrix(x, y)
------------------
Allows you to set an LED with a CRGB colour value.


HorizontalMirror();
-------------------
Mirrors the matrix horizontally.


VerticalMirror();
-----------------
Mirrors the matrix vertically.


QuadrantMirror();
-----------------
Mirrors the matrix bottom left quadrant to the other three.


QuadrantRotateMirror();
-----------------------
Rotates the matrix bottom left quadrant to the other three.


TriangleTopMirror();
--------------------
Mirrors the matrix upper left corner to the bottom right corner.


TriangleBottomMirror();
-----------------------
Mirrors the matrix bottom left corner to the top right corner.


QuadrantTopTriangleMirror();
----------------------------
Mirrors the matrix bottom left corner quadrants upper left corner to its bottom right corner.


QuadrantBottomTriangleMirror();
-------------------------------
Mirrors the matrix bottom left corner quadrants bottom left corner to its upper right corner.


DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
-------------------------------------------------------------------
Draws a line from (x0, y0) to (x1, y1) with Col.


DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
------------------------------------------------------------------------
Draws a rectangle (x0, y0) to (x1, y1) with Col.


DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
---------------------------------------------------------
Draws a circle with center (xc, yc) and a radius of r with Col.


DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
------------------------------------------------------------------------
Draws a filled rectangle (x0, y0) to (x1, y1) with Col.


DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
---------------------------------------------------------
Draws a filled circle with center (xc, yc) and a radius of r with Col.
