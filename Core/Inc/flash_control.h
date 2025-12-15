#ifndef _FLASH_C_H
#define _FLASH_C_H

#include "stm32f1xx_ll_flash.h"

uint8_t flash_erase_page(uint32_t address);

uint16_t flash_read16(uint32_t address);
uint32_t flash_read32(uint32_t address);
uint64_t flash_read64(uint32_t address);

uint8_t flash_save16(uint32_t address, uint16_t data);
uint8_t flash_save32(uint32_t address, uint32_t data);
uint8_t flash_save64(uint32_t address, uint64_t data);

#endif // _FLASH_C_H
