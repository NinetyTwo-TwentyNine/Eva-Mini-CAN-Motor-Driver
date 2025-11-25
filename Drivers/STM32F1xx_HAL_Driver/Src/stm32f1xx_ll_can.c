#include "stm32f1xx_ll_can.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

void LL_CAN_Init(uint8_t remap) {
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
	
		if (!remap)
		{
				LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		}
		else
		{
				LL_GPIO_AF_RemapPartial2_CAN1();
				LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
		}
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CAN1);
	
		GPIO_TypeDef * can_port;
		uint32_t tx_pin, rx_pin;
		if (!remap)
		{
				can_port = GPIOA;
				tx_pin = LL_GPIO_PIN_12;
				rx_pin = LL_GPIO_PIN_11;
		}
		else
		{
				can_port = GPIOB;
				tx_pin = LL_GPIO_PIN_9;
				rx_pin = LL_GPIO_PIN_8;
		}
		
    // Configure PA12/PB9 (CAN_TX) as alternate function push-pull
		LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = tx_pin;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		LL_GPIO_Init(can_port, &GPIO_InitStruct);
	
    // Configure PA11/PB8 (CAN_RX) as input floating
		GPIO_InitStruct.Pin = rx_pin;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
		//GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		LL_GPIO_Init(can_port, &GPIO_InitStruct);

    // Reset CAN
    CAN1->MCR |= CAN_MCR_RESET;
    while (CAN1->MCR & CAN_MCR_RESET);
	
		// Enable filter initialization
		CAN1->FMR |= CAN_FMR_FINIT;

		// Use filter bank 0, assign to FIFO0
		CAN1->FA1R &= ~(1 << 0); // Deactivate filter 0
		CAN1->FM1R &= ~(1 << 0); // Identifier-mask mode
		CAN1->FS1R |= (1 << 0);  // 32-bit scale
		CAN1->FFA1R &= ~(1 << 0); // Assign to FIFO0

		// Accept all IDs (Mask = 0)
		CAN1->sFilterRegister[0].FR1 = 0x00000000;
		CAN1->sFilterRegister[0].FR2 = 0x00000000;

		// Activate filter 0
		CAN1->FA1R |= (1 << 0);

		// Exit filter init mode
		CAN1->FMR &= ~CAN_FMR_FINIT;

    // Enter Initialization mode
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0);

    // Set CAN bitrate (example: 500 kbps assuming 36 MHz PCLK1)
		
		CAN1->BTR = (0 << 24)                // SJW = 1 TQ
          | ((11 - 1) << 16)         // BS1 = 11 TQ
          | ((6 - 1) << 20)          // BS2 = 6 TQ
          | ((8 - 1) << 0);          // Prescaler = 4
					
		//CAN1->BTR |= CAN_BTR_LBKM; // Loop back mode
		CAN1->MCR |= CAN_MCR_ABOM; // Additional robustness
		
		CAN1->IER |= CAN_IER_TMEIE | CAN_IER_ERRIE; // interrupts

    // Leave Initialization mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);
		
		// Exit Sleep mode
		CAN1->MCR &= ~CAN_MCR_SLEEP;
		while (CAN1->MSR & CAN_MSR_SLAK) {}
}

uint8_t LL_CAN_Send(uint32_t id, uint8_t* data, uint8_t extended_address) {
    if ((CAN1->TSR & CAN_TSR_TME0) == 0)
        return 0;

    CAN1->TSR |= CAN_TSR_RQCP0; // Clear old flag, just in case
    CAN1->sTxMailBox[0].TIR = 0; // Clear TIR

		if (extended_address)
		{
			id &= 0x1FFFFFFF;
			CAN1->sTxMailBox[0].TIR |= CAN_TI0R_IDE; // Enable ID extension
			CAN1->sTxMailBox[0].TIR |= (id << 3) | CAN_TI0R_IDE;  // 29-bit Extended ID
		}
		else
		{
			id &= 0x000007FF;
			CAN1->sTxMailBox[0].TIR |= (id << 21);  // 11-bit Standard ID
		}
    CAN1->sTxMailBox[0].TDTR = 8;
    CAN1->sTxMailBox[0].TDLR = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    CAN1->sTxMailBox[0].TDHR = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];

    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ; // Request transmission

    return 1;
}
