#ifndef _PARAMS_MNGM_H
#define _PARAMS_MNGM_H

#include "main.h"


#define KILO_MILLI_TRANSLATION 1000
#define DECI_DECA_TRANSLATION 10
#define SECONDS_MINUTES_TRANSLATION 60

#define GRAMS_IN_KILOGRAM KILO_MILLI_TRANSLATION
#define DECIMETERS_IN_METER DECI_DECA_TRANSLATION
#define METERS_IN_KILOMETER KILO_MILLI_TRANSLATION
#define MILLIS_IN_SECOND KILO_MILLI_TRANSLATION
#define SECONDS_IN_MINUTE SECONDS_MINUTES_TRANSLATION
#define MINUTES_IN_HOUR SECONDS_MINUTES_TRANSLATION
#define SQUARE_METERS_IN_HECTARE 10000


void setUserParameter(uint8_t pos, uint32_t parameter);
uint32_t getUserParameter(uint8_t pos);
uint8_t checkIfAllUserParamsAreSet(void);


uint32_t calculateTimeMillis_fromArea(uint32_t seeder_speed_kmh, uint32_t seeder_width_m, uint8_t area_divider);
float calculateSeederSpeed_fromSensorOutput(uint32_t wheel_diameter_dm, uint32_t pulses_per_wheel_count, float sensor_frequency_hz);
float calculateMotorSpeed_fromTime(uint32_t quota_kg_per_ha, uint8_t area_divider, uint32_t mass_per_turn_g, uint32_t time_total_millis);
float calculateMotorSpeed_fromSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g, float seeder_speed_kmh);
float calculateQuota_fromSpeed(uint32_t seeder_width_m, float seeder_speed_kmh, uint32_t mass_per_turn_g, float motor_speed_turns_per_min);
float calculateAreaAddition_fromSpeed(float begin_speed_kmh, float end_speed_kmh, uint32_t time_millis, uint32_t seeder_width_m);

uint8_t performMotorSpeedCheck(float motor_speed);
uint32_t calculateMinMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g);
uint32_t calculateMaxMotorSpeed(uint32_t quota_kg_per_ha, uint32_t seeder_width_m, uint32_t mass_per_turn_g);


void save_user_params_batch(void);
void restore_user_params_batch(void);


#endif // _PARAMS_MNGM_H
