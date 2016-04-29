// Source code is based on https://github.com/adafruit/Adafruit_NeoMatrix
// replace internal use of NeoPixel library with CRGB array to use with FastLED
//
// modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
// date:      2016/04/27
// --------------------------------------------------------------------
// Original copyright & description below
// --------------------------------------------------------------------
// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <FastLED.h>
#include <FastLED_GFX.h>
#include <FastLEDMatrix.h>

#define LED_PIN        2
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   5 // width of FastLED matrix
#define MATRIX_HEIGHT  8 // height of matrix
#define MATRIX_TYPE    (MTX_MATRIX_TOP + MTX_MATRIX_RIGHT + MTX_MATRIX_COLUMNS + MTX_MATRIX_PROGRESSIVE) // matrix layout flags, add together as needed

// MATRIX DECLARATION:
// Parameter 1 = width of FastLED matrix
// Parameter 2 = height of matrix
// Parameter 3 = matrix layout flags, add together as needed:
//   MTX_MATRIX_TOP, MTX_MATRIX_BOTTOM, MTX_MATRIX_LEFT, MTX_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     MTX_MATRIX_TOP + MTX_MATRIX_LEFT for the top-left corner.
//   MTX_MATRIX_ROWS, MTX_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   MTX_MATRIX_PROGRESSIVE, MTX_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.

cFastLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> matrix;

const CRGB colors[] = {
  CRGB(255, 0, 0),
  CRGB(0, 255, 0),
  CRGB(0, 0, 255)
};

void setup() {
  // initial FastLED
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix[0], matrix.Size());
  FastLED.setBrightness(127);
  FastLED.clear(true);

  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int pass = 0;

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Howdy"));
  if(--x < -36) {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  FastLED.show();
  delay(100);
}
