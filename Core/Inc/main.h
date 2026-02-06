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
#include <math.h>

#include "stm32f1xx_ll_flash.h" // Our own LL Flash driver replacement for STM32F103
#include "stm32f1xx_ll_can.h" // Our own LL CAN driver replacement for STM32F103

#include "flash_control.h"
#include "i2c_control.h"
#include "GFX_SSD1306.h"
#include "mcp23008.h"
#include "can_parser.h"
#include "frequency_calc_basic.h"
#include "user_params_management.h"

#include "ui_screen_system.h"

#include "ui_screen_start_menu.h"

#include "ui_screen_main_menu.h"
#include "ui_screen_seeder_options_menu.h"
#include "ui_screen_errors_menu.h"
#include "ui_screen_other_options_menu.h"

#include "ui_screen_calibration_menu.h"
#include "ui_screen_calibration_check_menu.h"

#include "ui_screen_fan_sensor_menu.h"
#include "ui_screen_speed_sensor_menu.h"
#include "ui_screen_progress_menu.h"


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

#define IC_ARRAY32_SIZE 3
#define IC_ARRAY32_POS_VAL1 0
#define IC_ARRAY32_POS_VAL2 1
#define IC_ARRAY32_POS_DIFF 2

#define IC_ARRAY8_SIZE 4
#define IC_ARRAY8_POS_CAPTURE_INITIAL 0
#define IC_ARRAY8_POS_CAPTURE_COMPLETE 1
#define IC_ARRAY8_POS_CAPTURE_ERROR 2
#define IC_ARRAY8_POS_OVERFLOW_COUNT 3

extern volatile uint32_t IC_Array32[3][SENSOR_COUNT_MAX];
extern volatile uint8_t IC_Array8[4][SENSOR_COUNT_MAX];


#define SENSADDR_POS_FAN 0
#define SENSADDR_POS_MOTOR 1
#define SENSADDR_POS_SPEED 2
#define SENSADDR_POS_SEEDER 3
#define SENSADDR_POS_BUNKER 4

#define SENSADDR_MASK_TIMS ((1 << SENSADDR_POS_FAN) | (1 << SENSADDR_POS_MOTOR) | (1 << SENSADDR_POS_SPEED))
#define SENSADDR_MASK_GPIOS ((1 << SENSADDR_POS_SEEDER) | (1 << SENSADDR_POS_BUNKER))

extern SENSADDR_TIM_TypeDef* sensor_address_timer[SENSOR_COUNT_MAX];
extern SENSADDR_GPIO_TypeDef* sensor_address_gpio[SENSOR_COUNT_MAX];
extern uint64_t sensor_last_check_time[SENSOR_COUNT_MAX];
extern float sensor_frequency[SENSOR_COUNT_MAX];

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
#define MCP23008_BUTTON_CHECK_TIME 175
#define MCP23008_DEBOUNCE_WAIT_TIME 20

#define MCP23008_BUTTON_ROW_COUNT 3
#define MCP23008_PINS_SETUP 0x70

extern uint8_t mcp23_check_required, mcp23_check_allowed, mcp23_check_result_output, mcp23_check_result_input, mcp23_check_result_success;
extern uint64_t mcp23_last_check_time;


#define MATRIX_POS_BUTTON_UP 01
#define MATRIX_POS_BUTTON_DOWN 21
#define MATRIX_POS_BUTTON_MENU 00
#define MATRIX_POS_BUTTON_OK 11
#define MATRIX_POS_BUTTON_POWER 10

// CAN
#define CAN_TRANSMISSION_INTERVAL 100

extern uint8_t can_last_send_success, can_should_stop_motor;
extern uint64_t can_last_send_time, can_test_initialization_time;

// UI/Logic
#define UI_UPDATE_MAX_FREQUENCY 50
#define UI_MAIN_TEXT_SIZE 1
#define UI_MAIN_COLOR_INVERTED 1

extern UI_Screen main_screen;
extern uint8_t ui_update_required, main_ui_on;
extern uint8_t switch_to_start_menu_allowed, currently_on_start_menu;
extern uint64_t ui_last_update_time, ui_last_callback_time;


#define SENSOR_POWER_PORT GPIOC
#define SENSOR_POWER_PIN LL_GPIO_PIN_14

#define MOTOR_CAN_ID 0x16000001
#define MOTOR_SPEED_LIMIT_MIN 0
#define MOTOR_SPEED_LIMIT_MAX 200
#define MOTOR_DEFAULT_SPEED_EMPTY 30
#define MOTOR_TURN_DIRECTION 0x00

#define SENSOR_VALUE_FAN_MIN_RANGE 20
#define SENSOR_VALUE_SPEED_BORDER_MIN 0.5
#define SENSOR_VALUE_MOTOR_ALLOWED_DEVIATION 10
#define SENSOR_VALUE_QUOTA_ALLOWED_DEVIATION 3
#define SENSOR_VALUE_LOST_TIME 1500

#define SENSOR_TYPE_BUNKER_NPN false
#define SENSOR_TYPE_SEEDER_NPN false

#define MAIN_LOGIC_TICK_TIME 10

typedef enum
{
	LSTATE_NONE,
	LSTATE_CAN_TEST,
	LSTATE_CAN_PROCEDURE,
	LSTATE_MAIN_LOGIC,
} Logic_State_Type;

extern Logic_State_Type curr_logic_state;
extern uint64_t main_logic_last_tick_time;

void setCurrentLogicState(Logic_State_Type new_state);


#define USER_PARAMS_COUNT 12
#define USER_PARAM_SPEED_MIN 0
#define USER_PARAM_SPEED_MAX 1
#define USER_PARAM_FAN_SPEED_MIN 2
#define USER_PARAM_FAN_SPEED_MAX 3
#define USER_PARAM_FAN_PULSES 4
#define USER_PARAM_WHEEL_DIAMETER 5
#define USER_PARAM_WHEEL_PULSES 6
#define USER_PARAM_SEEDER_WIDTH 7
#define USER_PARAM_QUOTA 8
#define USER_PARAM_MASS_PER_TURN 9
#define USER_PARAM_AREA_SESSION 10
#define USER_PARAM_AREA_TOTAL 11

#define USER_PARAMS_FLOAT_MASK ((1 << USER_PARAM_SPEED_MIN) | (1 << USER_PARAM_SPEED_MAX) | (1 << USER_PARAM_AREA_SESSION) | (1 << USER_PARAM_AREA_TOTAL))

#define USER_DATA_SAVE_PAGE_ADDR 0x0800FC00
#define USER_DATA_SAVE_SIZE ((USER_PARAMS_COUNT + 1)*4)
#define USER_DATA_SAVE_INTERAVAL 300000 // 5 minutes

extern uint8_t user_params_differentiate;
extern uint32_t user_params_array[USER_PARAMS_COUNT];
extern uint64_t user_params_last_save_time;

extern uint8_t current_state_seeder_down, current_state_bunker_full;
extern float current_can_motor_speed, current_actual_motor_speed, current_fan_speed, current_seeder_speed, current_quota;


#define BUZZER_PORT GPIOA
#define BUZZER_PIN LL_GPIO_PIN_9

#define ERROR_COUNT_TOTAL 6
#define ERROR_TYPE_FAN 0
#define ERROR_TYPE_MOTOR 1
#define ERROR_TYPE_CAN 2
#define ERROR_TYPE_SPEED 3
#define ERROR_TYPE_QUOTA 4
#define ERROR_TYPE_EMPTY 5

#define ERROR_DETERMINATION_TIME 1000
#define ERROR_RESET_ALLOW_TIME 500
#define ERROR_NOTIFICATION_INTERVAL_TIME 400
#define ERROR_NOTIFICATION_BEEP_TIME 200
#define ERROR_NOTIFICATION_BEEP_COUNT 4

#define ERROR_STATE_ARRAY_COUNT 4
#define ERROR_STATE_PREACTIVE 0
#define ERROR_STATE_ACTIVE 1
#define ERROR_NOTIFICATION_COMPLETE 2
#define ERROR_NOTIFICATION_IN_PROGRESS 3

extern uint8_t error_notification_beep_counter[ERROR_COUNT_TOTAL];
extern uint8_t error_state_array[ERROR_STATE_ARRAY_COUNT][ERROR_COUNT_TOTAL];
extern uint64_t error_last_activation_change[ERROR_COUNT_TOTAL], error_notification_start_end[ERROR_COUNT_TOTAL], error_on_time[ERROR_COUNT_TOTAL];

void updateErrorState(uint8_t state_pos, uint8_t error_pos, uint8_t value);

// Resources
#define LOGO_ERROR_ALERT_SIZE 20
#define LOGO_ERROR_ALERT_HEIGHT 10
#define LOGO_ERROR_ALERT_WIDTH 9

#define LOGO_QUESTION_MARK_SIZE 20
#define LOGO_QUESTION_MARK_HEIGHT 10
#define LOGO_QUESTION_MARK_WIDTH 9

#define LOGO_OK_MARK_SIZE 20
#define LOGO_OK_MARK_HEIGHT 10
#define LOGO_OK_MARK_WIDTH 9

#define LOGO_SEEDER_STATE_SIZE 9
#define LOGO_SEEDER_STATE_HEIGHT 9
#define LOGO_SEEDER_STATE_WIDTH 8

extern uint8_t logo_error_alert[LOGO_ERROR_ALERT_SIZE];
extern uint8_t logo_question_mark[LOGO_QUESTION_MARK_SIZE];
extern uint8_t logo_ok_mark[LOGO_OK_MARK_SIZE];
extern uint8_t logo_seeder_state[LOGO_SEEDER_STATE_SIZE];

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
