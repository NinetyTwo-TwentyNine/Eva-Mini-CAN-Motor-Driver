#include "i2c_control.h"

uint8_t I2Ctransmission_initialize(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t read, uint8_t restart) {
    // Wait until I2C is not busy (if it's a START, and not a RESTART)
    while (LL_I2C_IsActiveFlag_BUSY(I2Cx) && !restart);
    // Generate START condition
    LL_I2C_GenerateStartCondition(I2Cx);
    // Wait for START bit to be sent (SB)
    while (!LL_I2C_IsActiveFlag_SB(I2Cx));
    // Send slave address with write bit (0)
    LL_I2C_TransmitData8(I2Cx, (addr << 1) | (read ? 1 : 0));
    // Wait for address to be acknowledged and check for NACK
    while (!(LL_I2C_IsActiveFlag_ADDR(I2Cx) || LL_I2C_IsActiveFlag_AF(I2Cx)));
		if (LL_I2C_IsActiveFlag_AF(I2Cx)) {
			LL_I2C_ClearFlag_AF(I2Cx);
			LL_I2C_GenerateStopCondition(I2Cx);
			return false;
		}
    LL_I2C_ClearFlag_ADDR(I2Cx);  // Clear ADDR by reading SR1 then SR2
		return true;
}

void I2Ctransmission_write(I2C_TypeDef *I2Cx, uint8_t data) {
    // Wait until data register is empty
    while (!(LL_I2C_IsActiveFlag_TXE(I2Cx) || LL_I2C_IsActiveFlag_AF(I2Cx)));
		if (LL_I2C_IsActiveFlag_AF(I2Cx)) {
			LL_I2C_ClearFlag_AF(I2Cx);
			LL_I2C_GenerateStopCondition(I2Cx);
			return;
		}
    LL_I2C_TransmitData8(I2Cx, data);
		
		// Wait for transfer finished (BTF = byte transfer finished)
    while (!LL_I2C_IsActiveFlag_BTF(I2Cx));
}

uint8_t I2Ctransmission_read(I2C_TypeDef *I2Cx) {
    // Single-byte read: Must disable ACK before clearing ADDR!
    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);

    // Wait for RXNE (data received)
    while (!LL_I2C_IsActiveFlag_RXNE(I2Cx));

    uint8_t value = LL_I2C_ReceiveData8(I2Cx);
    return value;
}

void I2Ctransmission_finish(I2C_TypeDef *I2Cx) {
    // Generate STOP condition
    LL_I2C_GenerateStopCondition(I2Cx);
		
		//while (!LL_I2C_IsActiveFlag_STOP(I2Cx)); // Wait until STOPF is set (waiting's only required for slaves)
		LL_I2C_ClearFlag_STOP(I2Cx);
}
