#include "params_calculation.h"


uint32_t calculateTimeMillis_fromSpeed(uint32_t seeder_speed_kmh, uint32_t seeder_width_m, uint8_t area_divider)
{
	if (seeder_speed_kmh != 0 && seeder_width_m != 0 && area_divider != 0)
	{
		return round( (float)SQUARE_METERS_IN_HECTARE / (float)area_divider / ((float)seeder_speed_kmh * SECONDS_IN_MINUTE * MINUTES_IN_HOUR / METERS_IN_KILOMETER * (float)seeder_width_m) * MILLIS_IN_SECOND );
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
		return ( ((float)quota_kg_per_ha * GRAMS_IN_KILOGRAM / SQUARE_METERS_IN_HECTARE) * ((float)seeder_speed_kmh * SECONDS_IN_MINUTE * MINUTES_IN_HOUR / METERS_IN_KILOMETER) * (float)seeder_width_m / (float)mass_per_turn_g * SECONDS_IN_MINUTE );
	}
	return 0;
}


uint8_t performMotorSpeedCheck(float motor_speed)
{
	uint32_t motor_check = round( motor_speed * 10 );
	return ( motor_check > (MOTOR_SPEED_LIMIT_MIN * 10) && motor_check < (MOTOR_SPEED_LIMIT_MAX * 10) );
}

uint32_t calculateMinMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	for (uint8_t i = 0; i < 100; i++)
	{
		if ( performMotorSpeedCheck( calculateMotorSpeed_fromSpeed(quota_kg_per_ha, seeder_width_m, mass_per_turn_g, i) ) )
		{
			return i;
		}
	}
	return 0;
}

uint32_t calculateMaxMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g)
{
	for (uint8_t i = 0; i < 100; i++)
	{
		if ( performMotorSpeedCheck( calculateMotorSpeed_fromSpeed(quota_kg_per_ha, seeder_width_m, mass_per_turn_g, (100-i-1)) ) )
		{
			return (100-i-1);
		}
	}
	return 0;
}
