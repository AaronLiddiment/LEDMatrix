#ifndef LEDMatrix_h
#define LEDMatrix_h

enum MatrixType_t { HORIZONTAL_MATRIX,
                    VERTICAL_MATRIX,
                    HORIZONTAL_ZIGZAG_MATRIX,
                    VERTICAL_ZIGZAG_MATRIX };

class cLEDMatrixBase
{
  friend class cSprite;

  protected:
    int16_t m_Width, m_Height;
    MatrixType_t m_Type;
    struct CRGB *m_LED;
    struct CRGB m_OutOfBounds;

  public:
    cLEDMatrixBase();
    virtual uint16_t mXY(uint16_t x, uint16_t y)=0;
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

    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void DrawCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
    void DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB Col);
    void DrawFilledCircle(int16_t xc, int16_t yc, uint16_t r, CRGB Col);
};

template<int16_t tWidth, int16_t tHeight, MatrixType_t tMType, int16_t tXMult = 0, int16_t tYMult = 0> class cLEDMatrix : public cLEDMatrixBase
{
  private:
    static const int16_t m_absWidth = (tWidth * ((tWidth < 0) * -1 + (tWidth > 0)));
    static const int16_t m_absHeight = (tHeight * ((tHeight < 0) * -1 + (tHeight > 0)));
    struct CRGB p_LED[(m_absWidth * m_absHeight * ((tXMult == 0) && (tXMult == 0))) + ((tXMult != 0) && (tXMult != 0))]; // Will always be at least 1

  public:
    cLEDMatrix()
    {
      m_Width = m_absWidth;
      m_Height = m_absHeight;
      m_Type = tMType;
      if ((tXMult == 0) && (tYMult == 0))
        m_LED = p_LED;
      else
        m_LED = NULL;
    }
    void SetLEDArray(struct CRGB *pLED)
    {
      m_LED = pLED;
    }
    virtual uint16_t mXY(uint16_t x, uint16_t y)
    {
      if (tWidth < 0)
        x = (m_absWidth - 1) - x;
      if (tHeight < 0)
        y = (m_absHeight - 1) - y;
      if (tMType == HORIZONTAL_MATRIX)
      {
        if ((tXMult == 0) && (tYMult == 0))
          return((y * m_absWidth) + x);
        else
          return((y * m_absWidth * tYMult) + (x * tXMult));
      }
      else if (tMType == VERTICAL_MATRIX)
      {
        if ((tXMult == 0) && (tYMult == 0))
          return((x * m_absHeight) + y);
        else
          return((x * m_absHeight * tXMult) + (y * tYMult));
      }
      else if (tMType == HORIZONTAL_ZIGZAG_MATRIX)
      {
        if (y % 2)
        {
          if ((tXMult == 0) && (tYMult == 0))
            return((((y + 1) * m_absWidth) - 1) - x);
          else
            return((((y + 1) * m_absWidth * tYMult) - tXMult) - (x * tXMult));
        }
        else
        {
          if ((tXMult == 0) && (tYMult == 0))
            return((y * m_absWidth) + x);
          else
            return((y * m_absWidth * tYMult) + (x * tXMult));
        }
      }
      else /* if (tMType == VERTICAL_ZIGZAG_MATRIX) */
      {
        if (x % 2)
        {
          if ((tXMult == 0) && (tYMult == 0))
            return((((x + 1) * m_absHeight) - 1) - y);
          else
            return((((x + 1) * m_absHeight * tXMult) - tYMult) - (y * tYMult));
        }
        else
        {
          if ((tXMult == 0) && (tYMult == 0))
            return((x * m_absHeight) + y);
          else
            return((x * m_absHeight * tXMult) + (y * tYMult));
        }
      }
    }

    void ShiftLeft(void)
    {
      if ((tXMult == 0) && (tYMult == 0))
      {
        switch (tMType)
        {
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
      }
    }

    void ShiftRight(void)
    {
      if ((tXMult == 0) && (tYMult == 0))
      {
        switch (tMType)
        {
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
      }
    }

    void ShiftDown(void)
    {
      if ((tXMult == 0) && (tYMult == 0))
      {
        switch (tMType)
        {
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
      }
    }

    void ShiftUp(void)
    {
      if ((tXMult == 0) && (tYMult == 0))
      {
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
      }
    }

  private:
  	// Functions used by ShiftLeft & ShiftRight
    void HPWSL(void)
    {
      int16_t i = 0;
      for (int16_t y=m_absHeight; y>0; --y,++i)
      {
        for (int16_t x=m_absWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
      }
    }
    void HNWSL(void)
    {
      int16_t i = m_absWidth - 1;
      for (int16_t y=m_absHeight; y>0; --y)
      {
        for (int16_t x=m_absWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += ((m_absWidth * 2) - 1);
      }
    }
    void VPWSL(void)
    {
      int16_t i = 0;
      int16_t j = m_absHeight;
      for (int16_t x=m_absWidth-1; x>0; --x)
      {
        for (int16_t y=m_absHeight; y>0; --y)
          p_LED[i++] = p_LED[j++];
      }
      for (int16_t y=m_absHeight; y>0; --y)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void VNWSL(void)
    {
      int16_t i = (m_absHeight * m_absWidth) - 1;
      int16_t j = i - m_absHeight;
      for (int16_t x=m_absWidth-1; x>0; --x)
      {
        for (int16_t y=m_absHeight; y>0; --y)
          p_LED[i--] = p_LED[j--];
      }
      for (int16_t y=m_absHeight; y>0; --y)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void HZPWSL(void)
    {
      int16_t i = 0;
      for (int16_t y=m_absHeight; y>0; y-=2)
      {
        for (int16_t x=m_absWidth-1; x>0; --x,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i++;
        if (y > 1)
        {
          i += (m_absWidth - 1);
          for (int16_t x=m_absWidth-1; x>0; --x,--i)
            p_LED[i] = p_LED[i - 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absWidth;
        }
      }
    }
    void HZNWSL(void)
    {
      int16_t i = m_absWidth - 1;
      for (int16_t y=m_absHeight; y>0; y-=2)
      {
        for (int16_t x=m_absWidth-1; x>0; --x,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        if (y > 1)
        {
          i += m_absWidth;
          for (int16_t x=m_absWidth-1; x>0; --x,++i)
            p_LED[i] = p_LED[i + 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absWidth;
        }
      }
    }
    void VZPWSL(void)
    {
      int16_t i = 0;
      int16_t j = (m_absHeight * 2) - 1;
      for (int16_t x=m_absWidth-1; x>0; x-=2)
      {
        for (int16_t y=m_absHeight; y>0; --y)
          p_LED[i++] = p_LED[j--];
        if (x > 1)
        {
          j += (m_absHeight * 2);
          for (int16_t y=m_absHeight; y>0; --y)
            p_LED[i++] = p_LED[j--];
          j += (m_absHeight * 2);
        }
      }
      for (int16_t y=m_absHeight; y>0; y--)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void VZNWSL(void)
    {
      int16_t i = (m_absHeight * m_absWidth) - 1;
      int16_t j = m_absHeight * (m_absWidth - 2);
      for (int16_t x=m_absWidth-1; x>0; x-=2)
      {
        for (int16_t y=m_absHeight; y>0; --y)
          p_LED[i--] = p_LED[j++];
        if (x > 1)
        {
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
    void HPHSD(void)
    {
      int16_t i = 0;
      int16_t j = m_absWidth;
      for (int16_t y=m_absHeight-1; y>0; --y)
      {
        for (int16_t x=m_absWidth; x>0; --x)
          p_LED[i++] = p_LED[j++];
      }
      for (int16_t x=m_absWidth; x>0; --x)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void HNHSD(void)
    {
      int16_t i = (m_absWidth * m_absHeight) - 1;
      int16_t j = i - m_absWidth;
      for (int16_t y=m_absHeight-1; y>0; --y)
      {
        for (int16_t x=m_absWidth; x>0; --x)
          p_LED[i--] = p_LED[j--];
      }
      for (int16_t x=m_absWidth; x>0; --x)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void VPHSD(void)
    {
      int16_t i = 0;
      for (int16_t x=m_absWidth; x>0; --x,++i)
      {
        for (int16_t y=m_absHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
      }
    }
    void VNHSD(void)
    {
      int16_t i = m_absHeight - 1;
      for (int16_t x=m_absWidth; x>0; --x)
      {
        for (int16_t y=m_absHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        i += ((m_absHeight * 2) - 1);
      }
    }
    void HZPHSD(void)
    {
      int16_t i = 0;
      int16_t j = (m_absWidth * 2) - 1;
      for (int16_t y=m_absHeight-1; y>0; y-=2)
      {
        for (int16_t x=m_absWidth; x>0; --x)
          p_LED[i++] = p_LED[j--];
        if (y > 1)
        {
          j += (m_absWidth * 2);
          for (int16_t x=m_absWidth; x>0; --x)
            p_LED[i++] = p_LED[j--];
          j += (m_absWidth * 2);
        }
      }
      for (int16_t x=m_absWidth; x>0; x--)
        p_LED[i++] = CRGB(0, 0, 0);
    }
    void HZNHSD(void)
    {
      int16_t i = (m_absWidth * m_absHeight) - 1;
      int16_t j = m_absWidth * (m_absHeight - 2);
      for (int16_t y=m_absHeight-1; y>0; y-=2)
      {
        for (int16_t x=m_absWidth; x>0; --x)
          p_LED[i--] = p_LED[j++];
        if (y > 1)
        {
          j -= (m_absWidth * 2);
          for (int16_t x=m_absWidth; x>0; --x)
            p_LED[i--] = p_LED[j++];
          j -= (m_absWidth * 2);
        }
      }
      for (int16_t x=m_absWidth; x>0; x--)
        p_LED[i--] = CRGB(0, 0, 0);
    }
    void VZPHSD(void)
    {
      int16_t i = 0;
      for (int16_t x=m_absWidth; x>0; x-=2)
      {
        for (int16_t y=m_absHeight-1; y>0; --y,++i)
          p_LED[i] = p_LED[i + 1];
        p_LED[i] = CRGB(0, 0, 0);
        i++;
        if (x > 1)
        {
          i += (m_absHeight - 1);
          for (int16_t y=m_absHeight-1; y>0; --y,--i)
            p_LED[i] = p_LED[i - 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absHeight;
        }
      }
    }
    void VZNHSD(void)
    {
      int16_t i = m_absHeight - 1;
      for (int16_t x=m_absWidth; x>0; x-=2)
      {
        for (int16_t y=m_absHeight-1; y>0; --y,--i)
          p_LED[i] = p_LED[i - 1];
        p_LED[i] = CRGB(0, 0, 0);
        if (x > 1)
        {
          i += m_absHeight;
          for (int16_t y=m_absHeight-1; y>0; --y,++i)
            p_LED[i] = p_LED[i + 1];
          p_LED[i] = CRGB(0, 0, 0);
          i += m_absHeight;
        }
      }
    }

};

#endif
