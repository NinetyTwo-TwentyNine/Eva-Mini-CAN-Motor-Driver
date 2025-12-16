#include "flash_control.h"

uint16_t flash_read16(uint32_t address)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
	
	uint16_t value = *(__IO uint16_t*)address;
	return value;
}

uint32_t flash_read32(uint32_t address)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
	
	uint8_t size = sizeof(uint32_t)/sizeof(uint16_t);
	uint32_t value = 0;
	for (uint8_t i = 0; i < size; i++)
	{
		uint16_t value1 = flash_read16(address + 2 * i);
		value |= ((uint32_t)value1) << (i*16);
	}
	
	return value;
}

uint64_t flash_read64(uint32_t address)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
	
	uint8_t size = sizeof(uint64_t)/sizeof(uint16_t);
	uint64_t value = 0;
	for (uint8_t i = 0; i < size; i++)
	{
		uint16_t value1 = flash_read16(address + 2 * i);
		value |= ((uint64_t)value1) << (i*16);
	}
	
	return value;
}

uint8_t flash_save16(uint32_t address, uint16_t data)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
	if (address & 0x01) return 0;
	
	return LL_Flash_ProgramHalfWord(address, data);
}

uint8_t flash_save32(uint32_t address, uint32_t data)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
	if (address & 0x03) return 0;
	
	uint8_t size = sizeof(uint32_t)/sizeof(uint16_t);
	for (uint8_t i = 0; i < size; i++)
	{
		uint16_t data1 = data >> (i*16);
		if (!flash_save16(address + 2 * i, data1))
			return 0;
	}
	
	return 1;
}

uint8_t flash_save64(uint32_t address, uint64_t data)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
	if (address & 0x07) return 0;
	
	uint8_t size = sizeof(uint64_t)/sizeof(uint16_t);
	for (uint8_t i = 0; i < size; i++)
	{
		uint16_t data1 = data >> (i*16);
		if (!flash_save16(address + 2 * i, data1))
			return 0;
	}
	
	return 1;
}

uint8_t flash_erase_page(uint32_t address)
{
	if (address < FLASH_BASE || address >= FLASH_BANK1_END) return 0;
  if (address & (1024 - 1)) return 0;  // page alignment
	
	return LL_Flash_ErasePage(address);
}
