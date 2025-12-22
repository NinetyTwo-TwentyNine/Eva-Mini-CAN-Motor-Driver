#include "user_params_management.h"

// ======================
// Parameter management
// ======================

uint8_t setUserParameterInt(uint8_t pos, uint32_t parameter)
{
	if (USER_PARAMS_FLOAT_MASK & (1 << pos)) return 0;
	
	if (user_params_array[pos] != parameter)
	{
		user_params_differentiate = true;
	}
	user_params_array[pos] = parameter;
	return 1;
}

uint8_t setUserParameterFloat(uint8_t pos, float parameter)
{
	if (!(USER_PARAMS_FLOAT_MASK & (1 << pos))) return 0;
	
	float old_param;
	memcpy(&old_param, &user_params_array[pos], sizeof(old_param));
	
	if (parameter != old_param)
	{
		user_params_differentiate = true;
	}
	memcpy(&user_params_array[pos], &parameter, sizeof(user_params_array[pos]));
	return 1;
}

uint32_t getUserParameterInt(uint8_t pos)
{
	if (USER_PARAMS_FLOAT_MASK & (1 << pos)) return 0;
	
	return user_params_array[pos];
}

float getUserParameterFloat(uint8_t pos)
{
	if (!(USER_PARAMS_FLOAT_MASK & (1 << pos))) return 0;
	
	float user_param;
	memcpy(&user_param, &user_params_array[pos], sizeof(user_param));
	return user_param;
}

uint8_t checkIfSeederParamsAreSet()
{
	float user_speed_min = getUserParameterFloat(USER_PARAM_SPEED_MIN),
				user_speed_max = getUserParameterFloat(USER_PARAM_SPEED_MAX);
	uint32_t user_mass_per_turn = getUserParameterInt(USER_PARAM_MASS_PER_TURN),
					 user_quota = getUserParameterInt(USER_PARAM_MASS_PER_TURN),
					 user_seeder_width = getUserParameterInt(USER_PARAM_SEEDER_WIDTH);
	
	uint8_t seeder_options_check = (user_speed_min >= SENSOR_VALUE_SPEED_BORDER_MIN && user_speed_max > user_speed_min && user_mass_per_turn != 0 && user_quota != 0 && user_seeder_width != 0);
	return seeder_options_check;
}

uint8_t checkIfOtherParamsAreSet()
{
	uint32_t user_fan_speed_min = getUserParameterInt(USER_PARAM_FAN_SPEED_MIN),
					 user_fan_speed_max = getUserParameterInt(USER_PARAM_FAN_SPEED_MAX),
					 user_wheel_diameter = getUserParameterInt(USER_PARAM_WHEEL_DIAMETER),
					 user_wheel_pulses = getUserParameterInt(USER_PARAM_WHEEL_PULSES);
	
	uint8_t other_options_check = (user_fan_speed_max > user_fan_speed_min && (user_fan_speed_max - user_fan_speed_min) >= SENSOR_VALUE_FAN_MIN_RANGE && user_wheel_diameter != 0 && user_wheel_pulses != 0);
	return other_options_check;
}


// ======================
// Parameter calculation
// ======================

uint32_t getPow10(uint8_t pwr)
{
	uint32_t scaler = 1;
	for (uint8_t i = 0; i < pwr; i++)
		scaler *= 10;
	return scaler;
}

float equate_kg_per_ha_and_g_per_sq_m(float input, uint8_t from_kg_per_ha)
{
	if (from_kg_per_ha)
	{
		return ( input * GRAMS_IN_KILOGRAM / SQUARE_METERS_IN_HECTARE );
	}
	else
	{
		return ( input * SQUARE_METERS_IN_HECTARE / GRAMS_IN_KILOGRAM );
	}
}

float equate_km_per_h_and_m_per_s(float input, uint8_t from_km_per_h)
{
	if (from_km_per_h)
	{
		return ( input * METERS_IN_KILOMETER / MINUTES_IN_HOUR / SECONDS_IN_MINUTE );
	}
	else
	{
		return ( input * SECONDS_IN_MINUTE * MINUTES_IN_HOUR / METERS_IN_KILOMETER );
	}
}


uint32_t calculateTimeMillis_fromArea(uint32_t seeder_speed_kmh, uint32_t seeder_width_m, uint8_t area_divider)
{
	if (seeder_speed_kmh != 0 && seeder_width_m != 0 && area_divider != 0)
	{
		return round( (float)SQUARE_METERS_IN_HECTARE / (float)area_divider / ((float)seeder_speed_kmh * METERS_IN_KILOMETER / SECONDS_IN_MINUTE / MINUTES_IN_HOUR * (float)seeder_width_m) * MILLIS_IN_SECOND );
	}
	return 0;
}

float calculateSeederSpeed_fromSensorOutput(uint32_t wheel_diameter_dm, uint32_t pulses_per_wheel_count, float sensor_frequency_hz)
{
	if (pulses_per_wheel_count != 0)
	{
		return equate_km_per_h_and_m_per_s( (float)wheel_diameter_dm / DECIMETERS_IN_METER * sensor_frequency_hz / pulses_per_wheel_count, false );
	}
	return 0;
}

float calculateMotorSpeed_fromTime(uint32_t quota_kg_per_ha, uint8_t area_divider, uint32_t mass_per_turn_g, uint32_t time_total_millis)
{
	if (time_total_millis != 0 && mass_per_turn_g != 0 && area_divider != 0)
	{
		return ( (float)quota_kg_per_ha / (float)area_divider * GRAMS_IN_KILOGRAM / (float)mass_per_turn_g / ((float)time_total_millis / MILLIS_IN_SECOND / SECONDS_IN_MINUTE) );
	}
	return 0;
}

float calculateMotorSpeed_fromSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g, float seeder_speed_kmh)
{
	if (mass_per_turn_g != 0)
	{
		return ( equate_kg_per_ha_and_g_per_sq_m(quota_kg_per_ha, true) * equate_km_per_h_and_m_per_s(seeder_speed_kmh, true) * (float)seeder_width_m * SECONDS_IN_MINUTE / (float)mass_per_turn_g );
	}
	return 0;
}

float calculateQuota_fromSpeed(uint32_t seeder_width_m, float seeder_speed_kmh, uint32_t mass_per_turn_g, float motor_speed_turns_per_min)
{
	if (seeder_speed_kmh != 0 && seeder_width_m != 0)
	{
		return ( equate_kg_per_ha_and_g_per_sq_m((float)mass_per_turn_g * motor_speed_turns_per_min / (seeder_speed_kmh * METERS_IN_KILOMETER / MINUTES_IN_HOUR) / (float)seeder_width_m, false) );
	}
	return 0;
}

float calculateAreaAddition_fromSpeed(float begin_speed_kmh, float end_speed_kmh, uint32_t time_millis, uint32_t seeder_width_m)
{
	return ( equate_km_per_h_and_m_per_s((begin_speed_kmh + end_speed_kmh) / 2, true) * (float)seeder_width_m * ((float)MAIN_LOGIC_TICK_TIME / MILLIS_IN_SECOND) / SQUARE_METERS_IN_HECTARE );
}


uint8_t performMotorSpeedCheck(float motor_speed)
{
	uint32_t motor_check = round( motor_speed * 10 );
	return ( motor_check > (MOTOR_SPEED_LIMIT_MIN * 10) && motor_check <= (MOTOR_SPEED_LIMIT_MAX * 10) );
}

float calculateSpeed_fromMotorSpeed(float motor_speed_turns_per_min, uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	if (quota_kg_per_ha != 0 && seeder_width_m != 0)
	{
		return equate_km_per_h_and_m_per_s(motor_speed_turns_per_min / SECONDS_IN_MINUTE * (float)mass_per_turn_g / equate_kg_per_ha_and_g_per_sq_m(quota_kg_per_ha, true) / (float)seeder_width_m, false);
	}
	return 0;
}

float calculateMinSpeed_fromMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	float min_speed = calculateSpeed_fromMotorSpeed(MOTOR_SPEED_LIMIT_MIN, quota_kg_per_ha, seeder_width_m, mass_per_turn_g);
	min_speed += SENSOR_VALUE_SPEED_BORDER_MIN;
	
	return min_speed;
}

float calculateMaxSpeed_fromMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	float max_speed = calculateSpeed_fromMotorSpeed(MOTOR_SPEED_LIMIT_MAX, quota_kg_per_ha, seeder_width_m, mass_per_turn_g);
	
	if (max_speed < SENSOR_VALUE_SPEED_BORDER_MIN)
	{
		max_speed = 0;
	}
	else
	{
		max_speed -= SENSOR_VALUE_SPEED_BORDER_MIN;
	}
	return max_speed;
}


// ======================
// Parameter storing
// ======================

uint16_t crc16_ccitt(const uint8_t *data, uint16_t len)
{
	uint16_t crc = CRC_CALC_BGN;

	while (len--)
	{
		crc ^= (uint16_t)(*data++) << 8;
		
		for (uint8_t i = 0; i < 8; i++)
		{
			if (crc & 0x8000)
			{
				crc = (crc << 1) ^ CRC_CALC_XOR;
			}
			else
			{
				crc <<= 1;
			}
		}
	}

	return crc;
}

void save_user_params_batch()
{
	uint32_t begin_addr;
	for (uint8_t i = 0; i < SAVE_SLOTS_COUNT; i++)
	{
		begin_addr = USER_DATA_SAVE_PAGE_ADDR + i * USER_DATA_SAVE_SIZE;
		
		uint16_t save_flg = flash_read16(begin_addr + SAVE_POS_FLAG * 2);
		if (save_flg == SAVE_FLAG_SLOT_EMPTY)
		{
			break;
		}
		
		if (i == SAVE_SLOTS_COUNT - 1)
		{
			if (!flash_erase_page(USER_DATA_SAVE_PAGE_ADDR)) return; // abort if erase fails
			begin_addr = USER_DATA_SAVE_PAGE_ADDR;
		}
	}
	
	uint16_t crc16 = crc16_ccitt((uint8_t*)user_params_array, sizeof(user_params_array));
	
	
	flash_save16(begin_addr + SAVE_POS_FLAG * 2, SAVE_FLAG_SLOT_VALID);
	for (uint8_t i = 0; i < USER_PARAMS_COUNT; i++)
	{
		flash_save32(begin_addr + i * 4, user_params_array[i]);
	}
	flash_save16(begin_addr + SAVE_POS_CRC * 2, crc16);
	
	user_params_differentiate = false;
	user_params_last_save_time = sys_timer;
	
	return;
}

void restore_user_params_batch()
{
	uint32_t params_array[USER_PARAMS_COUNT] = {0};
	
	uint32_t begin_addr;
	uint8_t invalid_slots_found = false;
	for (uint8_t i = SAVE_SLOTS_COUNT - 1; i <= SAVE_SLOTS_COUNT - 1; i--)
	{
	  begin_addr = USER_DATA_SAVE_PAGE_ADDR + i * USER_DATA_SAVE_SIZE;
		uint16_t save_flg = flash_read16(begin_addr + SAVE_POS_FLAG * 2);
		if (save_flg == SAVE_FLAG_SLOT_VALID)
		{
			for (uint8_t j = 0; j < USER_PARAMS_COUNT; j++)
			{
				params_array[j] = flash_read32(begin_addr + j * 4);
			}
	
			uint16_t calc_crc16 = crc16_ccitt((uint8_t*)params_array, sizeof(params_array)), actl_crc16 = flash_read16(begin_addr + SAVE_POS_CRC * 2);
			if (actl_crc16 != calc_crc16)
			{
				invalid_slots_found = true;
			}
			else
			{
				break;
			}
		}
		else if (save_flg != SAVE_FLAG_SLOT_EMPTY)
		{
			invalid_slots_found = true;
		}
		
		if (i == 0)
		{
			if (invalid_slots_found)
			{
				flash_erase_page(USER_DATA_SAVE_PAGE_ADDR);
			}
			return;
		}
	}
	
	
	for (uint8_t i = 0; i < USER_PARAMS_COUNT; i++)
	{
		user_params_array[i] = params_array[i];
	}
	
	user_params_differentiate = false;
	user_params_last_save_time = sys_timer;
	
	return;
}
