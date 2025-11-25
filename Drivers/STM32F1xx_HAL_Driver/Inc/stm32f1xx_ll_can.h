/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F1xx_LL_CAN_H
#define STM32F1xx_LL_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"


void LL_CAN_Init(uint8_t remap);
uint8_t LL_CAN_Send(uint32_t id, uint8_t* data, uint8_t extended_address);


#endif /* STM32F1xx_LL_CAN_H */
