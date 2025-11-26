#ifndef _FCB_H
#define _FCB_H

typedef struct SensorAddress SENSADDR_TypeDef;

#include "main.h"

struct SensorAddress{
	TIM_TypeDef* timer;
	uint8_t channel;
};

//uint32_t get_apb_timer_clock(TIM_TypeDef* sensor_timer);
float calculate_frequency(uint8_t sensor_num);
void capture_value(TIM_TypeDef* timer, uint8_t channel);

#endif // _FCB_H
