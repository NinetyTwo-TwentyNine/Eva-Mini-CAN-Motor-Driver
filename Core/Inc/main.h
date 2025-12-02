/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdbool.h>

#include "stm32f1xx_ll_can.h" // Our own LL CAN driver replacement for STM32F103

#include "frequency_calc_basic.h"
#include "i2c_control.h"
#include "GFX_SSD1306.h"
#include "mcp23008.h"
#include "can_parser.h"

#include "ui_screen_system.h"
#include "ui_screen_start_menu.h"
#include "ui_screen_main_menu.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

// General utilities

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define swap(a, b) { int16_t t = a; a = b; b = t; }

extern volatile uint64_t sys_timer;

// Frequency calculation
#define SENSOR_COUNT_MAX 8

#define IC_ARRAY32_POS_VAL1 0
#define IC_ARRAY32_POS_VAL2 1
#define IC_ARRAY32_POS_DIFF 2
#define IC_ARRAY32_POS_TICK 3

#define IC_ARRAY8_POS_CAPTURE_INITIAL 0
#define IC_ARRAY8_POS_CAPTURE_COMPLETE 1
#define IC_ARRAY8_POS_CAPTURE_ERROR 2
#define IC_ARRAY8_POS_OVERFLOW_COUNT 3

extern volatile uint32_t IC_Array32[4][SENSOR_COUNT_MAX];
extern volatile uint8_t IC_Array8[4][SENSOR_COUNT_MAX];


#define SENSADDR_POS_FAN 0
#define SENSADDR_POS_MOTOR 1
#define SENSADDR_POS_SPEED 2
#define SENSADDR_POS_SEEDER 3
#define SENSADDR_POS_BUNKER 4

extern SENSADDR_TypeDef* sensor_address[SENSOR_COUNT_MAX];
extern float sensor_frequency[SENSOR_COUNT_MAX];
extern uint64_t sensor_last_check_time[SENSOR_COUNT_MAX];

// SSD1306
#define SCREEN_SPI SPI1
#define SCREEN_I2C I2C1

#define SCREEN_USE_SPI true
#define SCREEN_SPI_HARDWARE false

#define CS_PORT GPIOB
#define RST_PORT GPIOB
#define DC_PORT GPIOA
#define SCLK_PORT GPIOB
#define SID_PORT GPIOB

#define CS_PIN LL_GPIO_PIN_12
#define RST_PIN LL_GPIO_PIN_5
#define DC_PIN LL_GPIO_PIN_8
#define SCLK_PIN LL_GPIO_PIN_6
#define SID_PIN LL_GPIO_PIN_7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// MCP23008
#define MCP23008_ADDR 0x27
#define MCP23008_I2C I2C2
#define MCP23008_BUTTON_CHECK_TIME 160
#define MCP23008_DEBOUNCE_WAIT_TIME 20

#define MCP23008_BUTTON_ROW_COUNT 3
#define MCP23008_PINS_SETUP 0x70

extern uint64_t mcp23_check_last_time;
extern uint8_t mcp23_check_required, mcp23_check_allowed, mcp23_check_result_output, mcp23_check_result_input, mcp23_check_result_success;


#define MATRIX_POS_BUTTON_DOWN 00
#define MATRIX_POS_BUTTON_UP 01
#define MATRIX_POS_BUTTON_MENU 20
#define MATRIX_POS_BUTTON_OK 21
#define MATRIX_POS_BUTTON_POWER 11

// CAN
extern uint8_t can_last_send_success;
extern uint64_t can_last_send_time;

// UI/Logic
#define UI_UPDATE_MAX_FREQUENCY 50
#define UI_MAIN_TEXT_SIZE 1
#define UI_MAIN_COLOR_INVERTED 1

extern UI_Screen main_screen;
extern uint64_t ui_last_update_time;
extern uint8_t ui_update_required, main_ui_on;

extern uint8_t switch_to_start_menu_allowed, can_procedure_in_progress, main_functionality_active;


#define ERROR_COUNT_TOTAL 5
#define ERROR_TYPE_FAN 0
#define ERROR_TYPE_MOTOR 1
#define ERROR_TYPE_CAN 2
#define ERROR_TYPE_SPEED 3
#define ERROR_TYPE_QUOTA 4

extern uint8_t error_active[ERROR_COUNT_TOTAL];
extern uint64_t error_last_activated[ERROR_COUNT_TOTAL];

// Resources
#define LOGO_ERROR_ALERT_SIZE 20
#define LOGO_ERROR_ALERT_HEIGHT 10
#define LOGO_ERROR_ALERT_WIDTH 9

#define LOGO_SEEDER_STATE_SIZE 9
#define LOGO_SEEDER_STATE_HEIGHT 9
#define LOGO_SEEDER_STATE_WIDTH 8

extern uint8_t logo_error_alert[LOGO_ERROR_ALERT_SIZE];
extern uint8_t logo_seeder_state[LOGO_SEEDER_STATE_SIZE];

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
