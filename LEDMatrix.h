/* 
  LEDMatrix V5 class by Aaron Liddiment (c) 2016
  modified:  Juergen Skrotzky (JorgenVikingGod@gmail.com)
  date:      2016/04/27
*/

#ifndef LEDMatrix_h
#define LEDMatrix_h

enum MatrixType_t { HORIZONTAL_MATRIX,
                    VERTICAL_MATRIX,
                    HORIZONTAL_ZIGZAG_MATRIX,
                    VERTICAL_ZIGZAG_MATRIX };

enum BlockType_t	{	HORIZONTAL_BLOCKS,
										VERTICAL_BLOCKS,
										HORIZONTAL_ZIGZAG_BLOCKS,
										VERTICAL_ZIGZAG_BLOCKS };

class cLEDMatrixBase
{
  friend class cSprite;

  protected:
    int16_t m_Width, m_Height;
    struct CRGB *m_LED;
    struct CRGB m_OutOfBounds;

  public:
    cLEDMatrixBase();
    virtual uint32_t mXY(uint16_t x, uint16_t y)=0;
    void SetLEDArray(struct CRGB *pLED);	// Only used with externally defined LED arrays

    struct CRGB *operator[](int n);
    struct CRGB &operator()(int16_t x, int16_t y);
    struct CRGB &operator()(int16_t i);

    int Size()  { return(m_Width * m_Height); }
    int Width() { return(m_Width);  }
    int Height()  { return(m_Height); }

    void HorizontalMirror(bool FullHeight = true);
    void VerticalMirror();
    void QuadrantMirror();
    void QuadrantRotateMirror();
    void TriangleTopMirror(bool FullHeight = true);
    void TriangleBottomMirror(bool FullHeight = true);
    void QuadrantTopTriangleMirror();
    void QuadrantBottomTriangleMirror();

    void DrawPixel(int16_t x, int16_t y, CRGB Col);
    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
    void DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
};

template<int16_t tMWidth, int16_t tMHeight, MatrixType_t tMType, int8_t tBWidth = 1, int8_t tBHeight = 1, BlockType_t tBType = HORIZONTAL_BLOCKS> class cLEDMatrix : public cLEDMatrixBase
{
  private:
    static const int16_t m_absMWidth = (tMWidth * ((tMWidth < 0) * -1 + (tMWidth > 0)));
    static const int16_t m_absMHeight = (tMHeight * ((tMHeight < 0) * -1 + (tMHeight > 0)));
    static const int16_t m_absBWidth = (tBWidth * ((tBWidth < 0) * -1 + (tBWidth > 0)));
    static const int16_t m_absBHeight = (tBHeight * ((tBHeight < 0) * -1 + (tBHeight > 0)));
    struct CRGB *p_LED;

  public:
    cLEDMatrix(bool doMalloc=true)
    {
      m_Width = m_absMWidth * m_absBWidth;
      m_Height = m_absMHeight * m_absBHeight;
      if (doMalloc) {
	  // On ESP32, there is more memory available via malloc than static global arrays
          p_LED = (struct CRGB *) malloc(m_absMWidth * m_absBWidth * m_absMHeight * m_absBHeight * sizeof(CRGB));
          m_LED = p_LED;
	  if (! p_LED) {
	     Serial.begin(115200);
	     Serial.println("Malloc LEDMatrix Failed");
	     while (1);
	  }
      } else {
	  Serial.println("LED array not intialized, must be set by SetLEDArray");
      }
    }
    void SetLEDArray(struct CRGB *pLED)
    {
      p_LED = pLED;
      m_LED = pLED;
    }
    virtual uint32_t mXY(uint16_t x, uint16_t y)
    {
			if ((tBWidth == 1) && (tBHeight == 1))
			{
				// No Blocks, just a Matrix
	      if (tMWidth < 0)
	        x = (m_absMWidth - 1) - x;
		    if (tMHeight < 0)
	        y = (m_absMHeight - 1) - y;
	      if (tMType == HORIZONTAL_MATRIX)
	        return((y * m_absMWidth) + x);
	      else if (tMType == VERTICAL_MATRIX)
	        return((x * m_absMHeight) + y);
	      else if (tMType == HORIZONTAL_ZIGZAG_MATRIX)
	      {
	        if (y % 2)
	          return((((y + 1) * m_absMWidth) - 1) - x);
	        else
	          return((y * m_absMWidth) + x);
	      }
	      else /* if (tMType == VERTICAL_ZIGZAG_MATRIX) */
	      {
	        if (x % 2)
	          return((((x + 1) * m_absMHeight) - 1) - y);
	        else
	          return((x * m_absMHeight) + y);
	      }
			}
			else
			{
				// Reverse Block/Matrix X coordinate if needed
				if ((tBWidth < 0) && (tMWidth < 0))
					x = (((m_absBWidth - 1) - (x / m_absMWidth)) * m_absMWidth) + ((m_absMWidth - 1) - (x % m_absMWidth));
				else if (tBWidth < 0)
					x = (((m_absBWidth - 1) - (x / m_absMWidth)) * m_absMWidth) + (x % m_absMWidth);
				else if (tMWidth < 0)
					x = x - ((x % m_absMWidth) * 2) + (m_absMWidth - 1);
				// Reverse Block/Matrix Y coordinate if needed
				if ((tBHeight < 0) && (tMHeight < 0))
					y = (((m_absBHeight - 1) - (y / m_absMHeight)) * m_absMHeight) + ((m_absMHeight - 1) - (y % m_absMHeight));
				else if(tBHeight < 0)
					y = (((m_absBHeight - 1) - (y / m_absMHeight)) * m_absMHeight) + (y % m_absMHeight);
				else if (tMHeight < 0)
					y = y - ((y % m_absMHeight) * 2) + (m_absMHeight - 1);
				// Calculate Block base
	    	uint16_t Base;
		    if (tBType == HORIZONTAL_BLOCKS)
		    	Base = (((y / m_absMHeight) * m_absBWidth) + (x / m_absMWidth)) * (m_absMWidth * m_absMHeight);
		    else if (tBType == VERTICAL_BLOCKS)
		    	Base = (((x / m_absMWidth) * m_absBHeight) + (y / m_absMHeight)) * (m_absMHeight * m_absMWidth);
		    else if (tBType == HORIZONTAL_ZIGZAG_BLOCKS)
		    {
	        if ((y / m_absMHeight) % 2)
			    	Base = (((y / m_absMHeight) * m_absBWidth) + ((m_absBWidth - 1) - (x / m_absMWidth))) * (m_absMWidth * m_absMHeight);
	       	else
			    	Base = (((y / m_absMHeight) * m_absBWidth) + (x / m_absMWidth)) * (m_absMWidth * m_absMHeight);
	      }
	      else /* if (tBType == VERTICAL_ZIGZAG_BLOCKS) */
	      {
	        if ((x / m_absMWidth) % 2)
			    	Base = (((x / m_absMWidth) * m_absBHeight) + ((m_absBHeight - 1) - (y / m_absMHeight))) * (m_absMHeight * m_absMWidth);
	        else
			    	Base = (((x / m_absMWidth) * m_absBHeight) + (y / m_absMHeight)) * (m_absMHeight * m_absMWidth);
	      }
				// Calculate Matrix offset
	      if (tMType == HORIZONTAL_MATRIX)
	        return(Base + ((y % m_absMHeight) * m_absMWidth) + (x % m_absMWidth));
	      else if (tMType == VERTICAL_MATRIX)
	        return(Base + ((x % m_absMWidth) * m_absMHeight) + (y % m_absMHeight));
	      else if (tMType == HORIZONTAL_ZIGZAG_MATRIX)
	      {
	        if ((y % m_absMHeight) % 2)
	          return(Base + ((((y % m_absMHeight) + 1) * m_absMWidth) - 1) - (x % m_absMWidth));
	        else
	          return(Base + ((y % m_absMHeight) * m_absMWidth) + (x % m_absMWidth));
	      }
	      else /* if (tMType == VERTICAL_ZIGZAG_MATRIX) */
	      {
	        if ((x % m_absMWidth) % 2)
	          return(Base + ((((x % m_absMWidth) + 1) * m_absMHeight) - 1) - (y % m_absMHeight));
	        else
	          return(Base + ((x % m_absMWidth) * m_absMHeight) + (y % m_absMHeight));
	      }
	    }
    }

    void ShiftLeft(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
		    for (int16_t x=1; x<m_Width; ++x)
  			{
				  for (int16_t y=0; y<m_Height; ++y)
      			m_LED[mXY(x - 1, y)] = m_LED[mXY(x, y)];
			  }
			  for (int16_t y=0; y<m_Height; ++y)
     			m_LED[mXY(m_Width - 1, y)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
            if (tMWidth > 0)
              HPWSL();
            else
              HNWSL();
            break;
          case VERTICAL_MATRIX:
            if (tMWidth > 0)
              VPWSL();
            else
              VNWSL();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              HZPWSL();
            else
              HZNWSL();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              VZPWSL();
            else
              VZNWSL();
            break;
        }
      }
    }

    void ShiftRight(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
		    for (int16_t x=m_Width-1; x>=1; --x)
  			{
				  for (int16_t y=0; y<m_Height; ++y)
      			m_LED[mXY(x, y)] = m_LED[mXY(x - 1, y)];
			  }
			  for (int16_t y=0; y<m_Height; ++y)
     			m_LED[mXY(0, y)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
            if (tMWidth > 0)
              HNWSL();
            else
              HPWSL();
            break;
          case VERTICAL_MATRIX:
            if (tMWidth > 0)
              VNWSL();
            else
              VPWSL();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              HZNWSL();
            else
              HZPWSL();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMWidth > 0)
              VZNWSL();
            else
              VZPWSL();
            break;
        }
      }
    }

    void ShiftDown(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
			  for (int16_t y=1; y<m_Height; ++y)
  			{
			    for (int16_t x=0; x<m_Width; ++x)
      			m_LED[mXY(x, y - 1)] = m_LED[mXY(x, y)];
			  }
		    for (int16_t x=0; x<m_Width; ++x)
     			m_LED[mXY(x, m_Height - 1)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
            if (tMHeight > 0)
              HPHSD();
            else
              HNHSD();
            break;
          case VERTICAL_MATRIX:
            if (tMHeight > 0)
              VPHSD();
            else
              VNHSD();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              HZPHSD();
            else
              HZNHSD();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              VZPHSD();
            else
              VZNHSD();
            break;
        }
      }
    }

    void ShiftUp(void)
    {
      if ((tBWidth != 1) || (tBHeight != 1))
     	{
				// Blocks, so no optimisation
			  for (int16_t y=m_Height-1; y>=1; --y)
  			{
			    for (int16_t x=0; x<m_Width; ++x)
      			m_LED[mXY(x, y)] = m_LED[mXY(x, y - 1)];
			  }
		    for (int16_t x=0; x<m_Width; ++x)
     			m_LED[mXY(x, 0)] = CRGB(0, 0, 0);
     	}
     	else
      {
				// No Blocks, just a Matrix so optimise a little
        switch (tMType)
        {
          case HORIZONTAL_MATRIX:
          	if (tMHeight > 0)
              HNHSD();
            else
              HPHSD();
            break;
          case VERTICAL_MATRIX:
            if (tMHeight > 0)
              VNHSD();
            else
              VPHSD();
            break;
          case HORIZONTAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              HZNHSD();
            else
              HZPHSD();
            break;
          case VERTICAL_ZIGZAG_MATRIX:
            if (tMHeight > 0)
              VZNHSD();
            else
              VZPHSD();
            break;
        }
      }
    }

  private:
  	// Optimised functions used by ShiftLeft & ShiftRight in non block mode
    void HPWSL(void)
    {
      uint32_t i = 0;
      for (int16_t y=m_absMHeight; y>0; --y,++i)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
      }
    }
    void HNWSL(void)
    {
      uint32_t i = m_absMWidth - 1;
      for (int16_t y=m_absMHeight; y>0; --y)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += ((m_absMWidth * 2) - 1);
      }
    }
    void VPWSL(void)
    {
      uint32_t i = 0;
      uint32_t j = m_absMHeight;
      for (uint16_t x=m_absMWidth-1; x>0; --x)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i++] = p_LED[j++];
      }
      for (int16_t y=m_absMHeight; y>0; --y)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void VNWSL(void)
    {
      uint32_t i = (m_absMHeight * m_absMWidth) - 1;
      uint32_t j = i - m_absMHeight;
      for (uint16_t x=m_absMWidth-1; x>0; --x)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i--] = p_LED[j--];
      }
      for (int16_t y=m_absMHeight; y>0; --y)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void HZPWSL(void)
    {
      uint32_t i = 0;
      for (int16_t y=m_absMHeight; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i++;
        if (y > 1)
        {
          i += (m_absMWidth - 1);
          for (uint16_t x=m_absMWidth-1; x>0; --x,--i)
            p_LED[i] = p_LED[i - 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMWidth;
        }
      }
    }
    void HZNWSL(void)
    {
      uint32_t i = m_absMWidth - 1;
      for (int16_t y=m_absMHeight; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        if (y > 1)
        {
          i += m_absMWidth;
          for (uint16_t x=m_absMWidth-1; x>0; --x,++i)
            p_LED[i] = p_LED[i + 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMWidth;
        }
      }
    }
    void VZPWSL(void)
    {
      uint32_t i = 0;
      uint32_t j = (m_absMHeight * 2) - 1;
      for (uint16_t x=m_absMWidth-1; x>0; x-=2)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i++] = p_LED[j--];
        if (x > 1)
        {
          j += (m_absMHeight * 2);
          for (int16_t y=m_absMHeight; y>0; --y)
            p_LED[i++] = p_LED[j--];
          j += (m_absMHeight * 2);
        }
      }
      for (int16_t y=m_absMHeight; y>0; y--)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void VZNWSL(void)
    {
      uint32_t i = (m_absMHeight * m_absMWidth) - 1;
      uint32_t j = m_absMHeight * (m_absMWidth - 2);
      for (uint16_t x=m_absMWidth-1; x>0; x-=2)
      {
        for (int16_t y=m_absMHeight; y>0; --y)
          p_LED[i--] = p_LED[j++];
        if (x > 1)
        {
          j -= (m_absMHeight * 2);
          for (int16_t y=m_absMHeight; y>0; --y)
            p_LED[i--] = p_LED[j++];
          j -= (m_absMHeight * 2);
        }
      }
      for (int16_t y=m_absMHeight; y>0; y--)
        p_LED[i--] = CRGB(0, 0, 0);
    }

  	// Optimised functions used by ShiftDown & ShiftUp in non block mode
    void HPHSD(void)
    {
      uint32_t i = 0;
      uint32_t j = m_absMWidth;
      for (uint16_t y=m_absMHeight-1; y>0; --y)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i++] = p_LED[j++];
      }
      for (uint16_t x=m_absMWidth; x>0; --x)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void HNHSD(void)
    {
      uint32_t i = (m_absMWidth * m_absMHeight) - 1;
      uint32_t j = i - m_absMWidth;
      for (uint16_t y=m_absMHeight-1; y>0; --y)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i--] = p_LED[j--];
      }
      for (uint16_t x=m_absMWidth; x>0; --x)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void VPHSD(void)
    {
      uint32_t i = 0;
      for (uint16_t x=m_absMWidth; x>0; --x,++i)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
      }
    }
    void VNHSD(void)
    {
      uint32_t i = m_absMHeight - 1;
      for (uint16_t x=m_absMWidth; x>0; --x)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += ((m_absMHeight * 2) - 1);
      }
    }
    void HZPHSD(void)
    {
      uint32_t i = 0;
      uint32_t j = (m_absMWidth * 2) - 1;
      for (uint16_t y=m_absMHeight-1; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i++] = p_LED[j--];
        if (y > 1)
        {
          j += (m_absMWidth * 2);
          for (uint16_t x=m_absMWidth; x>0; --x)
            p_LED[i++] = p_LED[j--];
          j += (m_absMWidth * 2);
        }
      }
      for (uint16_t x=m_absMWidth; x>0; x--)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void HZNHSD(void)
    {
      uint32_t i = (m_absMWidth * m_absMHeight) - 1;
      uint32_t j = m_absMWidth * (m_absMHeight - 2);
      for (uint16_t y=m_absMHeight-1; y>0; y-=2)
      {
        for (uint16_t x=m_absMWidth; x>0; --x)
          p_LED[i--] = p_LED[j++];
        if (y > 1)
        {
          j -= (m_absMWidth * 2);
          for (uint16_t x=m_absMWidth; x>0; --x)
            p_LED[i--] = p_LED[j++];
          j -= (m_absMWidth * 2);
        }
      }
      for (uint16_t x=m_absMWidth; x>0; x--)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void VZPHSD(void)
    {
      uint32_t i = 0;
      for (uint16_t x=m_absMWidth; x>0; x-=2)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i++;
        if (x > 1)
        {
          i += (m_absMHeight - 1);
          for (uint16_t y=m_absMHeight-1; y>0; --y,--i)
            p_LED[i] = p_LED[i - 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMHeight;
        }
      }
    }
    void VZNHSD(void)
    {
      uint32_t i = m_absMHeight - 1;
      for (uint16_t x=m_absMWidth; x>0; x-=2)
      {
        for (uint16_t y=m_absMHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        if (x > 1)
        {
          i += m_absMHeight;
          for (uint16_t y=m_absMHeight-1; y>0; --y,++i)
            p_LED[i] = p_LED[i + 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absMHeight;
        }
      }
    }

};

#endif
