#ifndef _FCB_H
#define _FCB_H

#include "main.h"

//uint32_t get_tim1_clock(void);
float calculate_frequency(uint8_t sensor_num);
void capture_value(TIM_TypeDef* timer, uint8_t channel);

#endif // _FCB_H
