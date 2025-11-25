#include "frequency_calc_basic.h"

uint32_t get_apb1_timer_clock() {
	uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;

	// If APB1 prescaler > 1 ? timer clock = PCLK1 × 2
	if (ppre1 == 0) {
    // HCLK not divided ? PCLK1 = HCLK
    return SystemCoreClock;
	} else {
    // Decode prescaler (bitfield values start at divide-by-2)
    uint32_t div = 1 << ((ppre1 - 3) + 1); // maps 0b100=2, 0b101=4, etc.
    uint32_t pclk1 = SystemCoreClock / div;
    return pclk1 * 2;
	}
}

uint32_t get_apb2_timer_clock() {
	uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;

	// If APB2 prescaler > 1 ? timer clock = PCLK2 × 2
	if (ppre2 == 0) {
    // HCLK not divided ? PCLK2 = HCLK
    return SystemCoreClock;
	} else {
    // Decode prescaler (bitfield values start at divide-by-2)
    uint32_t div = 1 << ((ppre2 - 3) + 1); // maps 0b100=2, 0b101=4, etc.
    uint32_t pclk2 = SystemCoreClock / div;
    return pclk2 * 2;
	}
}

float calculate_frequency(uint8_t sensor_num)
{
	float delta = IC_Array32[IC_ARRAY32_POS_DIFF][sensor_num];
	TIM_TypeDef* sensor_timer = sensor_address[sensor_num]->timer;
	uint32_t clock_speed;
	if (sensor_timer == TIM1)
	{
		clock_speed = get_apb2_timer_clock();
	}
	else
	{
		clock_speed = get_apb1_timer_clock();
	}
	uint32_t timer_clk_hz = clock_speed / (sensor_timer->PSC + 1);
	
	IC_Array8[IC_ARRAY8_POS_CAPTURE_COMPLETE][sensor_num] = 0;
	return (float)timer_clk_hz / delta;
}

void capture_value(TIM_TypeDef* timer, uint8_t channel)
{
	uint8_t sensor_num;
	for (uint8_t i = 0; i < SENSOR_COUNT_MAX; i++)
	{
		if (sensor_address[i]->timer == timer && sensor_address[i]->channel == channel)
		{
			sensor_num = i;
			break;
		}
		
		if (i == SENSOR_COUNT_MAX - 1)
		{
			return;
		}
	}
	
	if (IC_Array8[IC_ARRAY8_POS_CAPTURE_ERROR][sensor_num])
	{
		IC_Array8[IC_ARRAY8_POS_CAPTURE_ERROR][sensor_num] = 0;
		IC_Array8[IC_ARRAY8_POS_CAPTURE_COMPLETE][sensor_num] = 0;
		IC_Array8[IC_ARRAY8_POS_CAPTURE_INITIAL][sensor_num] = 1;
		IC_Array8[IC_ARRAY8_POS_OVERFLOW_COUNT][sensor_num] = 0;
	}
	
  uint32_t current_capture;
  switch(sensor_address[sensor_num]->channel)
  {
    case 1: current_capture = LL_TIM_IC_GetCaptureCH1(sensor_address[sensor_num]->timer); break;
    case 2: current_capture = LL_TIM_IC_GetCaptureCH2(sensor_address[sensor_num]->timer); break;
    case 3: current_capture = LL_TIM_IC_GetCaptureCH3(sensor_address[sensor_num]->timer); break;
    case 4: current_capture = LL_TIM_IC_GetCaptureCH4(sensor_address[sensor_num]->timer); break;
		default: return;
  }
  IC_Array32[IC_ARRAY32_POS_VAL2][sensor_num] = current_capture;
  uint32_t last_capture = IC_Array32[IC_ARRAY32_POS_VAL1][sensor_num];

	if (IC_Array8[IC_ARRAY8_POS_OVERFLOW_COUNT][sensor_num] == 0 && last_capture > current_capture)
	{
		IC_Array8[IC_ARRAY8_POS_CAPTURE_ERROR][sensor_num] = 1;
	}
  if (!IC_Array8[IC_ARRAY8_POS_CAPTURE_INITIAL][sensor_num]) { 
    // Calculate difference, considering overflows
    IC_Array32[IC_ARRAY32_POS_DIFF][sensor_num] = (IC_Array8[IC_ARRAY8_POS_OVERFLOW_COUNT][sensor_num] * (sensor_address[sensor_num]->timer->ARR + 1) + current_capture - last_capture);
    
    IC_Array8[IC_ARRAY8_POS_CAPTURE_COMPLETE][sensor_num] = 1;
    IC_Array8[IC_ARRAY8_POS_OVERFLOW_COUNT][sensor_num] = 0; // reset overflow counter
  }
	IC_Array8[IC_ARRAY8_POS_CAPTURE_INITIAL][sensor_num] = 0;

  // Save current capture as last capture
  IC_Array32[IC_ARRAY32_POS_VAL1][sensor_num] = IC_Array32[IC_ARRAY32_POS_VAL2][sensor_num];
}
