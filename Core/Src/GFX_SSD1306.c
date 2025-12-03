#include "GFX_SSD1306.h"

GPIO_TypeDef *_cs_port = CS_PORT, *_rst_port = RST_PORT, *_dc_port = DC_PORT, *_sclk_port = SCLK_PORT, *_sid_port = SID_PORT;
LL_GPIO_InitTypeDef cs_buf, rst_buf, dc_buf, sclk_buf, sid_buf;
LL_GPIO_InitTypeDef *_cs_pin = &cs_buf, *_rst_pin = &rst_buf, *_dc_pin = &dc_buf, *_sclk_pin = &sclk_buf, *_sid_pin = &sid_buf;
LL_SPI_InitTypeDef spi_buf;
LL_SPI_InitTypeDef *_spi = &spi_buf;
LL_I2C_InitTypeDef i2c_buf;
LL_I2C_InitTypeDef *_i2c = &i2c_buf;
int8_t _i2caddr, _vccstate;

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = { 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8,
0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80,
0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01,
0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF,
0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00,
0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF,
0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF,
0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F,
0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03,
0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01,
0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03,
0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#if (SSD1306_LCDHEIGHT == 64)
0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F,
0x87, 0xC7, 0xF7, 0xFF, 0xFF, 0x1F, 0x1F, 0x3D, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0x7C, 0x7D, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F,
0x0F, 0x07, 0x1F, 0x7F, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0,
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00,
0x00, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0xF0, 0xF8, 0x1C, 0x0E,
0x06, 0x06, 0x06, 0x0C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFC,
0xFE, 0xFC, 0x00, 0x18, 0x3C, 0x7E, 0x66, 0xE6, 0xCE, 0x84, 0x00, 0x00, 0x06, 0xFF, 0xFF, 0x06,
0x06, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xF8,
0xFC, 0x4E, 0x46, 0x46, 0x46, 0x4E, 0x7C, 0x78, 0x40, 0x18, 0x3C, 0x76, 0xE6, 0xCE, 0xCC, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0C,
0x18, 0x18, 0x0C, 0x06, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0E, 0x0C, 0x18, 0x0C, 0x0F,
0x07, 0x01, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x07,
0x07, 0x0C, 0x0C, 0x18, 0x1C, 0x0C, 0x06, 0x06, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif
};



// the most basic function, set a single pixel
void gfx_drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= display_width()) || (y < 0) || (y >= display_height()))
    return;

  // check rotation, move pixel around if necessary
  switch (gfx_getRotation()) {
  case 1:
    swap(x, y);
    x = SCREEN_WIDTH - x - 1;
    break;
  case 2:
    x = SCREEN_WIDTH - x - 1;
    y = SCREEN_HEIGHT - y - 1;
    break;
  case 3:
    swap(x, y);
    y = SCREEN_HEIGHT - y - 1;
    break;
  }  

  // x is which column
  if (color == WHITE) 
    buffer[x+ (y/8)*SSD1306_LCDWIDTH] |= (1 << (y&7));  
  else
    buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); 
}

void ssd1306_begin(uint8_t vccstate, uint8_t i2caddr) {
	if (DC_PORT == GPIOA || CS_PORT == GPIOA || RST_PORT == GPIOA || SCLK_PORT == GPIOA || SID_PORT == GPIOA)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	}
	if (DC_PORT == GPIOB || CS_PORT == GPIOB || RST_PORT == GPIOB || SCLK_PORT == GPIOB || SID_PORT == GPIOB)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	}
		if (DC_PORT == GPIOC || CS_PORT == GPIOC || RST_PORT == GPIOC || SCLK_PORT == GPIOC || SID_PORT == GPIOC)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	}
	if (DC_PORT == GPIOD || CS_PORT == GPIOD || RST_PORT == GPIOD || SCLK_PORT == GPIOD || SID_PORT == GPIOD)
	{
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
	}
	
	LL_GPIO_StructInit(_cs_pin);
	LL_GPIO_StructInit(_rst_pin);
	LL_GPIO_StructInit(_dc_pin);
	LL_GPIO_StructInit(_sclk_pin);
	LL_GPIO_StructInit(_sid_pin);
	
	_cs_pin->Pin = CS_PIN;
	_rst_pin->Pin = RST_PIN;
	_dc_pin->Pin = DC_PIN;
	_sclk_pin->Pin = SCLK_PIN;
	_sid_pin->Pin = SID_PIN;
	
	_cs_pin->Pull = LL_GPIO_PULL_UP;
	_rst_pin->Pull = LL_GPIO_PULL_UP;
	_dc_pin->Pull = LL_GPIO_PULL_UP;

	_cs_pin->Speed = LL_GPIO_SPEED_FREQ_HIGH;
	_rst_pin->Speed = LL_GPIO_SPEED_FREQ_HIGH;
	_dc_pin->Speed = LL_GPIO_SPEED_FREQ_HIGH;
	_sclk_pin->Speed = LL_GPIO_SPEED_FREQ_HIGH;
	_sid_pin->Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
 // set pin directions
	if (SCREEN_USE_SPI && !SCREEN_SPI_HARDWARE)
	{
		_sclk_pin->Mode = LL_GPIO_MODE_OUTPUT;
		_sid_pin->Mode = LL_GPIO_MODE_OUTPUT;
	}
	else
	{
		_sclk_pin->Mode = LL_GPIO_MODE_ALTERNATE;
		_sid_pin->Mode = LL_GPIO_MODE_ALTERNATE;
	}
	
  if (SCREEN_USE_SPI){
		_dc_pin->Mode = LL_GPIO_MODE_OUTPUT;
		_cs_pin->Mode = LL_GPIO_MODE_OUTPUT;
		_dc_pin->OutputType  = LL_GPIO_OUTPUT_PUSHPULL;
		_cs_pin->OutputType  = LL_GPIO_OUTPUT_PUSHPULL;
		
		_sid_pin->OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		_sclk_pin->OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  }
  else
  {
		_sid_pin->OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
		_sclk_pin->OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
		
		_sclk_pin->Pull = LL_GPIO_PULL_UP;
		_sid_pin->Pull = LL_GPIO_PULL_UP;
  }
	
	_rst_pin->OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	_rst_pin->Mode       = LL_GPIO_MODE_OUTPUT;
	
	LL_GPIO_Init(_cs_port, _cs_pin);
	LL_GPIO_Init(_rst_port, _rst_pin);
	LL_GPIO_Init(_dc_port, _dc_pin);
	LL_GPIO_Init(_sclk_port, _sclk_pin);
	LL_GPIO_Init(_sid_port, _sid_pin);
	
	if (SCREEN_USE_SPI && SCREEN_SPI_HARDWARE)
	{
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
		
		if (SCREEN_SPI == SPI1)
		{
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
		}
		if (SCREEN_SPI == SPI2)
		{
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
		}
		
		LL_SPI_StructInit(_spi);
		_spi->BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
		_spi->BitOrder = LL_SPI_MSB_FIRST;
		_spi->DataWidth = LL_SPI_DATAWIDTH_8BIT;
		_spi->TransferDirection = LL_SPI_FULL_DUPLEX;
		_spi->ClockPolarity = LL_SPI_POLARITY_LOW;
		_spi->ClockPhase = LL_SPI_PHASE_1EDGE;
		_spi->NSS = LL_SPI_NSS_SOFT;
		_spi->Mode = LL_SPI_MODE_MASTER;
		_spi->CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
		_spi->CRCPoly = 10;
		LL_SPI_Init(SCREEN_SPI, _spi);
		
		LL_SPI_Enable(SCREEN_SPI);
	}
	if (!SCREEN_USE_SPI)
	{
		if (SCREEN_I2C == I2C1)
		{
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
		}
		if (SCREEN_I2C == I2C2)
		{
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
		}
		
		LL_I2C_StructInit(_i2c);
		_i2c->PeripheralMode  = LL_I2C_MODE_I2C;
		_i2c->ClockSpeed      = 100000; // 100 kHz
		_i2c->DutyCycle       = LL_I2C_DUTYCYCLE_2;
		_i2c->OwnAddress1     = 0;
		_i2c->TypeAcknowledge = LL_I2C_NACK;
		_i2c->OwnAddrSize     = LL_I2C_OWNADDRESS1_7BIT;
		LL_I2C_Init(SCREEN_I2C, _i2c);
		
		LL_I2C_Enable(SCREEN_I2C);
	}
	
  _vccstate = vccstate;
  _i2caddr = i2caddr;

  // Setup reset pin direction (used by both SPI and I2C) 
	LL_GPIO_SetOutputPin(_rst_port, _rst_pin->Pin);
  // VDD (3.3V) goes high at start, lets just chill for a ms
  LL_mDelay(1);
  // bring reset low
	LL_GPIO_ResetOutputPin(_rst_port, _rst_pin->Pin);
  // wait 10ms
  LL_mDelay(20);
  // bring out of reset
	LL_GPIO_SetOutputPin(_rst_port, _rst_pin->Pin);
  // turn on VCC (9V?)
		

   #if defined SSD1306_128_32
    // Init sequence for 128x32 OLED module
    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x1F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x10); }
    else 
      { ssd1306_command(0x14); }
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
		ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x02);
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    ssd1306_command(0x8F);
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x22); }
    else 
      { ssd1306_command(0xF1); }
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
  #endif

  #if defined SSD1306_128_64
    // Init sequence for 128x64 OLED module
    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x3F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x10); }
    else 
      { ssd1306_command(0x14); }
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    //ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
		
		/* SSD1315-Specific
	  // Unlock driver IC
    ssd1306_command(0xFD);
    ssd1306_command(0x12); // Unlock OLED driver IC (Set Command Lock)

    // Internal IREF setting
    ssd1306_command(0xAD); // Set IREF Setting
    ssd1306_command(0x30); // Enable internal IREF
		*/
		
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x9F); }
    else 
      { ssd1306_command(0xCF); }
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    if (vccstate == SSD1306_EXTERNALVCC) 
      { ssd1306_command(0x22); }
    else 
      { ssd1306_command(0xF1); }
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
  #endif
  
  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
			
	//LL_mDelay(300);
	//ssd1306_command(SSD1306_DISPLAYALLON);//--turn on oled panel
}	

void ssd1306_begin_default() {
	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
}

void display_invert(uint8_t i) {
  if (i) {
    ssd1306_command(SSD1306_INVERTDISPLAY);
  } else {
    ssd1306_command(SSD1306_NORMALDISPLAY);
  }
}

void ssd1306_command(uint8_t c) { 
  if (SCREEN_USE_SPI)
  {
    // SPI
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
		LL_GPIO_ResetOutputPin(_dc_port, _dc_pin->Pin);
		LL_GPIO_ResetOutputPin(_cs_port, _cs_pin->Pin);
    fastSPIwrite(c);
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
  }
  else
  {
    // I2C
		uint8_t control = 0x00;   // Co = 0, D/C = 0
		I2Cwrite_address();
		I2Cwrite_data(control);
		I2Cwrite_data(c);
		I2Cwrite_finish();
  }
}

void ssd1306_data(uint8_t c) {
  if (SCREEN_USE_SPI)
  {
    // SPI
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
		LL_GPIO_SetOutputPin(_dc_port, _dc_pin->Pin);
		LL_GPIO_ResetOutputPin(_cs_port, _cs_pin->Pin);
    fastSPIwrite(c);
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
  }
  else
  {
     // I2C
		uint8_t control = 0x40;   // Co = 0, D/C = 0
		I2Cwrite_address();
		I2Cwrite_data(control);
		I2Cwrite_data(c);
		I2Cwrite_finish();
  }
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void display_dim(uint8_t dim) {
  uint8_t contrast;

  if (dim) {
    contrast = 0; // Dimmed display
  } else {
    if (_vccstate == SSD1306_EXTERNALVCC) {
      contrast = 0x9F;
    } else {
      contrast = 0xCF;
    }
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  ssd1306_command(SSD1306_SETCONTRAST);
  ssd1306_command(contrast);
}

void display_update(void) {
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);   // Column start address (0 = reset)
  ssd1306_command(127); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0); // Page start address (0 = reset)
  ssd1306_command((SSD1306_LCDHEIGHT == 64) ? 7 : 3); // Page end address

  if (SCREEN_USE_SPI)
  {
    // SPI
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
		LL_GPIO_SetOutputPin(_dc_port, _dc_pin->Pin);
		LL_GPIO_ResetOutputPin(_cs_port, _cs_pin->Pin);
	  //__NOP();		// May not be necessary - needs testing

    for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) {
      fastSPIwrite(buffer[i]);
    }
	  //__NOP();		// May not be necessary - needs testing
		LL_GPIO_SetOutputPin(_cs_port, _cs_pin->Pin);
  }
  else
  {
    // I2C
		uint8_t control = 0x40;
		
    for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++)
		{
      // send a bunch of data in one xmission
			I2Cwrite_address();
			I2Cwrite_data(control);
			
      for (uint8_t x=0; x<16; x++) {
				I2Cwrite_data(buffer[i]);
				i++;
			}
			i--;
			
			I2Cwrite_finish();
		}
  }
}

// clear everything
void gfx_clearBuffer(void) {
  memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}


void shiftOut(GPIO_TypeDef * dataPort, uint32_t dataPin, GPIO_TypeDef * clockPort, uint32_t clockPin, uint8_t bitOrder, uint8_t val)
{
#if SSD1306_MSB_FIRST
	// MSB first
	for (uint8_t mask = 0x80; mask != 0; mask >>= 1)
	{
		if (val & mask)
		{
			LL_GPIO_SetOutputPin(dataPort, dataPin);
		}
		else
		{
			LL_GPIO_ResetOutputPin(dataPort, dataPin);
		}

		//__NOP(); __NOP(); __NOP();
		LL_GPIO_SetOutputPin(clockPort, clockPin);
		__NOP(); __NOP(); __NOP();
		LL_GPIO_ResetOutputPin(clockPort, clockPin);
	}
#else
	// LSB first
	for (uint8_t mask = 1; mask != 0; mask <<= 1)
	{
		if (val & mask)
		{
			LL_GPIO_SetOutputPin(dataPort, dataPin);
		}
		else
		{
			LL_GPIO_ResetOutputPin(dataPort, dataPin);
		}
		
		//__NOP(); __NOP(); __NOP();
		LL_GPIO_SetOutputPin(clockPort, clockPin);
		__NOP(); __NOP(); __NOP();
		LL_GPIO_ResetOutputPin(clockPort, clockPin);
  }
#endif
}

void fastSPIwrite(uint8_t d) {
  if (SCREEN_SPI_HARDWARE) {
		while (!LL_SPI_IsActiveFlag_TXE(SCREEN_SPI));
		LL_SPI_TransmitData8(SCREEN_SPI, d);
		while (!LL_SPI_IsActiveFlag_TXE(SCREEN_SPI));
		while (LL_SPI_IsActiveFlag_BSY(SCREEN_SPI));
  } else {
    shiftOut(_sid_port, _sid_pin->Pin, _sclk_port, _sclk_pin->Pin, 1, d);		// SSD1306 specs show MSB out first
  }
}

void I2Cwrite_address() {
	I2Ctransmission_initialize(SCREEN_I2C, _i2caddr, 0, 1);
}

void I2Cwrite_data(uint8_t data) {
	I2Ctransmission_write(SCREEN_I2C, data);
}

void I2Cwrite_finish() {
	I2Ctransmission_finish(SCREEN_I2C);
}

void gfx_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  uint8_t bSwap = false;
  switch(_rotation) { 
    case 0:
      // 0 degree rotation, do nothing
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x
      bSwap = true;
      swap(x, y);
      x = SCREEN_WIDTH - x - 1;
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = SCREEN_WIDTH - x - 1;
      y = SCREEN_HEIGHT - y - 1;
      x -= (w-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
      bSwap = true;
      swap(x, y);
      y = SCREEN_HEIGHT - y - 1;
      y -= (w-1);
      break;
  }

  if(bSwap) { 
    gfx_drawFastVLineInternal(x, y, w, color);
  } else { 
    gfx_drawFastHLineInternal(x, y, w, color);
  }
}

void gfx_drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Do bounds/limit checks
  if(y < 0 || y >= SCREEN_HEIGHT) { return; }

  // make sure we don't try to draw below 0
  if(x < 0) { 
    w += x;
    x = 0;
  }

  // make sure we don't go off the edge of the display
  if( (x + w) > SCREEN_WIDTH) { 
    w = (SCREEN_HEIGHT- x);
  }

  // if our width is now negative, punt
  if(w <= 0) { return; }

  // set up the pointer for  movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SSD1306_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  register uint8_t mask = 1 << (y&7);

  if(color == WHITE) { 
    while(w--) { *pBuf++ |= mask; }
  } else {
    mask = ~mask;
    while(w--) { *pBuf++ &= mask; }
  }
}

void gfx_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  uint8_t bSwap = false;
  switch(_rotation) { 
    case 0:
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
      bSwap = true;
      swap(x, y);
      x = SCREEN_WIDTH - x - 1;
      x -= (h-1);
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = SCREEN_WIDTH - x - 1;
      y = SCREEN_HEIGHT - y - 1;
      y -= (h-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y 
      bSwap = true;
      swap(x, y);
      y = SCREEN_HEIGHT - y - 1;
      break;
  }

  if(bSwap) { 
    gfx_drawFastHLineInternal(x, y, h, color);
  } else {
    gfx_drawFastVLineInternal(x, y, h, color);
  }
}


void gfx_drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {

  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= SCREEN_WIDTH) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) { 
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  } 

  // make sure we don't go past the height of the display
  if( (__y + __h) > SCREEN_HEIGHT) { 
    __h = (SCREEN_HEIGHT - __y);
  }

  // if our height is now negative, punt 
  if(__h <= 0) { 
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SSD1306_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set 
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if( h < mod) { 
      mask &= (0XFF >> (mod-h));
    }

    if(color == WHITE) { 
      *pBuf |= mask;
    } else {
      *pBuf &= ~mask;
    }

    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += SSD1306_LCDWIDTH;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) { 
    // store a local value to work with 
    register uint8_t val = (color == WHITE) ? 255 : 0;

    do  {
      // write our value in
      *pBuf = val;

      // adjust the buffer forward 8 rows worth of data
      pBuf += SSD1306_LCDWIDTH;

      // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
      h -= 8;
    } while(h >= 8);
  }

  // now do the final partial byte, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    if(color == WHITE) { 
      *pBuf |= mask;
    } else { 
      *pBuf &= ~mask;
    }
  }
}

// UI screen visualizer
void display_buildUIScreen(UI_Screen* screen)
{
	gfx_clearBuffer();
	
	uint16_t offset_y = screen->offset_y;
	UI_Element_Visual* curr_hovered = screen->hovered;
	if (curr_hovered != NULL)
	{
		uint8_t font_size = UI_MAIN_TEXT_SIZE;
		if (curr_hovered->type == VISUAL_TYPE_TEXT)
		{
			font_size = curr_hovered->data.text.font;
		}
		
		uint16_t comparison_height_down = (_height + offset_y - font_size * CHAR_BASE_HEIGHT * 3 / 2), comparison_height_up = CHAR_BASE_HEIGHT / 2;
		if ((int16_t)curr_hovered->pos_y - (int16_t)offset_y > comparison_height_down)
		{
			offset_y += (int16_t)curr_hovered->pos_y - (int16_t)offset_y - (int16_t)comparison_height_down;
		}
		else if ((int16_t)curr_hovered->pos_y - (int16_t)offset_y < comparison_height_up)
		{
			offset_y -= (int16_t)offset_y - (int16_t)curr_hovered->pos_y + (int16_t)comparison_height_up;
		} 
		screen->offset_y = offset_y;
	}
	
	for (uint8_t i = 0; i < screen->visuals_count; i++)
	{
		UI_Element_Visual* curr_visual = &(screen->visuals[i]);
		int16_t x0 = curr_visual->pos_x, y0 = curr_visual->pos_y - offset_y;
		gfx_setCursor(x0, y0);
		switch(curr_visual->type)
		{
			case VISUAL_TYPE_TEXT:
			{
				gfx_setTextColor(WHITE, WHITE);
				gfx_setTextSize(curr_visual->data.text.font);
				gfx_print(curr_visual->data.text.text);
			} break;
			case VISUAL_TYPE_LINES:
			{
				uint8_t prev_pos_x = x0, prev_pos_y = y0;
				for (uint8_t j = 0; j < curr_visual->data.lines.line_count-1; j++)
				{
					if (j >= UI_MAX_ELEMENT_COUNT)
					{
						break;
					}
					uint8_t new_pos_x = x0 + curr_visual->data.lines.x_n[j], new_pos_y = y0 + curr_visual->data.lines.y_n[j];
					gfx_drawLine(prev_pos_x, prev_pos_y, new_pos_x, new_pos_y, WHITE);
					prev_pos_x = new_pos_x;
					prev_pos_y = new_pos_y;
				}
			}	break;
			case VISUAL_TYPE_TRIANGLE:
			{
				uint8_t x1 = x0 + curr_visual->data.triangle.x1, y1 = y0 + curr_visual->data.triangle.y1, x2 = x0 + curr_visual->data.triangle.x2, y2 = y0 + curr_visual->data.triangle.y2;
				gfx_drawTriangle(x0, y0, x1, y1, x2, y2, WHITE);
				if (!curr_visual->data.triangle.is_hollow)
				{
					gfx_fillTriangle(x0, y0, x1, y1, x2, y2, WHITE);
				}
			}	break;
			case VISUAL_TYPE_RECTANGLE: 
			{
				gfx_drawRect(x0, y0, curr_visual->data.rectangle.w, curr_visual->data.rectangle.h, WHITE);
				if (!curr_visual->data.rectangle.is_hollow)
				{
					gfx_fillRect(x0, y0, curr_visual->data.rectangle.w, curr_visual->data.rectangle.h, WHITE);
				}
			}	break;
			case VISUAL_TYPE_CIRCLE:
			{
				uint8_t r = curr_visual->data.circle.radius, new_x = x0 + r, new_y = y0 - r;
				gfx_drawCircle(new_x, new_y, r, WHITE);
				if (!curr_visual->data.circle.is_hollow)
				{
					gfx_fillCircle(new_x, new_y, r, WHITE);
				}
			}	break;
			case VISUAL_TYPE_BITMAP:
			{
				gfx_drawBitmap(x0, y0, curr_visual->data.bitmap.data, curr_visual->data.bitmap.w, curr_visual->data.bitmap.h, WHITE);
			}	break;
		}
	}
	
	// Cursor handling
	if (screen->should_draw_cursor && curr_hovered != NULL && curr_hovered->type == VISUAL_TYPE_TEXT)
	{
		char* text_str = curr_hovered->data.text.text;
		uint8_t text_font = curr_hovered->data.text.font;
		
		uint8_t cursor_left_right = screen->cursor_left_or_right, cursor_offset = curr_hovered->cursor_offset;
		
		uint8_t triangle_centerline = CHAR_BASE_HEIGHT * 3 / 4;
		uint16_t triangle_x0 = curr_hovered->pos_x, triangle_y0 = curr_hovered->pos_y + (text_font * CHAR_BASE_HEIGHT / 2 - 1) - offset_y;
		if (cursor_left_right == 0)
		{
			if (cursor_offset > triangle_x0)
			{
				triangle_x0 = triangle_centerline; // In case the cursor's out of bounds
			}
			else
			{
				triangle_x0 -= cursor_offset;
			}
		}
		else
		{
			uint16_t text_length = strlen(text_str);
			for (uint16_t i = 0; i < text_length; i++)
			{
				if ( text_str[i] == '\n' || (_wrap && triangle_x0 > SCREEN_WIDTH - CHAR_BASE_WIDTH * text_font))
				{
					triangle_y0 += text_font * CHAR_BASE_HEIGHT;
					triangle_x0 = 0;
				}
				else
				{
					triangle_x0 += CHAR_BASE_WIDTH;
				}
			}
			if (triangle_x0 + cursor_offset + triangle_centerline > _width)
			{
				triangle_x0 = _width - triangle_centerline - 1; // In case the cursor's out of bounds
			}
			else
			{
				triangle_x0 += cursor_offset;
			}
		}

		uint16_t triangle_x1, triangle_y1, triangle_x2, triangle_y2;
		triangle_x1 = cursor_left_right ? (triangle_x0 + triangle_centerline) : (triangle_x0 - triangle_centerline);
		triangle_x2 = triangle_x1;
		triangle_y1 = triangle_y0 + triangle_centerline, triangle_y2 = triangle_y0 - triangle_centerline;
		
		gfx_drawTriangle(triangle_x0, triangle_y0, triangle_x1, triangle_y1, triangle_x2, triangle_y2, WHITE);
		if (!screen->item_is_selected)
		{
			gfx_fillTriangle(triangle_x0, triangle_y0, triangle_x1, triangle_y1, triangle_x2, triangle_y2, WHITE);
		}
	}
	
	display_update();
	display_invert(UI_MAIN_COLOR_INVERTED);
}		

