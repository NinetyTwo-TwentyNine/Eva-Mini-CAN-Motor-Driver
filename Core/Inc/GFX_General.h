#ifndef _GGFX_H
#define _GGFX_H

#include <string.h>
#include "main.h"

#define CHAR_BASE_HEIGHT 8
#define CHAR_BASE_WIDTH 6

#define swap(a, b) { int16_t t = a; a = b; b = t; }

extern uint16_t _width, _height, _cursor_y, _cursor_x, _textcolor, _textbgcolor;
extern uint8_t _rotation, _textsize, _wrap;

	uint32_t gfx_write_length(const uint8_t *write_buffer, uint32_t size);
	uint32_t gfx_write_lengthless(const char *str);

	uint32_t gfx_print(char *str);
	uint32_t gfx_println(void);

  // This MUST be defined separately:
  void gfx_drawPixel(int16_t x, int16_t y, uint16_t color);

  // These MAY be overridden to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  void
    gfx_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    gfx_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    gfx_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    gfx_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    gfx_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    gfx_fillScreen(uint16_t color),
    gfx_invertDisplay(uint8_t i);

  // These exist without overrides
  void
    gfx_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    gfx_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      uint16_t color),
    gfx_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    gfx_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta, uint16_t color),
    gfx_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    gfx_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    gfx_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    gfx_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    gfx_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    gfx_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
      uint16_t bg, uint8_t size),
    gfx_setCursor(int16_t x, int16_t y),
    gfx_setTextColor(uint16_t c, uint16_t bg),
    gfx_setTextSize(uint8_t s),
    gfx_setTextWrap(uint8_t w),
    gfx_setRotation(uint8_t r);

   uint32_t gfx_write(uint8_t);

  int16_t
    display_height(void),
    display_width(void);

  uint8_t gfx_getRotation(void);

#endif // _GGFX_H
