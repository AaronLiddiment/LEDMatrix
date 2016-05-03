// Source code is based on https://github.com/adafruit/Adafruit_NeoMatrix
// replace internal use of NeoPixel library with CRGB array to use with FastLED
//
// modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
// date:      2016/04/27
// --------------------------------------------------------------------
// Original copyright & description below
// --------------------------------------------------------------------
// Adafruit_NeoMatrix example for tiled NeoPixel matrices.  Scrolls
// 'Howdy' across three 10x8 NeoPixel grids that were created using
// NeoPixel 60 LEDs per meter flex strip.

#include <FastLED.h>
#include <FastLED_GFX.h>
#include <LEDMatrix.h>

#define LED_PIN             2
#define COLOR_ORDER         GRB
#define CHIPSET             WS2812B

#define TARGET_FRAME_TIME   25  // Desired update rate, though if too many leds it will just run as fast as it can!
#define PLASMA_X_FACTOR     24
#define PLASMA_Y_FACTOR     24

// MATRIX DECLARATION:
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = matrix layout flags, add together as needed:
//   MTX_MATRIX_TOP, MTX_MATRIX_BOTTOM, MTX_MATRIX_LEFT, MTX_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     MTX_MATRIX_TOP + MTX_MATRIX_LEFT for the top-left corner.
//   MTX_MATRIX_ROWS, MTX_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   MTX_MATRIX_PROGRESSIVE, MTX_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   MTX_TILE_TOP, MTX_TILE_BOTTOM, MTX_TILE_LEFT, MTX_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. MTX_TILE_TOP + MTX_TILE_LEFT for the top-left corner.
//   MTX_TILE_ROWS, MTX_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   MTX_TILE_PROGRESSIVE, MTX_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
// Parameter 4 = number of matrices arranged horizontally
// Parameter 5 = number of matrices arranged vertically
//   See example below for these values in action.

// Example with three 10x8 matrices (created using NeoPixel flex strip --
// these grids are not a ready-made product).  In this application we'd
// like to arrange the three matrices side-by-side in a wide display.
// The first matrix (tile) will be at the left, and the first pixel within
// that matrix is at the top left.  The matrices use zig-zag line ordering.
// There's only one row here, so it doesn't matter if we declare it in row
// or column order.  The matrices use 800 KHz (v2) pixels that expect GRB
// color data.

#define MATRIX_WIDTH        16 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_HEIGHT       16 // height of each matrix
#define MATRIX_TILE_H       4  // number of matrices arranged horizontally
#define MATRIX_TILE_V       1  // number of matrices arranged vertically
#define MATRIX_TYPE        (MTX_MATRIX_TOP + MTX_MATRIX_LEFT + MTX_MATRIX_ROWS + MTX_MATRIX_ZIGZAG + MTX_TILE_TOP + MTX_TILE_LEFT + MTX_TILE_ROWS) // matrix layout flags, add together as needed
#define MATRIX_SIZE        (MATRIX_WIDTH*MATRIX_HEIGHT)

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE, MATRIX_TILE_H, MATRIX_TILE_V> matrix;

uint16_t PlasmaTime, PlasmaShift;
uint32_t LoopDelayMS, LastLoop;

void ESP8266_yield() {
#ifdef ESP8266
  yield(); // secure time for the WiFi stack of ESP8266
#endif
}

void setup() {
  // initial FastLED
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix[0], matrix.Size());
  FastLED.setBrightness(128);
  FastLED.clear(true);
  // initial helpers for plasma animation
  LoopDelayMS = TARGET_FRAME_TIME;
  LastLoop = millis() - LoopDelayMS;
  PlasmaShift = (random8(0, 5) * 32) + 64;
  PlasmaTime = 0;
}

void loop() {
  if (abs(millis() - LastLoop) >= LoopDelayMS) {
    LastLoop = millis();
    FastLED.clear();
    // Fill background with dim plasma
    for (int16_t x=0; x<matrix.width(); x++) {
      for (int16_t y=0; y<matrix.height(); y++) {
        ESP8266_yield(); // secure time for the WiFi stack of ESP8266
        int16_t r = sin16(PlasmaTime) / 256;
        int16_t h = sin16(x * r * PLASMA_X_FACTOR + PlasmaTime) + cos16(y * (-r) * PLASMA_Y_FACTOR + PlasmaTime) + sin16(y * x * (cos16(-PlasmaTime) / 256) / 2);
        matrix(x, y) = CHSV((uint8_t)((h / 256) + 128), 255, 64);
      }
    }
    uint16_t OldPlasmaTime = PlasmaTime;
    PlasmaTime += PlasmaShift;
    if (OldPlasmaTime > PlasmaTime)
      PlasmaShift = (random8(0, 5) * 32) + 64;
    FastLED.show();
  }
}
