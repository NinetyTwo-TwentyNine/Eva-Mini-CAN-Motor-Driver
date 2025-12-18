/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F1xx_LL_FLASH_H
#define STM32F1xx_LL_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"


void LL_Flash_Unlock(void);
void LL_Flash_Lock(void);
uint8_t LL_Flash_ProgramHalfWord(uint32_t address, uint16_t data);
uint8_t LL_Flash_ErasePage(uint32_t address);


#endif /* STM32F1xx_LL_FLASH_H */
