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
#include <FastLED.h>
#include <LEDMatrix.h>

#ifdef __AVR__
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#else
 #ifndef pgm_read_byte
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #endif
#endif

#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif

cLEDMatrixBase::cLEDMatrixBase(int w, int h)
  : FastLED_GFX(w, h) { }

struct CRGB* cLEDMatrixBase::operator[](int n) {
  return(&m_LED[n]);
}

struct CRGB& cLEDMatrixBase::operator()(int16_t x, int16_t y) {
  if ( (x >= 0) && (x < _width) && (y >= 0) && (y < _height))
    return(m_LED[mXY(x, y)]);
  else
    return(m_OutOfBounds);
}

struct CRGB& cLEDMatrixBase::operator()(int16_t i) {
  if ((i >=0) && (i < (_width * _height)))
    return(m_LED[i]);
  else
    return(m_OutOfBounds);
}

uint16_t cLEDMatrixBase::mXY(uint16_t x, uint16_t y) {
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return 0;

  int16_t t;
  switch(rotation) {
   case 1:
    t = x;
    x = WIDTH  - 1 - y;
    y = t;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    t = x;
    x = y;
    y = HEIGHT - 1 - t;
    break;
  }

  int tileOffset = 0, pixelOffset;

  if(remapFn) { // Custom X/Y remapping function
    pixelOffset = (*remapFn)(x, y);
  } else {      // Standard single matrix or tiled matrices
    uint8_t  corner = type & MTX_MATRIX_CORNER;
    uint16_t minor, major, majorScale;

    if(tilesX) { // Tiled display, multiple matrices
      uint16_t tile;

      minor = x / matrixWidth;            // Tile # X/Y; presume row major to
      major = y / matrixHeight,           // start (will swap later if needed)
      x     = x - (minor * matrixWidth);  // Pixel X/Y within tile
      y     = y - (major * matrixHeight); // (-* is less math than modulo)

      // Determine corner of entry, flip axes if needed
      if(type & MTX_TILE_RIGHT)  minor = tilesX - 1 - minor;
      if(type & MTX_TILE_BOTTOM) major = tilesY - 1 - major;

      // Determine actual major axis of tiling
      if((type & MTX_TILE_AXIS) == MTX_TILE_ROWS) {
        majorScale = tilesX;
      } else {
        _swap_uint16_t(major, minor);
        majorScale = tilesY;
      }

      // Determine tile number
      if((type & MTX_TILE_SEQUENCE) == MTX_TILE_PROGRESSIVE) {
        // All tiles in same order
        tile = major * majorScale + minor;
      } else {
        // Zigzag; alternate rows change direction.  On these rows,
        // this also flips the starting corner of the matrix for the
        // pixel math later.
        if(major & 1) {
          corner ^= MTX_MATRIX_CORNER;
          tile = (major + 1) * majorScale - 1 - minor;
        } else {
          tile =  major      * majorScale     + minor;
        }
      }
      // Index of first pixel in tile
      tileOffset = tile * matrixWidth * matrixHeight;
    } // else no tiling (handle as single tile)

    // Find pixel number within tile
    minor = x; // Presume row major to start (will swap later if needed)
    major = y;

    // Determine corner of entry, flip axes if needed
    if(corner & MTX_MATRIX_RIGHT)  minor = matrixWidth  - 1 - minor;
    if(corner & MTX_MATRIX_BOTTOM) major = matrixHeight - 1 - major;

    // Determine actual major axis of matrix
    if((type & MTX_MATRIX_AXIS) == MTX_MATRIX_ROWS) {
      majorScale = matrixWidth;
    } else {
      _swap_uint16_t(major, minor);
      majorScale = matrixHeight;
    }

    // Determine pixel number within tile/matrix
    if((type & MTX_MATRIX_SEQUENCE) == MTX_MATRIX_PROGRESSIVE) {
      // All lines in same order
      pixelOffset = major * majorScale + minor;
    } else {
      // Zigzag; alternate rows change direction.
      if(major & 1) pixelOffset = (major + 1) * majorScale - 1 - minor;
      else          pixelOffset =  major      * majorScale     + minor;
    }
  }
  return (tileOffset + pixelOffset);
}

void cLEDMatrixBase::HorizontalMirror(bool FullHeight) {
  int ty, y, x, xx;
  if (FullHeight)
    ty = _height - 1;
  else
    ty = (_height / 2);
  for (y=ty; y>=0; --y) {
    for (x=(_width/2)-1,xx=((_width+1)/2); x>=0; --x,++xx)
      m_LED[mXY(xx, y)] = m_LED[mXY(x, y)];
  }
}

void cLEDMatrixBase::VerticalMirror() {
  int y, yy, x;
  for (y=(_height/2)-1,yy=((_height+1)/2); y>=0; --y,++yy) {
    for (x=_width-1; x>=0; --x)
      m_LED[mXY(x, yy)] = m_LED[mXY(x, y)];
  }
}

void cLEDMatrixBase::QuadrantMirror() {
  HorizontalMirror(false);
  VerticalMirror();
}

void cLEDMatrixBase::QuadrantRotateMirror() {
  int MaxXY, MidXY, x, y, src;
  if (_width > _height)
    MaxXY = _height;
  else
    MaxXY = _width;
  MidXY = (MaxXY / 2);
  MaxXY--;
  for (x=MidXY-(MaxXY%2); x>=0; --x) {
    for (y=MidXY-(MaxXY%2); y>=0; --y) {
      src = mXY(x, y);
      m_LED[mXY(MidXY + y, MidXY - (MaxXY % 2) - x)] = m_LED[src];
      m_LED[mXY(MaxXY - x, MaxXY - y)] = m_LED[src];
      m_LED[mXY(MidXY - (MaxXY % 2) - y, MidXY + x)] = m_LED[src];
    }
  }
}

void cLEDMatrixBase::TriangleTopMirror(bool FullHeight) {
  int MaxXY, x, y;
  if (_width > _height)
    MaxXY = _height - 1;
  else
    MaxXY = _width - 1;
  if (! FullHeight)
    MaxXY /= 2;
  for (y=1; y<=MaxXY; ++y) {
    for (x=0; x<y; ++x)
      m_LED[mXY(y,x)] = m_LED[mXY(x,y)];
  }
}

void cLEDMatrixBase::TriangleBottomMirror(bool FullHeight) {
  int MaxXY, x, y, xx, yy;
  if (_width > _height)
    MaxXY = _height - 1;
  else
    MaxXY = _width - 1;
  if (! FullHeight)
    MaxXY /= 2;
  for (y=0,xx=MaxXY; y<MaxXY; y++,xx--) {
    for (x=MaxXY-y-1,yy=y+1; x>=0; --x,++yy)
      m_LED[mXY(xx, yy)] = m_LED[mXY(x, y)];
  }
}

void cLEDMatrixBase::QuadrantTopTriangleMirror() {
  TriangleTopMirror(false);
  QuadrantMirror();
}

void cLEDMatrixBase::QuadrantBottomTriangleMirror() {
  TriangleBottomMirror(false);
  QuadrantMirror();
}

void cLEDMatrixBase::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color) {
  drawLine(x0, y0, x1, y1, color);
}

void cLEDMatrixBase::DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color) {
  drawRect(x0, y0, (x1-x0), (y1-y0), color);
}

void cLEDMatrixBase::DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color) {
  drawCircle(xc, yc, r, color);
}

void cLEDMatrixBase::DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color) {
  fillRect(x0, y0, (x1-x0), (y1-y0), color);
}

void cLEDMatrixBase::DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB color) {
  fillCircle(xc, yc, r, color);
}

void cLEDMatrixBase::drawPixel(int n, CRGB color) {
  (*this)(n) = color;
}

void cLEDMatrixBase::drawPixel(int16_t x, int16_t y, CRGB color) {
  (*this)(x,y) = color;
}

struct CRGB& cLEDMatrixBase::pixel(int n) {
  return (*this)(n);
}

struct CRGB& cLEDMatrixBase::pixel(int16_t x, int16_t y) {
  return (*this)(x, y);
}

void cLEDMatrixBase::fillScreen(CRGB color) {
  uint16_t i, n;
  uint32_t c;

  n = Size();
  for(i=0; i<n; i++) (*this)(i) = c;
}

void cLEDMatrixBase::setRemapFunction(uint16_t (*fn)(uint16_t, uint16_t)) {
  remapFn = fn;
}
