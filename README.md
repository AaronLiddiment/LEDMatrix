
# LEDMatrix
------------
[![GitHub version](https://badge.fury.io/gh/Jorgen-VikingGod%2FLEDMatrix.svg)](https://badge.fury.io/gh/Jorgen-VikingGod%2FLEDMatrix) 
[![GitHub version](https://img.shields.io/badge/dependencies-FastLED-blue.svg)](https://github.com/FastLED/FastLED) 
[![GitHub version](https://img.shields.io/badge/dependencies-FastLED--GFX-456789.svg)](https://github.com/Jorgen-VikingGod/FastLED-GFX)

> A fork of ([cLEDMatrix](https://github.com/AaronLiddiment/LEDMatrix) by [Aaron Liddiment](https://github.com/AaronLiddiment)) and FastLED port of ([Adafruit-NeoMatrix](https://github.com/adafruit/Adafruit_NeoMatrix)) by using the graphics library [FastLED-GFX](https://github.com/Jorgen-VikingGod/FastLED-GFX) (based on [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library))

Once you have downloaded the Zip file, it should be extracted into your Arduino Libraries folder and the folder renamed to "LEDMatrix".

<table>
  <tr>
    <td colspan="2">
      The LEDMatrix library based on cLEDMatrix and Adafruit-NeoMatrix to create two-dimensional graphic displays using FastLED. You can then easily draw shapes, text and animation without having to calculate every X/Y pixel position. Larger displays can be formed using sections of LED strip / matrices, as shown in the photo below.
    </td>
  </tr>
  <tr style="vertical-align: top;">
    <td width="50%">
      <strong>Table of Contents</strong>
      <ul>
      <li><a href="#single-matrix-example">Signle Matrix</a>
          <ul>
            <li><a href="#parameters">Parameter</a></li>
            <li><a href="#includes">Includes</a></li>
            <li><a href="#decleration">Decleration</a></li>
            <li><a href="#initialize-fastled">Initialize FastLED</a></li>
          </ul>
        </li>
        <li><a href="#tile-matrix-example">Tile Matrix</a>
          <ul>
            <li><a href="#parameters-1">Parameter</a></li>
            <li><a href="#includes-1">Includes</a></li>
            <li><a href="#decleration-1">Decleration</a></li>
            <li><a href="#initialize-fastled-1">Initialize FastLED</a></li>
            <li><a href="#initialize-fastled-multiple-controller">Initialize FastLED (multiple controller)</a></li>
          </ul>
        </li>
        <li><a href="#available-methods">Available Methods</a></li>
        </li>
        <li><a href="#graphic-library-fastled-gfx">Graphic Library</a>
          <ul>
            <li><a href="#available-methods-1">Available Methods</a></li>
          </ul>
        </li>
      </ul>
    </td>
    <td>
      <img src="https://cdn-learn.adafruit.com/assets/assets/000/010/704/medium640/leds_hairy.png?1377844757" alt="Espruino Pico OLED NFC" />
    </td>
  </tr>
</table>

## Single Matrix ([Example](examples/matrixtest/matrixtest.ino))
----------------------------------------------------------------
### Parameters
| Parameter    | Description                                 |
| ------------ |---------------------------------------------|
| Parameter 1  | width of matrix                             |
| Parameter 2  | height of matrix                            |
| Parameter 3  | matrix layout flags, add together as needed |

### Includes
```c
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <LEDMatrix.h>
```

### Decleration
```c
// declare FastLED (matrix / LED strip)
#define LED_PIN        2
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

// declare matrix
#define MATRIX_WIDTH   5 // width of matrix
#define MATRIX_HEIGHT  8 // height of matrix
#define MATRIX_TYPE    (MTX_MATRIX_TOP + MTX_MATRIX_RIGHT + MTX_MATRIX_COLUMNS + MTX_MATRIX_PROGRESSIVE) // matrix layout flags, add together as needed

// create our matrix based on matrix definition
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> matrix;
```

### Initialize FastLED
```c
void setup() {
  // initial FastLED by using CRGB led source from our matrix class
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix[0], matrix.Size());
  FastLED.setBrightness(127);
  FastLED.clear(true);
}
```


## Tile Matrix ([Example](examples/tiletest/tiletest.ino))
----------------------------------------------------------------
### Parameters
| Parameter    | Description                                       |
| ------------ |---------------------------------------------------|
| Parameter 1  | width of EACH NEOPIXEL MATRIX (not total display) |
| Parameter 2  | height of each matrix                             |
| Parameter 3  | matrix layout flags, add together as needed       |
| Parameter 4  | number of matrices arranged horizontally          |
| Parameter 5  | number of matrices arranged vertically            |


### Includes
```c
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <LEDMatrix.h>
```

### Decleration
```c
// declare FastLED (matrix / LED strip)
#define LED_PIN        2
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

// declare matrix
#define MATRIX_WIDTH        16 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_HEIGHT       16 // height of each matrix
#define MATRIX_TILE_H       4  // number of matrices arranged horizontally
#define MATRIX_TILE_V       1  // number of matrices arranged vertically
#define MATRIX_TYPE        (MTX_MATRIX_TOP + MTX_MATRIX_LEFT + MTX_MATRIX_ROWS + MTX_MATRIX_ZIGZAG + MTX_TILE_TOP + MTX_TILE_LEFT + MTX_TILE_ROWS) // matrix layout flags, add together as needed
#define MATRIX_SIZE        (MATRIX_WIDTH*MATRIX_HEIGHT)

// create our matrix based on matrix definition
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE, MATRIX_TILE_H, MATRIX_TILE_V> matrix;
```

### Initialize FastLED
```c
void setup() {
  // initial FastLED by using CRGB led source from our matrix class
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(matrix[0], matrix.Size());
  FastLED.setBrightness(127);
  FastLED.clear(true);
}
```

### Initialize FastLED (multiple controller)
```c
void setup() {
  // initial FastLED with multiple controller, by using CRGB led source from each matrix panal
  // panel 1 (from   0 to  255)
  FastLED.addLeds<WS2812B, PANEL_1_DATA_PIN, GRB>(matrix[0], 0, MATRIX_SIZE);
  // panel 2 (from 255 to  511)
  FastLED.addLeds<APA102, PANEL_2_DATA_PIN, PANEL_2_CLOCK_PIN, RGB>(matrix[0], 1*MATRIX_SIZE, MATRIX_SIZE);
  // panel 3 (from 512 to  767)
  FastLED.addLeds<SK9822, PANEL_3_DATA_PIN, PANEL_3_CLOCK_PIN, BGR>(matrix[0], 2*MATRIX_SIZE, MATRIX_SIZE);
  // panel 4 (from 768 to 1023)
  FastLED.addLeds<WS2812B, PANEL_4_DATA_PIN, GRB>(matrix[0], 3*MATRIX_SIZE, MATRIX_SIZE);
  FastLED.setBrightness(127);
  FastLED.clear(true);
}
```

## Available Methods
```c
uint16_t mXY(uint16_t x, uint16_t y)
void SetLEDArray(struct CRGB *pLED)

struct CRGB *operator[](int n)
struct CRGB &operator()(int16_t x, int16_t y)
struct CRGB &operator()(int16_t i)

int Size()
int Width()
int Height()

void HorizontalMirror(bool FullHeight = true)
void VerticalMirror()
void QuadrantMirror()
void QuadrantRotateMirror()
void TriangleTopMirror(bool FullHeight = true)
void TriangleBottomMirror(bool FullHeight = true)
void QuadrantTopTriangleMirror()
void QuadrantBottomTriangleMirror()

void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color)
void DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color)

void drawPixel(int n, CRGB color)
void drawPixel(int16_t x, int16_t y, CRGB color)
struct CRGB & pixel(int n)
struct CRGB & pixel(int16_t x, int16_t y)
void fillScreen(CRGB color)
void setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t))
```

## Graphic Library ([FastLED-GFX](https://github.com/Jorgen-VikingGod/FastLED-GFX))
----------------------------------------------------------------
Simple FastLED port of ([Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library))

### Available Methods
```c
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void drawFastVLine(int16_t x, int16_t y, int16_t h, CRGB color)
void drawFastHLine(int16_t x, int16_t y, int16_t w, CRGB color)
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color)
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, CRGB color)
void fillScreen(CRGB color)
void invertDisplay(boolean i)

void drawCircle(int16_t x0, int16_t y0, int16_t r, CRGB color)
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, CRGB color)
void fillCircle(int16_t x0, int16_t y0, int16_t r, CRGB color)
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, CRGB color)
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color)
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color)
void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, CRGB color)
void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, CRGB color)
void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, CRGB color)
void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, CRGB color, CRGB bg)
void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, CRGB color)
void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, CRGB color, CRGB bg)
void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, CRGB color)
void drawChar(int16_t x, int16_t y, unsigned char c, CRGB color, CRGB bg, uint8_t size)
void setCursor(int16_t x, int16_t y)
void setTextColor(CRGB c)
void setTextColor(CRGB c, CRGB bg)
void setTextSize(uint8_t s)
void setTextWrap(boolean w)
void setRotation(uint8_t r)
void cp437(boolean x=true)
void setFont(const GFXfont *f = NULL)
void getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)

size_t write(uint8_t)

int16_t height(void) const
int16_t width(void) const

uint8_t getRotation(void) const

int16_t getCursorX(void) const
int16_t getCursorY(void) const
```
