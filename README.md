
# LEDMatrix
------------
[![Travis build](https://img.shields.io/travis/Jorgen-VikingGod/LEDMatrix.svg)](https://travis-ci.org/Jorgen-VikingGod/LEDMatrix) 
[![GitHub version](https://img.shields.io/github/release/Jorgen-VikingGod/LEDMatrix.svg)](https://github.com/Jorgen-VikingGod/LEDMatrix/releases/latest) 
[![FastLED dependencies](https://img.shields.io/badge/dependencies-FastLED-blue.svg)](https://github.com/FastLED/FastLED) 

> A fork of ([cLEDMatrix](https://github.com/AaronLiddiment/LEDMatrix) by [Aaron Liddiment](https://github.com/AaronLiddiment)) and build in some features from ([Adafruit-NeoMatrix](https://github.com/adafruit/Adafruit_NeoMatrix))

Once you have downloaded the Zip file, it should be extracted into your Arduino Libraries folder and the folder renamed to "LEDMatrix".

<table>
  <tr>
    <td colspan="2">
      The LEDMatrix library builds a giant two-dimensional graphics array which can be used by FastLED. You can then easily draw shapes, text and animation without having to calculate every X/Y pixel position. Larger displays can be formed using tiles of LED strip / matrices - to build one big matrix. Similar to Adafruit-NeoMatrix (see picture below as example).
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
      </ul>
    </td>
    <td>
      <img src="https://cdn-learn.adafruit.com/assets/assets/000/010/704/medium640/leds_hairy.png?1377844757" alt="Espruino Pico OLED NFC" />
    </td>
  </tr>
</table>

## Single Matrix ([Example](examples/MatrixExample1/MatrixExample1.ino))
----------------------------------------------------------------
### Parameters
| Parameter    | Description                                 |
| ------------ |---------------------------------------------|
| Parameter 1  | width of matrix                             |
| Parameter 2  | height of matrix                            |
| Parameter 3  | MatrixType_t = matrix layout type           |

```c
enum MatrixType_t { HORIZONTAL_MATRIX,
                    VERTICAL_MATRIX,
                    HORIZONTAL_ZIGZAG_MATRIX,
                    VERTICAL_ZIGZAG_MATRIX };

enum BlockType_t  { HORIZONTAL_BLOCKS,
                    VERTICAL_BLOCKS,
                    HORIZONTAL_ZIGZAG_BLOCKS,
                    VERTICAL_ZIGZAG_BLOCKS };
```

### Includes
```c
#include <FastLED.h>
#include <LEDMatrix.h>
```

### Decleration
```c
// Change the next defines to match your matrix type and size
#define DATA_PIN            D5

#define COLOR_ORDER         GRB
#define CHIPSET             WS2812B

// initial matrix layout (to get led strip index by x/y)
#define MATRIX_WIDTH   11
#define MATRIX_HEIGHT  11
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX
#define MATRIX_SIZE    (MATRIX_WIDTH*MATRIX_HEIGHT)
#define NUMPIXELS      MATRIX_SIZE

// create our matrix based on matrix definition
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;
```

### Initialize FastLED
```c
void setup() {
  // initial FastLED by using CRGB led source from our matrix class
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds[0], leds.Size()).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(127);
  FastLED.clear(true);
}
```


## Tile Matrix ([Example](examples/MatrixTilesExample/MatrixTilesExample.ino))
----------------------------------------------------------------
### Parameters
| Parameter    | Description                                       |
| ------------ |---------------------------------------------------|
| Parameter 1  | width of EACH NEOPIXEL MATRIX (not total display) |
| Parameter 2  | height of each matrix                             |
| Parameter 3  | MatrixType_t = matrix layout type                 |
| Parameter 4  | number of matrices arranged horizontally          |
| Parameter 5  | number of matrices arranged vertically            |
| Parameter 6  | BlockType_t = layout of each matrix tile          |

```c
enum MatrixType_t { HORIZONTAL_MATRIX,
                    VERTICAL_MATRIX,
                    HORIZONTAL_ZIGZAG_MATRIX,
                    VERTICAL_ZIGZAG_MATRIX };

enum BlockType_t  { HORIZONTAL_BLOCKS,
                    VERTICAL_BLOCKS,
                    HORIZONTAL_ZIGZAG_BLOCKS,
                    VERTICAL_ZIGZAG_BLOCKS };
```

### Includes
```c
#include <FastLED.h>
#include <LEDMatrix.h>
```

### Decleration
```c
// Change the next defines to match your matrix type and size
#define DATA_PIN            D2
#define CLOCK_PIN           D1
#define DATA2_PIN           D4
#define CLOCK2_PIN          D3

#define COLOR_ORDER         BGR
#define CHIPSET             APA102

#define MATRIX_TILE_WIDTH   16 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT  8 // height of each matrix
#define MATRIX_TILE_H       1  // number of matrices arranged horizontally
#define MATRIX_TILE_V       8  // number of matrices arranged vertically
#define MATRIX_SIZE         (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_PANEL        (MATRIX_WIDTH*MATRIX_HEIGHT)

#define MATRIX_WIDTH        (MATRIX_TILE_WIDTH*MATRIX_TILE_H)
#define MATRIX_HEIGHT       (MATRIX_TILE_HEIGHT*MATRIX_TILE_V)

#define NUM_LEDS            (MATRIX_WIDTH*MATRIX_HEIGHT)

// create our matrix based on matrix definition
cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, HORIZONTAL_ZIGZAG_MATRIX, MATRIX_TILE_H, MATRIX_TILE_V, VERTICAL_BLOCKS> leds;
```

### Initialize FastLED
```c
void setup() {
  // initial FastLED by using CRGB led source from our matrix class
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(127);
  FastLED.clear(true);
}
```

### Initialize FastLED (multiple controller)
```c
void setup() {
  // initial FastLED with multiple controller, by using CRGB led source from each matrix panal  
  FastLED.addLeds<CHIPSET, DATA_PIN,  CLOCK_PIN,  COLOR_ORDER>(leds[0], 0,             leds.Size()/2).setCorrection(TypicalSMD5050);
  FastLED.addLeds<CHIPSET, DATA2_PIN, CLOCK2_PIN, COLOR_ORDER>(leds[0], leds.Size()/2, leds.Size()/2).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(127);
  FastLED.clear(true);
}
```

## Available Methods
```c
virtual uint16_t mXY(uint16_t x, uint16_t y)
void SetLEDArray(struct CRGB *pLED)

void ShiftLeft(void)
void ShiftRight(void)
void ShiftDown(void)
void ShiftUp(void)

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

void DrawPixel(int16_t x, int16_t y, CRGB color)
void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color)
void DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color)
void DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color)
```
