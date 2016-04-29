/*--------------------------------------------------------------------
  Source code is based on https://github.com/adafruit/Adafruit_NeoMatrix
  and on https://github.com/AaronLiddiment/LEDMatrix
  replace internal use of NeoPixel library with CRGB array to use with FastLED

  modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
  date:      2016/04/27
  --------------------------------------------------------------------
  Original copyright & description below
  --------------------------------------------------------------------
  This file is part of the Adafruit NeoMatrix library.

  NeoMatrix is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoMatrix is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoMatrix.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------
  LEDMatrix V4 class by Aaron Liddiment (c) 2015
  Inspiration for some of the Matrix functions from Stefan Petrick
  FastLED v3.1 library by Daniel Garcia and Mark Kriegsmann.
  Written & tested on a Teensy 3.1 using Arduino V1.6.3 & teensyduino V1.22
  --------------------------------------------------------------------*/

#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif
#include <FastLED_GFX.h>

// Matrix layout information is passed in the 'matrixType' parameter for
// each constructor.

// These define the layout for a single 'unified' matrix (e.g. one made
// from NeoPixel strips, or a single NeoPixel shield), or for the pixels
// within each matrix of a tiled display (e.g. multiple NeoPixel shields).
#define MTX_MATRIX_TOP         0x00 // Pixel 0 is at top of matrix
#define MTX_MATRIX_BOTTOM      0x01 // Pixel 0 is at bottom of matrix
#define MTX_MATRIX_LEFT        0x00 // Pixel 0 is at left of matrix
#define MTX_MATRIX_RIGHT       0x02 // Pixel 0 is at right of matrix
#define MTX_MATRIX_CORNER      0x03 // Bitmask for pixel 0 matrix corner
#define MTX_MATRIX_ROWS        0x00 // Matrix is row major (horizontal)
#define MTX_MATRIX_COLUMNS     0x04 // Matrix is column major (vertical)
#define MTX_MATRIX_AXIS        0x04 // Bitmask for row/column layout
#define MTX_MATRIX_PROGRESSIVE 0x00 // Same pixel order across each line
#define MTX_MATRIX_ZIGZAG      0x08 // Pixel order reverses between lines
#define MTX_MATRIX_SEQUENCE    0x08 // Bitmask for pixel line order

// These apply only to tiled displays (multiple matrices):
#define MTX_TILE_TOP           0x00 // First tile is at top of matrix
#define MTX_TILE_BOTTOM        0x10 // First tile is at bottom of matrix
#define MTX_TILE_LEFT          0x00 // First tile is at left of matrix
#define MTX_TILE_RIGHT         0x20 // First tile is at right of matrix
#define MTX_TILE_CORNER        0x30 // Bitmask for first tile corner
#define MTX_TILE_ROWS          0x00 // Tiles ordered in rows
#define MTX_TILE_COLUMNS       0x40 // Tiles ordered in columns
#define MTX_TILE_AXIS          0x40 // Bitmask for tile H/V orientation
#define MTX_TILE_PROGRESSIVE   0x00 // Same tile order across each line
#define MTX_TILE_ZIGZAG        0x80 // Tile order reverses between lines
#define MTX_TILE_SEQUENCE      0x80 // Bitmask for tile line order

class cLEDMatrixBase: public FastLED_GFX {
friend class cSprite;

protected:
  struct CRGB *m_LED;
  struct CRGB m_OutOfBounds;
  uint8_t type;
  uint8_t matrixWidth;
  uint8_t matrixHeight;
  uint8_t tilesX;
  uint8_t tilesY;
  uint16_t (*remapFn)(uint16_t x, uint16_t y);

public:
  cLEDMatrixBase(int w, int h);

  virtual uint16_t mXY(uint16_t x, uint16_t y);
  void SetLEDArray(struct CRGB *pLED);	// Only used with externally defined LED arrays

  struct CRGB *operator[](int n);
  struct CRGB &operator()(int16_t x, int16_t y);
  struct CRGB &operator()(int16_t i);

  int Size()   { return(_width * _height); }
  int Width()  { return(_width); }
  int Height() { return(_height); }

  void HorizontalMirror(bool FullHeight = true);
  void VerticalMirror();
  void QuadrantMirror();
  void QuadrantRotateMirror();
  void TriangleTopMirror(bool FullHeight = true);
  void TriangleBottomMirror(bool FullHeight = true);
  void QuadrantTopTriangleMirror();
  void QuadrantBottomTriangleMirror();

  void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
  void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
  void DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color);
  void DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
  void DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color);

  void drawPixel(int n, CRGB color);
  void drawPixel(int16_t x, int16_t y, CRGB color);
  struct CRGB & pixel(int n);
  struct CRGB & pixel(int16_t x, int16_t y);
  void fillScreen(CRGB color);
  void setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t));
};

template<uint8_t matrixW, uint8_t matrixH, uint8_t matrixType = MTX_MATRIX_TOP + MTX_MATRIX_LEFT + MTX_MATRIX_ROWS + MTX_MATRIX_ZIGZAG + MTX_TILE_TOP + MTX_TILE_LEFT + MTX_TILE_ROWS + MTX_TILE_ZIGZAG, uint8_t tX = 0, uint8_t tY = 0> class cLEDMatrix : public cLEDMatrixBase {
private:
  static const int m_absWidth  = (matrixW * (tX >0?tX:1));
  static const int m_absHeight = (matrixH * (tY >0?tY:1));
  struct CRGB p_LED[(m_absWidth * m_absHeight)];
public:
  cLEDMatrix()
    : cLEDMatrixBase(m_absWidth, m_absHeight) {
      type = matrixType;
      matrixWidth = matrixW;
      matrixHeight = matrixH;
      tilesX = tX;
      tilesY = tY;
      remapFn = NULL;
      m_LED = p_LED;
  }
  void SetLEDArray(struct CRGB *pLED) {
    m_LED = pLED;
  }

  void ShiftLeft(void) {
    /* t.b.d.
    switch (tMType) {
      case HORIZONTAL_MATRIX:
        if (tWidth > 0)
          HPWSL();
        else
          HNWSL();
        break;
      case VERTICAL_MATRIX:
        if (tWidth > 0)
          VPWSL();
        else
          VNWSL();
        break;
      case HORIZONTAL_ZIGZAG_MATRIX:
        if (tWidth > 0)
          HZPWSL();
        else
          HZNWSL();
        break;
      case VERTICAL_ZIGZAG_MATRIX:
        if (tWidth > 0)
          VZPWSL();
        else
          VZNWSL();
        break;
    }
    */
  }

  void ShiftRight(void) {
    /*
    switch (tMType) {
      case HORIZONTAL_MATRIX:
        if (tWidth > 0)
          HNWSL();
        else
          HPWSL();
        break;
      case VERTICAL_MATRIX:
        if (tWidth > 0)
          VNWSL();
        else
          VPWSL();
        break;
      case HORIZONTAL_ZIGZAG_MATRIX:
        if (tWidth > 0)
          HZNWSL();
        else
          HZPWSL();
        break;
      case VERTICAL_ZIGZAG_MATRIX:
        if (tWidth > 0)
          VZNWSL();
        else
          VZPWSL();
        break;
    }
    */
  }

  void ShiftDown(void) {
    /*
    switch (tMType) {
      case HORIZONTAL_MATRIX:
        if (tHeight > 0)
          HPHSD();
        else
          HNHSD();
        break;
      case VERTICAL_MATRIX:
        if (tHeight > 0)
          VPHSD();
        else
          VNHSD();
        break;
      case HORIZONTAL_ZIGZAG_MATRIX:
        if (tHeight > 0)
          HZPHSD();
        else
          HZNHSD();
        break;
      case VERTICAL_ZIGZAG_MATRIX:
        if (tHeight > 0)
          VZPHSD();
        else
          VZNHSD();
        break;
    }
    */
  }

  void ShiftUp(void) {
    /*
    switch (tMType)
    {
      case HORIZONTAL_MATRIX:
      	if (tHeight > 0)
          HNHSD();
        else
          HPHSD();
        break;
      case VERTICAL_MATRIX:
        if (tHeight > 0)
          VNHSD();
        else
          VPHSD();
        break;
      case HORIZONTAL_ZIGZAG_MATRIX:
        if (tHeight > 0)
          HZNHSD();
        else
          HZPHSD();
        break;
      case VERTICAL_ZIGZAG_MATRIX:
        if (tHeight > 0)
          VZNHSD();
        else
          VZPHSD();
        break;
    }
    */
  }

private:
	// Functions used by ShiftLeft & ShiftRight
  void HPWSL(void) {
    int16_t i = 0;
    for (int16_t y=m_absHeight; y>0; --y,++i) {
      for (int16_t x=m_absWidth-1; x>0; --x,++i)
        p_LED[i] = p_LED[i + 1];
      p_LED[i] = CRGB(0, 0, 0);
    }
  }
  void HNWSL(void) {
    int16_t i = m_absWidth - 1;
    for (int16_t y=m_absHeight; y>0; --y) {
      for (int16_t x=m_absWidth-1; x>0; --x,--i)
        p_LED[i] = p_LED[i - 1];
      p_LED[i] = CRGB(0, 0, 0);
      i += ((m_absWidth * 2) - 1);
    }
  }
  void VPWSL(void) {
    int16_t i = 0;
    int16_t j = m_absHeight;
    for (int16_t x=m_absWidth-1; x>0; --x) {
      for (int16_t y=m_absHeight; y>0; --y)
        p_LED[i++] = p_LED[j++];
    }
    for (int16_t y=m_absHeight; y>0; --y)
      p_LED[i++] = CRGB(0, 0, 0);
  }
  void VNWSL(void) {
    int16_t i = (m_absHeight * m_absWidth) - 1;
    int16_t j = i - m_absHeight;
    for (int16_t x=m_absWidth-1; x>0; --x) {
      for (int16_t y=m_absHeight; y>0; --y)
        p_LED[i--] = p_LED[j--];
    }
    for (int16_t y=m_absHeight; y>0; --y)
      p_LED[i--] = CRGB(0, 0, 0);
  }
  void HZPWSL(void) {
    int16_t i = 0;
    for (int16_t y=m_absHeight; y>0; y-=2) {
      for (int16_t x=m_absWidth-1; x>0; --x,++i)
        p_LED[i] = p_LED[i + 1];
      p_LED[i] = CRGB(0, 0, 0);
      i++;
      if (y > 1) {
        i += (m_absWidth - 1);
        for (int16_t x=m_absWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += m_absWidth;
      }
    }
  }
  void HZNWSL(void) {
    int16_t i = m_absWidth - 1;
    for (int16_t y=m_absHeight; y>0; y-=2) {
      for (int16_t x=m_absWidth-1; x>0; --x,--i)
        p_LED[i] = p_LED[i - 1];
      p_LED[i] = CRGB(0, 0, 0);
      if (y > 1) {
        i += m_absWidth;
        for (int16_t x=m_absWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += m_absWidth;
      }
    }
  }
  void VZPWSL(void) {
    int16_t i = 0;
    int16_t j = (m_absHeight * 2) - 1;
    for (int16_t x=m_absWidth-1; x>0; x-=2) {
      for (int16_t y=m_absHeight; y>0; --y)
        p_LED[i++] = p_LED[j--];
      if (x > 1) {
        j += (m_absHeight * 2);
        for (int16_t y=m_absHeight; y>0; --y)
          p_LED[i++] = p_LED[j--];
        j += (m_absHeight * 2);
      }
    }
    for (int16_t y=m_absHeight; y>0; y--)
      p_LED[i++] = CRGB(0, 0, 0);
  }
  void VZNWSL(void) {
    int16_t i = (m_absHeight * m_absWidth) - 1;
    int16_t j = m_absHeight * (m_absWidth - 2);
    for (int16_t x=m_absWidth-1; x>0; x-=2) {
      for (int16_t y=m_absHeight; y>0; --y)
        p_LED[i--] = p_LED[j++];
      if (x > 1) {
        j -= (m_absHeight * 2);
        for (int16_t y=m_absHeight; y>0; --y)
          p_LED[i--] = p_LED[j++];
        j -= (m_absHeight * 2);
      }
    }
    for (int16_t y=m_absHeight; y>0; y--)
      p_LED[i--] = CRGB(0, 0, 0);
  }

	// Functions used by ShiftDown & ShiftUp
  void HPHSD(void) {
    int16_t i = 0;
    int16_t j = m_absWidth;
    for (int16_t y=m_absHeight-1; y>0; --y) {
      for (int16_t x=m_absWidth; x>0; --x)
        p_LED[i++] = p_LED[j++];
    }
    for (int16_t x=m_absWidth; x>0; --x)
      p_LED[i++] = CRGB(0, 0, 0);
  }
  void HNHSD(void) {
    int16_t i = (m_absWidth * m_absHeight) - 1;
    int16_t j = i - m_absWidth;
    for (int16_t y=m_absHeight-1; y>0; --y) {
      for (int16_t x=m_absWidth; x>0; --x)
        p_LED[i--] = p_LED[j--];
    }
    for (int16_t x=m_absWidth; x>0; --x)
      p_LED[i--] = CRGB(0, 0, 0);
  }
  void VPHSD(void) {
    int16_t i = 0;
    for (int16_t x=m_absWidth; x>0; --x,++i) {
      for (int16_t y=m_absHeight-1; y>0; --y,++i)
        p_LED[i] = p_LED[i + 1];
      p_LED[i] = CRGB(0, 0, 0);
    }
  }
  void VNHSD(void) {
    int16_t i = m_absHeight - 1;
    for (int16_t x=m_absWidth; x>0; --x) {
      for (int16_t y=m_absHeight-1; y>0; --y,--i)
        p_LED[i] = p_LED[i - 1];
      p_LED[i] = CRGB(0, 0, 0);
      i += ((m_absHeight * 2) - 1);
    }
  }
  void HZPHSD(void) {
    int16_t i = 0;
    int16_t j = (m_absWidth * 2) - 1;
    for (int16_t y=m_absHeight-1; y>0; y-=2) {
      for (int16_t x=m_absWidth; x>0; --x)
        p_LED[i++] = p_LED[j--];
      if (y > 1) {
        j += (m_absWidth * 2);
        for (int16_t x=m_absWidth; x>0; --x)
          p_LED[i++] = p_LED[j--];
        j += (m_absWidth * 2);
      }
    }
    for (int16_t x=m_absWidth; x>0; x--)
      p_LED[i++] = CRGB(0, 0, 0);
  }
  void HZNHSD(void) {
    int16_t i = (m_absWidth * m_absHeight) - 1;
    int16_t j = m_absWidth * (m_absHeight - 2);
    for (int16_t y=m_absHeight-1; y>0; y-=2) {
      for (int16_t x=m_absWidth; x>0; --x)
        p_LED[i--] = p_LED[j++];
      if (y > 1) {
        j -= (m_absWidth * 2);
        for (int16_t x=m_absWidth; x>0; --x)
          p_LED[i--] = p_LED[j++];
        j -= (m_absWidth * 2);
      }
    }
    for (int16_t x=m_absWidth; x>0; x--)
      p_LED[i--] = CRGB(0, 0, 0);
  }
  void VZPHSD(void) {
    int16_t i = 0;
    for (int16_t x=m_absWidth; x>0; x-=2) {
      for (int16_t y=m_absHeight-1; y>0; --y,++i)
        p_LED[i] = p_LED[i + 1];
      p_LED[i] = CRGB(0, 0, 0);
      i++;
      if (x > 1) {
        i += (m_absHeight - 1);
        for (int16_t y=m_absHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += m_absHeight;
      }
    }
  }
  void VZNHSD(void) {
    int16_t i = m_absHeight - 1;
    for (int16_t x=m_absWidth; x>0; x-=2) {
      for (int16_t y=m_absHeight-1; y>0; --y,--i)
        p_LED[i] = p_LED[i - 1];
      p_LED[i] = CRGB(0, 0, 0);
      if (x > 1) {
        i += m_absHeight;
        for (int16_t y=m_absHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += m_absHeight;
      }
    }
  }
};

#endif // _LEDMATRIX_H_
