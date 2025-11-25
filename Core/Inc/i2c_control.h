#ifndef _I2C_C_H
#define _I2C_C_H

#include "main.h"

uint8_t I2Ctransmission_initialize(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t read, uint8_t restart);
void I2Ctransmission_write(I2C_TypeDef *I2Cx, uint8_t data);
uint8_t I2Ctransmission_read(I2C_TypeDef *I2Cx);
uint8_t I2Ctransmission_read_old(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr);
void I2Ctransmission_finish(I2C_TypeDef *I2Cx);

#endif // _I2C_C_H
