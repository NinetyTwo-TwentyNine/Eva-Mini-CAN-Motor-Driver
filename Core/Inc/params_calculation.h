#ifndef _PARAMS_CALC_H
#define _PARAMS_CALC_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>


#define KILO_MILLI_TRANSLATION 1000
#define SECONDS_MINUTES_TRANSLATION 60

#define GRAMS_IN_KILOGRAM KILO_MILLI_TRANSLATION
#define METERS_IN_KILOMETER KILO_MILLI_TRANSLATION
#define MILLIS_IN_SECOND KILO_MILLI_TRANSLATION
#define SECONDS_IN_MINUTE SECONDS_MINUTES_TRANSLATION
#define MINUTES_IN_HOUR SECONDS_MINUTES_TRANSLATION
#define SQUARE_METERS_IN_HECTARE 10000

#define MOTOR_SPEED_LIMIT_MIN 0
#define MOTOR_SPEED_LIMIT_MAX 256


uint32_t calculateTimeMillis_fromSpeed(uint32_t seeder_speed_kmh, uint32_t seeder_width_m, uint8_t area_divider);
float calculateMotorSpeed_fromTime(uint32_t quota_kg_per_ha, uint8_t area_divider, uint32_t mass_per_turn_g, uint32_t time_total_millis);
float calculateMotorSpeed_fromSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g, uint32_t seeder_speed_kmh);

uint8_t performMotorSpeedCheck(float motor_speed);
uint32_t calculateMinMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g);
uint32_t calculateMaxMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g);

#endif // _PARAMS_CALC_H
