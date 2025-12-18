#include "user_params_management.h"

static const uint8_t flg_save_pos = (USER_DATA_SAVE_SIZE/2 - 1), crc_save_pos = (USER_DATA_SAVE_SIZE/2 - 2); // Flag and CRC positions (in half-words)
static const uint8_t params_total_count = USER_PARAMS_COUNT + 2, slots_total_count = 1024/USER_DATA_SAVE_SIZE;
static const uint16_t flg_slot_empty = 0xFFFF, flg_slot_valid = 0x0000;
static const uint16_t crc_calc_bgn = 0xFFFF, crc_calc_pwr = 0x1021;

// ======================
// Parameter management
// ======================

void setUserParameter(uint8_t pos, uint32_t parameter)
{
	if (user_params_array[pos] != parameter)
	{
		user_params_differentiate = true;
	}
	user_params_array[pos] = parameter;
}

uint32_t getUserParameter(uint8_t pos)
{
	return user_params_array[pos];
}

uint8_t checkIfAllUserParamsAreSet()
{
	uint32_t check_sum = 0;
	for (uint8_t i = 0; i < USER_PARAMS_COUNT; i++)
	{
		if (i == USER_PARAM_FAN_SPEED_MIN || i == USER_PARAM_FAN_SPEED_MIN) continue; // allow minimums to be zero
		check_sum |= (user_params_array[i] == 0) << i;
	}
	return (check_sum == 0);
}


// ======================
// Parameter calculation
// ======================

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

uint32_t calculateMinMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	for (uint8_t i = 0; i < 100; i++)
	{
		uint8_t speed = i;
		if ( performMotorSpeedCheck( calculateMotorSpeed_fromSpeed(quota_kg_per_ha, seeder_width_m, mass_per_turn_g, speed) ) )
		{
			return speed;
		}
	}
	return 0;
}

uint32_t calculateMaxMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	for (uint8_t i = 0; i < 100; i++)
	{
		uint8_t speed = 100-i-1;
		if ( performMotorSpeedCheck( calculateMotorSpeed_fromSpeed(quota_kg_per_ha, seeder_width_m, mass_per_turn_g, speed) ) )
		{
			return speed;
		}
	}
	return 0;
}


// ======================
// Parameter storing
// ======================

uint16_t crc16_ccitt(const uint8_t *data, uint16_t len)
{
	uint16_t crc = crc_calc_bgn;

	while (len--)
	{
		crc ^= (uint16_t)(*data++) << 8;
		
		for (uint8_t i = 0; i < 8; i++)
		{
			if (crc & 0x8000)
			{
				crc = (crc << 1) ^ crc_calc_pwr;
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
	for (uint8_t i = 0; i < slots_total_count; i++)
	{
		begin_addr = USER_DATA_SAVE_PAGE_ADDR + i * USER_DATA_SAVE_SIZE;
		
		uint16_t save_flg = flash_read16(begin_addr + flg_save_pos * 2);
		if (save_flg == flg_slot_empty)
		{
			break;
		}
		
		if (i == slots_total_count - 1)
		{
			if (!flash_erase_page(USER_DATA_SAVE_PAGE_ADDR)) return; // abort if erase fails
			begin_addr = USER_DATA_SAVE_PAGE_ADDR;
		}
	}
	
	
	uint32_t area_scaler = USER_DATA_SAVE_AREA_SCALER;
	uint32_t total_area = round(current_user_area_total * area_scaler), session_area = round(current_user_area_session * area_scaler);
	
	uint32_t params_array[params_total_count] = {0};
	for (uint8_t i = 0; i < USER_PARAMS_COUNT; i++)
	{
		params_array[i] = getUserParameter(i);
	}
	params_array[params_total_count-2] = total_area;
	params_array[params_total_count-1] = session_area;
	
	uint16_t crc16 = crc16_ccitt((uint8_t*)params_array, sizeof(params_array));
	
	
	flash_save16(begin_addr + flg_save_pos * 2, flg_slot_valid);
	for (uint8_t i = 0; i < params_total_count; i++)
	{
		flash_save32(begin_addr + i * 4, params_array[i]);
	}
	flash_save16(begin_addr + crc_save_pos * 2, crc16);
	
	user_params_differentiate = false;
	user_params_last_save_time = sys_timer;
	
	return;
}

void restore_user_params_batch()
{
	uint32_t params_array[params_total_count] = {0};
	
	uint32_t begin_addr;
	uint8_t invalid_slots_found = false;
	for (uint8_t i = slots_total_count - 1; i <= slots_total_count - 1; i--)
	{
	  begin_addr = USER_DATA_SAVE_PAGE_ADDR + i * USER_DATA_SAVE_SIZE;
		uint16_t save_flg = flash_read16(begin_addr + flg_save_pos * 2);
		if (save_flg == flg_slot_valid)
		{
			for (uint8_t j = 0; j < params_total_count; j++)
			{
				params_array[j] = flash_read32(begin_addr + j * 4);
			}
	
			uint16_t calc_crc16 = crc16_ccitt((uint8_t*)params_array, sizeof(params_array)), actl_crc16 = flash_read16(begin_addr + crc_save_pos * 2);
			if (actl_crc16 != calc_crc16)
			{
				invalid_slots_found = true;
			}
			else
			{
				break;
			}
		}
		else if (save_flg != flg_slot_empty)
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
		setUserParameter(i, params_array[i]);
	}
	uint32_t area_scaler = USER_DATA_SAVE_AREA_SCALER;
	current_user_area_total = (float)params_array[params_total_count - 2] / (float)area_scaler;
	current_user_area_session = (float)params_array[params_total_count - 1] / (float)area_scaler;
	
	user_params_differentiate = false;
	user_params_last_save_time = sys_timer;
	
	return;
}
