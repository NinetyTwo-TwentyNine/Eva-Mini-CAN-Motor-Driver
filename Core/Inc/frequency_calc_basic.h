#ifndef _FCB_H
#define _FCB_H

typedef struct SensorAddress_Timer SENSADDR_TIM_TypeDef;
typedef struct SensorAddress_GPIO SENSADDR_GPIO_TypeDef;

#include "main.h"

struct SensorAddress_Timer
{
	TIM_TypeDef* timer;
	uint8_t channel;
};

struct SensorAddress_GPIO
{
  GPIO_TypeDef* port;
	uint32_t pin_mask;
};

//uint32_t get_apb_timer_clock(TIM_TypeDef* sensor_timer);
float calculate_frequency(uint8_t sensor_num);
void capture_value(TIM_TypeDef* timer, uint8_t channel);

#endif // _FCB_H
