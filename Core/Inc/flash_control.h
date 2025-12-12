#ifndef _FLASH_C_H
#define _FLASH_C_H

#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

void flash_erase(uint32_t address);
void flash_write_halfword(uint32_t address, uint16_t half_word);

#endif // _FLASH_C_H
