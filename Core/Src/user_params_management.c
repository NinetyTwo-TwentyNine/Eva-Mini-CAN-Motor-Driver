#include "user_params_management.h"


// ======================
// Parameter calculation
// ======================

uint32_t calculateTimeMillis_fromArea(uint32_t seeder_speed_kmh, uint32_t seeder_width_m, uint8_t area_divider)
{
	if (seeder_speed_kmh != 0 && seeder_width_m != 0 && area_divider != 0)
	{
		return round( (float)SQUARE_METERS_IN_HECTARE / (float)area_divider / ((float)seeder_speed_kmh * METERS_IN_KILOMETER / SECONDS_IN_MINUTE / MINUTES_IN_HOUR * (float)seeder_width_m) * MILLIS_IN_SECOND );
	}
	return 0;
}

float calculateMotorSpeed_fromTime(uint32_t quota_kg_per_ha, uint8_t area_divider, uint32_t mass_per_turn_g, uint32_t time_total_millis)
{
	if (time_total_millis != 0 && mass_per_turn_g != 0 && area_divider != 0)
	{
		return ( (float)quota_kg_per_ha / (float)area_divider * GRAMS_IN_KILOGRAM / (float)mass_per_turn_g / ((float)time_total_millis / SECONDS_IN_MINUTE / MILLIS_IN_SECOND) );
	}
	return 0;
}

float calculateMotorSpeed_fromSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g, uint32_t seeder_speed_kmh)
{
	if (mass_per_turn_g != 0)
	{
		return ( ((float)quota_kg_per_ha * GRAMS_IN_KILOGRAM / SQUARE_METERS_IN_HECTARE) * ((float)seeder_speed_kmh * METERS_IN_KILOMETER / MINUTES_IN_HOUR / SECONDS_IN_MINUTE) * (float)seeder_width_m / (float)mass_per_turn_g * SECONDS_IN_MINUTE );
	}
	return 0;
}

float calculateQuota_fromSpeed(uint32_t seeder_width_m, float seeder_speed_kmh, uint32_t mass_per_turn_g, float motor_speed)
{
	if (seeder_speed_kmh != 0 && seeder_width_m != 0)
	{
		return ( (float)mass_per_turn_g * motor_speed / (seeder_speed_kmh * METERS_IN_KILOMETER / MINUTES_IN_HOUR) * SQUARE_METERS_IN_HECTARE / (float)seeder_width_m / GRAMS_IN_KILOGRAM );
	}
	return 0;
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

void save_user_params_batch(void)
{
	uint32_t flg_pos = (USER_DATA_SAVE_SIZE/2 - 1); // Flag pos in half-words (the last position in the data save)
	
	uint8_t count = 1024/USER_DATA_SAVE_SIZE;
	uint32_t begin_addr = USER_DATA_SAVE_PAGE_ADDR;
	for (uint8_t i = 0; i < count; i++)
	{
		begin_addr = USER_DATA_SAVE_PAGE_ADDR + i * USER_DATA_SAVE_SIZE;
		
		uint16_t save_flg = flash_read16(begin_addr + flg_pos * 2);
		if (save_flg == 0xFFFF)
		{
			break;
		}
		
		if (i == count - 1)
		{
			flash_erase_page(USER_DATA_SAVE_PAGE_ADDR);
			begin_addr = USER_DATA_SAVE_PAGE_ADDR;
		}
	}
	
	for (uint8_t j = 0; j < USER_PARAMS_COUNT; j++)
	{
		flash_save32(begin_addr + j * 4, getUserParameter(j));
	}
	begin_addr += USER_PARAMS_COUNT * 4;
			
	uint32_t area_scaler = 1;
	for (uint8_t j = 0; j < USER_DATA_AREA_SAVE_PRECISION; j++)
	{
		area_scaler *= 10;
	}
	
	uint32_t total_area = round(current_user_area_total * area_scaler), session_area = round(current_user_area_session * area_scaler);
	flash_save32(begin_addr, total_area);
	begin_addr += 4;
	flash_save32(begin_addr, session_area);
	begin_addr += 4;
			
	flash_save16(begin_addr, 1);
	
	return;
}

void restore_user_params_batch(void)
{
	uint32_t flg_pos = (USER_DATA_SAVE_SIZE/2 - 1); // Flag pos in half-words (the last position in the data save)
	
	uint8_t count = 1024/USER_DATA_SAVE_SIZE;
	uint32_t begin_addr = USER_DATA_SAVE_PAGE_ADDR + (count - 1) * USER_DATA_SAVE_SIZE;
	for (uint8_t i = 0; i < count; i++)
	{
	  begin_addr = USER_DATA_SAVE_PAGE_ADDR + i * USER_DATA_SAVE_SIZE;
		uint16_t save_flg = flash_read16(begin_addr + flg_pos * 2);
		if (save_flg == 0xFFFF)
		{
			if (i == 0)
			{
				return; // Completely empty page
			}
			else
			{
				begin_addr = USER_DATA_SAVE_PAGE_ADDR + (i - 1) * USER_DATA_SAVE_SIZE;
			}
			
			break;
		}
	}
	
	for (uint8_t j = 0; j < USER_PARAMS_COUNT; j++)
	{
		setUserParameter(j, flash_read32(begin_addr + j * 4));
	}
	begin_addr += USER_PARAMS_COUNT * 4;
	
	uint32_t area_scaler = 1;
	for (uint8_t j = 0; j < USER_DATA_AREA_SAVE_PRECISION; j++)
	{
		area_scaler *= 10;
	}
	
	uint32_t total_area = flash_read32(begin_addr);
	current_user_area_total = (float)total_area / (float)area_scaler;
	begin_addr += 4;
	uint32_t session_area = flash_read32(begin_addr);
	current_user_area_session = (float)session_area / (float)area_scaler;
	
	return;
}
