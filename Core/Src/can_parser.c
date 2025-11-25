#include "can_parser.h"

void parseMotorSpeed(uint8_t direction, uint8_t fractional_part, uint8_t rotation_speed, uint8_t output[2]) {
    uint16_t buf_speed[2];
    char hex_speed[4][8];
    char har_str[8];
	
    // Calculate speed based on rotation direction
    if (direction == 0xFF) {
        buf_speed[0] = (uint16_t)rotation_speed;
        buf_speed[1] = 4096 - buf_speed[0] * 10 + fractional_part;
    } else if (direction == 0x00) {
        buf_speed[0] = (uint16_t)rotation_speed;
        buf_speed[1] = buf_speed[0] * 10;
    }

		// Convert speed to hex string
		sprintf(har_str, "%X", buf_speed[1]);

		// Normalize to 4-character hex string
		switch(strlen(har_str))
		{
			case 1:
				sprintf(hex_speed[0], "00%s", har_str);
				sprintf(hex_speed[1], "%s0", hex_speed[0]);
				break;
			case 2:
				sprintf(hex_speed[0], "0%s", har_str);
				sprintf(hex_speed[1], "%s0", hex_speed[0]);
				break;
			case 3:
				sprintf(hex_speed[1], "%s0", har_str);
				break;
			case 4:
				strcpy(hex_speed[1], har_str);
				break;
		}

    // Split and convert final hex values
    memmove(&hex_speed[2][0], &hex_speed[1][0], 2); // get 2 chars from pos 1
    hex_speed[2][2] = '\0';
		
    strcpy(hex_speed[3], hex_speed[1]);
    memmove(&hex_speed[3][0], &hex_speed[3][2], strlen(hex_speed[3]) - 1); // delete 2 chars at pos 1

    // Convert hex strings to bytes
    output[0] = (uint8_t)strtol(hex_speed[2], NULL, 16);
    output[1] = (uint8_t)strtol(hex_speed[3], NULL, 16);
}

void setMotorControl(uint8_t MotorSpeed_1[2], uint32_t AddressSend_1, uint8_t result[8]) {
    uint8_t CAN1_Tx1700_au8[8] = {0};
    char buf[10];
    
    // Turn address into an array
		uint8_t address_result[4] = {AddressSend_1 >> 24, (AddressSend_1 >> 16) % 256, (AddressSend_1 >> 8) % 256, AddressSend_1 % 256};

    // Copy speed
    uint8_t speedtestinput[2] = { MotorSpeed_1[0], MotorSpeed_1[1] };

    // Convert to hex string
    sprintf(buf, "%02X%02X", speedtestinput[0], speedtestinput[1]);
    
    // Extract substrings
    char bufspeeddeldr1[5];
    snprintf(bufspeeddeldr1, sizeof(bufspeeddeldr1), "0%s", buf + 2);

    // Get bytes from substring
    uint8_t testresultdr[2];
    sscanf(bufspeeddeldr1, "0%1hhX%1hhX", &testresultdr[0], &testresultdr[1]);

    // Decide which speed to send
    uint8_t testresultsend[2];
    if (address_result[3] % 2 == 0) {
        testresultsend[0] = speedtestinput[0];
        testresultsend[1] = speedtestinput[1];
    } else {
        testresultsend[0] = testresultdr[0];
        testresultsend[1] = testresultdr[1];
    }

    // Set CAN payload based on address
    switch (AddressSend_1) {
      case 24117248:
        CAN1_Tx1700_au8[0] = 0x11;
        CAN1_Tx1700_au8[1] = 0x00;
        CAN1_Tx1700_au8[2] = testresultsend[0];
        CAN1_Tx1700_au8[3] = testresultsend[1];
        break;
      case 24117249:
        CAN1_Tx1700_au8[0] = 0x12;
        CAN1_Tx1700_au8[3] = testresultsend[0];
        CAN1_Tx1700_au8[4] = testresultsend[1];
        break;
      case 24117250:
        CAN1_Tx1700_au8[0] = 0x14;
        CAN1_Tx1700_au8[5] = testresultsend[0];
        CAN1_Tx1700_au8[6] = testresultsend[1];
        break;
      case 24117251:
        CAN1_Tx1700_au8[0] = 0x18;
        CAN1_Tx1700_au8[6] = testresultsend[0];
        CAN1_Tx1700_au8[7] = testresultsend[1];
        break;
      default:
        // fallback for unrecognized address
        break;
    }

    // Return CAN frame
    memcpy(result, CAN1_Tx1700_au8, 8);
}
