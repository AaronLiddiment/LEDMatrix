#include <FastLED.h>

#include <LEDMatrix.h>

// Change the next 6 defines to match your matrix type and size

#define LED_PIN        2
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   80  // Set this negative if physical led 0 is opposite to where you want logical 0
#define MATRIX_HEIGHT  10  // Set this negative if physical led 0 is opposite to where you want logical 0
#define MATRIX_TYPE    HORIZONTAL_MATRIX  // See top of LEDMatrix.h for matrix wiring types

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

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

