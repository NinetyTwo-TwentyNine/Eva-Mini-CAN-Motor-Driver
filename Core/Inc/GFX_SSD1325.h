#ifndef _GFX_SSD1325_H
#define _GFX_SSD1325_H

/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1325 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "GFX_General.h"
#include "i2c_control.h"
#include "ui_screen_system.h"


#define BLACK				0x00
#define DARKGRAY		0x04
#define LIGHTGRAY		0x0A
#define WHITE				0x0F

#define SSD1325_I2C_ADDRESS   0x3D	// 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1325 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1325_128_64  128x64 pixel display

    SSD1325_128_32  128x32 pixel display

    -----------------------------------------------------------------------*/
#define SSD1325_128_64
//#define SSD1325_128_32
/*=========================================================================*/

#if defined SSD1325_128_64 && defined SSD1325_128_32
  #error "Only one SSD1325 display can be specified at once in SSD1325.h"
#endif
#if !defined SSD1325_128_64 && !defined SSD1325_128_32
  #error "At least one SSD1325 display must be specified in SSD1325.h"
#endif

#if defined SSD1325_128_64
  #define SSD1325_LCDWIDTH                  128
  #define SSD1325_LCDHEIGHT                 64
#endif
#if defined SSD1325_128_32
  #define SSD1325_LCDWIDTH                  128
  #define SSD1325_LCDHEIGHT                 32
#endif

#define SSD1325_MSB_FIRST true

#define SSD1325_SETCONTRAST 0x81 //!< Set contrast current
#define SSD1325_SETCURRENT 0x84  //!< Set current range

#define SSD1325_SETREMAP 0xA0      //!< Set re-map
#define SSD1325_SETSTARTLINE 0xA1  //!< Set display start line
#define SSD1325_SETOFFSET 0xA2     //!< Set display offset
#define SSD1325_NORMALDISPLAY 0xA4 //!< Set display mode normal
#define SSD1325_DISPLAYALLON 0xA5  //!< Set entire display on
#define SSD1325_DISPLAYALLOFF 0xA6 //!< Set entire display off
#define SSD1325_INVERTDISPLAY 0xA7 //!< Invert display
#define SSD1325_SETMULTIPLEX 0xA8  //!< Set multiplex ratio
#define SSD1325_MASTERCONFIG 0xAD  //!< Set master configuration
#define SSD1325_DISPLAYOFF 0xAE    //!< Set display off
#define SSD1325_DISPLAYON 0xAF     //!< Set display on

#define SSD1325_SETPRECHARGECOMPENABLE 0xB0                                   //!< Set pre-charge compensation enable
#define SSD1325_SETPHASELEN 0xB1         //!< Set phase length
#define SSD1325_SETROWPERIOD 0xB2        //!< Set row period
#define SSD1325_SETCLOCK 0xB3            //!< Set display clock divide ratio
#define SSD1325_SETPRECHARGECOMP 0xB4    //!< Set pre-charge
#define SSD1325_SETGRAYTABLE 0xB8        //!< Set gray scale table
#define SSD1325_SETPRECHARGEVOLTAGE 0xBC //!< Set precharge voltage
#define SSD1325_SETVCOMLEVEL 0xBE        //!< Set Vcomh Voltage
#define SSD1325_SETVSL 0xBF              //!< Set segment low voltage (VSL)

#define SSD1325_COLUMNADDR 0x15
#define SSD1325_ROWADDR   0x75

#define SSD1325_COMSCANINC 0xC0
#define SSD1325_COMSCANDEC 0xC8

#define SSD1325_SEGREMAP 0xA0

#define SSD1325_GRAYSCALETABLE 0xB8

#define SSD1325_EXTERNALVCC 0x1
#define SSD1325_SWITCHCAPVCC 0x2


extern GPIO_TypeDef *_cs_port, *_rst_port, *_dc_port, *_sclk_port, *_sid_port;
extern LL_GPIO_InitTypeDef *_cs_pin, *_rst_pin, *_dc_pin, *_sclk_pin, *_sid_pin;
extern int8_t _i2caddr, _vccstate;


  void ssd1325_begin(uint8_t switchvcc, uint8_t i2caddr);
	void ssd1325_begin_default(void);
  void ssd1325_command(uint8_t c);
  void ssd1325_data(uint8_t c);

  void display_invert(uint8_t i);
  void display_dim(uint8_t dim);
  void display_update(void);

  void gfx_clearBuffer(void);

  void gfx_drawPixel(int16_t x, int16_t y, uint8_t color);

  void gfx_drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
  void gfx_drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);

	void shiftOut(GPIO_TypeDef * dataPort, uint32_t dataPin, GPIO_TypeDef * clockPort, uint32_t clockPin, uint8_t bitOrder, uint8_t val);
  void fastSPIwrite(uint8_t c);
	void I2Cwrite_address(void);
	void I2Cwrite_data(uint8_t data);
	void I2Cwrite_finish(void);

  inline void gfx_drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint8_t color) __attribute__((always_inline));
  inline void gfx_drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint8_t color) __attribute__((always_inline));


	void display_buildUIScreen(UI_Screen* screen);

#endif // _GFX_SSD1325_H
