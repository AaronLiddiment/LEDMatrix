#include <FastLED.h>
#include <FastLED_GFX.h> // https://github.com/Jorgen-VikingGod/FastLED-GFX
#include <LEDMatrix.h>

// Change the next 6 defines to match your matrix type and size

#define LED_PIN             2
#define COLOR_ORDER         GRB
#define CHIPSET             WS2812B

#define MATRIX_TILE_WIDTH   16  // width of each matrix (not total display)
#define MATRIX_TILE_HEIGHT  16  // height of each matrix
#define NUM_H_TILES         4   // number of matrices arranged horizontally
#define NUM_V_TILES         1   // number of matrices arranged vertically
#define MATRIX_LAYOUT       MTX_TILE_TOP + MTX_TILE_LEFT + MTX_TILE_ROWS + MTX_TILE_PROGRESSIVE +
                            MTX_MATRIX_TOP + MTX_MATRIX_LEFT + MTX_MATRIX_ROWS + MTX_MATRIX_ZIGZAG;

// MATRIX DECLARATION:
// Parameter 1 = width of each matrix (not total display)
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

// Example with four 16x16 matrices. In this application we'd
// like to arrange the four matrices side-by-side in a wide display.
// The first matrix (tile) will be at the left, and the first pixel within
// that matrix is at the top left. The matrices use zig-zag line ordering.
// There's only one row here, so it doesn't matter if we declare it in row
// or column order.
cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, MATRIX_LAYOUT, NUM_H_TILES, NUM_V_TILES> leds;

uint8_t angle = 0;

void setup()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(127);
  FastLED.clear(true);
  delay(500);
  FastLED.showColor(CRGB::Red);
  delay(1000);
  FastLED.showColor(CRGB::Lime);
  delay(1000);
  FastLED.showColor(CRGB::Blue);
  delay(1000);
  FastLED.showColor(CRGB::White);
  delay(1000);
  FastLED.clear(true);

  // Scottish Flag
  leds.DrawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(0, 0, 255));
  leds.DrawRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawLine(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawLine(0, 1, leds.Width() - 1, leds.Height() - 2, CRGB(255, 255, 255));
  leds.DrawLine(0, leds.Height() - 1, leds.Width() - 1, 0, CRGB(255, 255, 255));
  leds.DrawLine(0, leds.Height() - 2, leds.Width() - 1, 1, CRGB(255, 255, 255));
  FastLED.show();
  delay(5000);

  // Japanese Flag
  leds.DrawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  uint16_t r = min((leds.Width() - 1) / 2, (leds.Height() - 1) / 2) - 1;
  leds.DrawFilledCircle((leds.Width() - 1) / 2, (leds.Height() - 1) / 2, r, CRGB(255, 0, 0));
  FastLED.show();
  delay(5000);

  // Norwegian Flag
  int16_t x = (leds.Width() / 4);
  int16_t y = (leds.Height() / 2) - 2;
  leds.DrawFilledRectangle(0, 0, x, y, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(0, 0, x - 1, y - 1, CRGB(255, 0, 0));
  leds.DrawFilledRectangle(x + 3, 0, leds.Width() - 1, y, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(x + 4, 0, leds.Width() - 1, y - 1, CRGB(255, 0, 0));
  leds.DrawFilledRectangle(0, y + 3, x, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(0, y + 4, x - 1, leds.Height() - 1, CRGB(255, 0, 0));
  leds.DrawFilledRectangle(x + 3, y + 3, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(x + 4, y + 4, leds.Width() - 1, leds.Height() - 1, CRGB(255, 0, 0));
  leds.DrawLine(0, y + 1, leds.Width() - 1, y + 1, CRGB(0, 0, 255));
  leds.DrawLine(0, y + 2, leds.Width() - 1, y + 2, CRGB(0, 0, 255));
  leds.DrawLine(x + 1, 0, x + 1, leds.Height() - 1, CRGB(0, 0, 255));
  leds.DrawLine(x + 2, 0, x + 2, leds.Height() - 1, CRGB(0, 0, 255));
  FastLED.show();
  delay(5000);
  leds.ShiftLeft();
}


void loop()
{
  uint8_t h = sin8(angle);
  leds.ShiftLeft();
  for (int16_t y=leds.Height()-1; y>=0; --y)
  {
    leds(leds.Width()-1, y) = CHSV(h, 255, 255);
    h += 32;
  }
  angle += 4;
  FastLED.show();
  delay(20);
}
