#include "stm32f1xx_ll_flash.h"
#include "stm32f1xx_ll_bus.h"

void LL_Flash_Unlock()
{
	if (FLASH->CR & FLASH_CR_LOCK) {
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}
}

void LL_Flash_Lock()
{
	FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t LL_Flash_ErasePage(uint32_t address)
{
	LL_Flash_Unlock();
	
	while (FLASH->SR & FLASH_SR_BSY);
	
  // Clear previous errors
	FLASH->SR = FLASH_SR_PGERR | FLASH_SR_WRPRTERR | FLASH_SR_EOP;
	
	__disable_irq(); // disable interrupts before erasing
	FLASH->CR |= FLASH_CR_PER;   // page erase
	
	FLASH->AR  = address;        // page address
	FLASH->CR |= FLASH_CR_STRT;  // start erase

	while (FLASH->SR & FLASH_SR_BSY);

	FLASH->CR &= ~FLASH_CR_PER;  // clear erase bit
	__enable_irq();

	uint8_t ok = 1;
	if (FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR))
	{
		ok = 0;          // operation failed with an error
	}
	else if (!(FLASH->SR & FLASH_SR_EOP))
	{
    ok = 0;           // operation did not complete successfully
	}

	FLASH->SR = FLASH_SR_EOP;

	LL_Flash_Lock();
	return ok;
}

uint8_t LL_Flash_ProgramHalfWord(uint32_t address, uint16_t data)
{
	LL_Flash_Unlock();
	while (FLASH->SR & FLASH_SR_BSY);
	
  // Clear previous errors
	FLASH->SR = FLASH_SR_PGERR | FLASH_SR_WRPRTERR | FLASH_SR_EOP;

	__disable_irq(); // disable interrupts before programming
	FLASH->CR |= FLASH_CR_PG;       // enable programming

	*(__IO uint16_t*)address = data;

	while (FLASH->SR & FLASH_SR_BSY);

	FLASH->CR &= ~FLASH_CR_PG;      // disable programming
	__enable_irq();

	uint8_t ok = 1;
	if (FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR))
	{
		ok = 0;          // operation failed with an error
	}
	else if (!(FLASH->SR & FLASH_SR_EOP))
	{
    ok = 0;           // operation did not complete successfully
	}

	FLASH->SR = FLASH_SR_EOP;

	LL_Flash_Lock();
	return ok;
}
