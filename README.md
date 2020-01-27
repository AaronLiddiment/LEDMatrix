
# LEDMatrix
------------
[![Travis build](https://img.shields.io/travis/Jorgen-VikingGod/LEDMatrix.svg)](https://travis-ci.org/Jorgen-VikingGod/LEDMatrix) 
[![GitHub version](https://img.shields.io/github/release/Jorgen-VikingGod/LEDMatrix.svg)](https://github.com/Jorgen-VikingGod/LEDMatrix/releases/latest) 
[![FastLED dependencies](https://img.shields.io/badge/dependencies-FastLED-blue.svg)](https://github.com/FastLED/FastLED) 

> A fork of ([cLEDMatrix](https://github.com/AaronLiddiment/LEDMatrix) by [Aaron Liddiment](https://github.com/AaronLiddiment)) and build in some features from ([Adafruit-NeoMatrix](https://github.com/adafruit/Adafruit_NeoMatrix))

As another option, if you would like full Adafruit::Neomatrix support, you can check this library: https://github.com/marcmerlin/FastLED_NeoMatrix  A big plus of access to the full Adafruit GFX compatibility is font support

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
| Parameter    | Description                                   |
| ------------ |-----------------------------------------------|
| Parameter 1  | width of matrix (negative for reverse order)  |
| Parameter 2  | height of matrix  (negative for reverse order)|
| Parameter 3  | MatrixType_t = matrix layout type             |

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

### Declaration
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
cLEDMatrix<[-]MATRIX_WIDTH, [-]MATRIX_HEIGHT, MATRIX_TYPE> leds;
To cope with a matrix wired right to left, add - in front of your width. Similarly if the matrix is wired down to up, add - in front of height.
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

### Declaration
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
#define MATRIX_TILE_H       1  // number of matrices arranged horizontally (negative for reverse order)
#define MATRIX_TILE_V       8  // number of matrices arranged vertically (negative for reverse order)
#define MATRIX_SIZE         (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_PANEL        (MATRIX_WIDTH*MATRIX_HEIGHT)

#define MATRIX_WIDTH        (MATRIX_TILE_WIDTH*MATRIX_TILE_H)
#define MATRIX_HEIGHT       (MATRIX_TILE_HEIGHT*MATRIX_TILE_V)

#define NUM_LEDS            (MATRIX_WIDTH*MATRIX_HEIGHT)

// create our matrix based on matrix definition
cLEDMatrix<[-]MATRIX_TILE_WIDTH, [-]MATRIX_TILE_HEIGHT, HORIZONTAL_ZIGZAG_MATRIX, [-]MATRIX_TILE_H, [-]MATRIX_TILE_V, VERTICAL_BLOCKS> leds;
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

## Compatibility with SmartMatrix supported RGBPanels

NeoMatrix displays can be big, typically 1cm^2 per pixel, sometimes more. If you need Matrices that are both smaller and cheaper, you can use RGBPanels which are supported by SmartMatrix: https://github.com/pixelmatix/SmartMatrix  
If you also install https://github.com/marcmerlin/SmartMatrix_GFX you get a compat layer that lets you run FastLED and LEDMatrix code. See https://github.com/marcmerlin/FastLED_NeoMatrix_SmartMatrix_LEDMatrix_GFX_Demos/tree/master/LEDMatrix for examples.

## Running/Debugging your LEDMatrix code on a Linux Computer

For ease of development, you can develop and debug your code on linux using ArduinoOnPc-FastLED-GFX-LEDMatrix.

http://marc.merlins.org/perso/arduino/post_2020-01-24_Running-Arduino-code-with-2D-FastLED_-Adafruit_GFX_-and-LEDMatrix-displays-on-Linux.html introduces https://github.com/marcmerlin/ArduinoOnPc-FastLED-GFX-LEDMatrix

This solution allows you to build arduino code so that it works on linux (you can run it in a VM if you aren't running linux natively) and uses these layers:
- https://github.com/marcmerlin/ArduinoOnPc-FastLED-GFX-LEDMatrix
- https://github.com/marcmerlin/Framebuffer_GFX is the base arduino framebuffer that also supports more 2D arduino code in addition to Adafruit::GFX, including code that uses LEDMatrix.
- https://github.com/marcmerlin/FastLED_TFTWrapper_GFX is the driver that bridges that framebuffer with X11 (LINUX_RENDERER_X11) and the APIs it supports (FastLED, Adafruit::GFX, and LEDMatrix), with rpi-rgb-led-matrix for display. The other option is to use the faster LINUX_RENDERER_SDL which emulates FastLED and allows running native FastLED::NeoMatrix
