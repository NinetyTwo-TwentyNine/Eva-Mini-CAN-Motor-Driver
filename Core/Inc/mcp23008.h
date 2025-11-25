#ifndef _MCP23_H
#define _MCP23_H

#include "main.h"
#include "i2c_control.h"

#define MCP23008_REG_IODIR 0x00
#define MCP23008_REG_IPOL 0x01
#define MCP23008_REG_GPINTEN 0x02
#define MCP23008_REG_DEFVAL 0x03
#define MCP23008_REG_INTCON 0x04
#define MCP23008_REG_IOCON 0x05
#define MCP23008_REG_GPPU 0x06
#define MCP23008_REG_INTF 0x07
#define MCP23008_REG_INTCAP 0x08
#define MCP23008_REG_GPIO 0x09
#define MCP23008_REG_OLAT 0x0A

uint8_t mcp23008_transmission_start(uint8_t read, uint8_t restart);
void mcp23008_transmission_send(uint8_t reg, uint8_t val);
void mcp23008_transmission_read_p1(uint8_t reg);
uint8_t mcp23008_transmission_read_p2(void);
void mcp23008_transmission_stop(void);
uint8_t mcp23008_read_register(uint8_t reg);
void mcp23008_write_register(uint8_t reg, uint8_t val);

void mcp23008_matrix_check(void);

#endif // _MCP23_H
