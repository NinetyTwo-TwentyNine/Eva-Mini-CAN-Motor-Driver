#ifndef _CAN_PARSER_H
#define _CAN_PARSER_H

#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parseMotorSpeed(uint8_t direction, uint8_t fractional_part, uint8_t rotation_speed, uint8_t output[2]);
void setMotorControl(uint8_t MotorSpeed_1[2], uint32_t AdressSend_1, uint8_t result[8]);

#endif // _CAN_PARSER_H
