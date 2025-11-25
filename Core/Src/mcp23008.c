#include "mcp23008.h"

uint8_t mcp23008_transmission_start(uint8_t read, uint8_t restart)
{
	return I2Ctransmission_initialize(MCP23008_I2C, MCP23008_ADDR, read, restart);
}

void mcp23008_transmission_send(uint8_t reg, uint8_t val)
{
	I2Ctransmission_write(MCP23008_I2C, reg);
	I2Ctransmission_write(MCP23008_I2C, val);
}

void mcp23008_transmission_read_p1(uint8_t reg)
{
	I2Ctransmission_write(MCP23008_I2C, reg);
}

uint8_t mcp23008_transmission_read_p2()
{
	return I2Ctransmission_read(MCP23008_I2C);
}

void mcp23008_transmission_stop()
{
	I2Ctransmission_finish(MCP23008_I2C);
}

void mcp23008_write_register(uint8_t reg, uint8_t val)
{
  mcp23008_transmission_start(0, 0);
  mcp23008_transmission_send(reg, val);
  mcp23008_transmission_stop();
}

uint8_t mcp23008_read_register(uint8_t reg) {
	uint8_t value = 0;
  mcp23008_transmission_start(0, 0);
  mcp23008_transmission_read_p1(reg);
	
	mcp23008_transmission_start(1, 1);
	value = mcp23008_transmission_read_p2();
	
  mcp23008_transmission_stop();
  return value;
}


void mcp23008_matrix_check()
{
	if (!mcp23_check_required) { return; }
		
	mcp23_check_result_success = false;
	uint8_t mcp23_check_triggered_reg, mcp23_check_curr_button;

	mcp23_check_triggered_reg = mcp23008_read_register(MCP23008_REG_INTF);
	mcp23_check_result_input = 0;
	for (int i = 0; i < 8; i++)
	{
		if (mcp23_check_triggered_reg & (1 << i))
		{
			mcp23_check_result_input = i;
			break;
		}
	}
	
	if (mcp23_check_result_input < 4 || mcp23_check_result_input > (4 + MCP23008_BUTTON_ROW_COUNT - 1))
	{
		mcp23_check_required = false;
		NVIC_EnableIRQ(EXTI2_IRQn);
		return; // check failed: couldn't find correct input pin
	}
	mcp23_check_result_input -= 4;
	
	
	mcp23008_write_register(MCP23008_REG_GPINTEN, 0x00);
	
	for (mcp23_check_curr_button = 0; mcp23_check_curr_button < MCP23008_BUTTON_ROW_COUNT; mcp23_check_curr_button++)
	{
		mcp23008_write_register(MCP23008_REG_OLAT, 1 << mcp23_check_curr_button);
		
		if (mcp23008_read_register(MCP23008_REG_GPIO) & (1 << (mcp23_check_result_input + 4)))
		{
			mcp23_check_result_output = mcp23_check_curr_button;
			mcp23_check_result_success = true;
			break;
		}
	}
	
	
	mcp23_check_required = false; // check finished
	mcp23008_write_register(MCP23008_REG_OLAT, 0x00);
	mcp23008_write_register(MCP23008_REG_GPINTEN, (uint8_t)MCP23008_PINS_SETUP);
	//NVIC_EnableIRQ(EXTI2_IRQn);
	
	// No need for reading the GPIO register, since we've already done it in the for loop
}
