#ifndef _GGFX_H
#define _GGFX_H

#include "main.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

extern uint16_t _width, _height, _cursor_y, _cursor_x, _textcolor, _textbgcolor;
extern uint8_t _rotation, _textsize, _wrap;

	uint32_t write_length(const uint8_t *write_buffer, uint32_t size);
	uint32_t write_lengthless(const char *str);

	uint32_t display_print(char *str);
	uint32_t display_println(void);

  // This MUST be defined separately:
  void drawPixel(int16_t x, int16_t y, uint16_t color);

  // These MAY be overridden to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  void
    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillScreen(uint16_t color),
    invertDisplay(uint8_t i);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void
    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      uint16_t color),
    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta, uint16_t color),
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
      uint16_t bg, uint8_t size),
    setCursor(int16_t x, int16_t y),
    setTextColor(uint16_t c, uint16_t bg),
    setTextSize(uint8_t s),
    setTextWrap(uint8_t w),
    setRotation(uint8_t r);

   uint32_t display_write(uint8_t);

  int16_t
    display_height(void),
    display_width(void);

  uint8_t getRotation(void);

#endif // _GGFX_H
