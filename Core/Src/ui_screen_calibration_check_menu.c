#include <ui_screen_calibration_check_menu.h>

#define CCM_ELEMENT_TEXT_COUNT 13
#define CCM_POS_SPEED_ITEM 0
#define CCM_POS_AREA_ITEM 1
#define CCM_POS_MASS_PER_TURN_ITEM 2
#define CCM_POS_BEGIN_CHECK 3
#define CCM_POS_TIME_ITEM 4
#define CCM_POS_STOP_CHECK 5
#define CCM_POS_COUNTED_MASS_ITEM 6
#define CCM_POS_ACTUAL_MASS_ITEM 7
#define CCM_POS_COUNT_PARAMS 8
#define CCM_POS_DEVIATION_ITEM 9
#define CCM_POS_NEW_MASS_ITEM 10
#define CCM_POS_APPLY 11
#define CCM_POS_BACK 12

static char* labels[CCM_ELEMENT_TEXT_COUNT] = { "Скорость:", "Площадь:1/", "Масса:", "Начать проверку", "Время:", "Стоп", "Рас.масса:", "Масса:", "Рассчитать массу", "Отклонение:", "НМасса:", "Применить", "назад" };
static uint8_t text_xpos[CCM_ELEMENT_TEXT_COUNT] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t text_ypos[CCM_ELEMENT_TEXT_COUNT] = { 4, 20, 36, 52, 68, 84, 100, 116, 132, 148, 164, 180, 196 };
static uint8_t text_offset_scalers[CCM_ELEMENT_TEXT_COUNT] = { 6, 4, 10, 0, 11, 0, 9, 9, 0, 7, 10, 0, 0 };
static const uint8_t speed_item_id = 1, area_item_id = 2, mass_per_turn_item_id = 3, begin_check_id = 4, time_item_id = 5, stop_check_id = 6, counted_mass_item_id = 7, actual_mass_item_id = 8, count_params_id = 9, deviation_item_id = 10, new_mass_item_id = 11, apply_id = 12, back_id = 13;
static uint8_t label_ids[CCM_ELEMENT_TEXT_COUNT] = { speed_item_id, area_item_id, mass_per_turn_item_id, begin_check_id, time_item_id, stop_check_id, counted_mass_item_id, actual_mass_item_id, count_params_id, deviation_item_id, new_mass_item_id, apply_id, back_id };
static uint8_t label_tab_ids[CCM_ELEMENT_TEXT_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

#define CCM_ELEMENT_VAL_COUNT 10
#define CCM_POS_SPEED_VAL 0
#define CCM_POS_AREA_DIVIDER_VAL 1
#define CCM_POS_MASS_PER_TURN_VAL 2
#define CCM_POS_TIME_HOUR_VAL 3
#define CCM_POS_TIME_MIN_VAL 4
#define CCM_POS_TIME_SEC_VAL 5
#define CCM_POS_COUNTED_MASS_VAL 6
#define CCM_POS_ACTUAL_MASS_VAL 7
#define CCM_POS_DEVIATION_VAL 8
#define CCM_POS_NEW_MASS_VAL 9

static uint32_t speed_val, area_divider_val, mass_per_turn_val, time_hour_val, time_min_val, time_sec_val, counted_mass_val, actual_mass_val, deviation_percent_val, new_mass_val;
static uint32_t* val_ptrs[CCM_ELEMENT_VAL_COUNT] = { &speed_val, &area_divider_val, &mass_per_turn_val, &time_hour_val, &time_min_val, &time_sec_val, &counted_mass_val, &actual_mass_val, &deviation_percent_val, &new_mass_val };
static char* val_types[CCM_ELEMENT_VAL_COUNT] = { "км/ч", "га", "кг/об", "ч", "мин", "с", "кг", "кг", "%", "кг/га" };
static uint8_t val_xpos[CCM_ELEMENT_VAL_COUNT] = { 58, 64, 40, 40, 58, 88, 64, 40, 70, 46 };
static uint8_t val_ypos[CCM_ELEMENT_VAL_COUNT] = { 4, 20, 36, 68, 68, 68, 100, 116, 148, 164 };
static uint8_t val_allowed_lengths[CCM_ELEMENT_VAL_COUNT] = { 2, 1, 4, 2, 2, 2, 6, 6, 5, 4 };
static uint8_t val_lengths_after_dot[CCM_ELEMENT_VAL_COUNT] = { 0, 0, 3, 0, 0, 0, 3, 3, 2, 3 };
static const uint8_t speed_val_id = 21, area_divider_val_id = 22, mass_per_turn_val_id = 23, time_hour_val_id = 24, time_min_val_id = 25, time_sec_val_id = 26, counted_mass_val_id = 27, actual_mass_val_id = 28, deviation_percent_val_id = 29, new_mass_val_id = 30;
static uint8_t val_ids[CCM_ELEMENT_VAL_COUNT] = { speed_val_id, area_divider_val_id, mass_per_turn_val_id, time_hour_val_id, time_min_val_id, time_sec_val_id, counted_mass_val_id, actual_mass_val_id, deviation_percent_val_id, new_mass_val_id };


static uint8_t possible_area_divider_vals[] = {1, 2, 4, 8, 16}, selected_area_divider_pos;
static uint8_t selected_slot, selected_slot_gone;

static uint32_t initial_time_total, total_motor_movement_time;
static uint64_t time_save_ui;
static const uint16_t slot_blink_period = 400;

static int16_t CCMHelper_GetValPosFromItemId(uint8_t item_id);
static uint8_t CCMHelper_CheckValPosValidity(int16_t item_id);
static void CCMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos);
static void CCMHelper_UpdateValStr_SlotBlink(UI_Screen* screen, uint8_t slot_gone);
static UI_Element_Visual* CCMHelper_SetElementFunctionality(UI_Screen* screen, uint8_t item_pos, uint8_t tab_index_on, uint8_t functionality_on);
static void CCMHelper_SetElementFunctionality_Array(UI_Screen* screen, uint8_t* elem_array, uint8_t elem_count, uint8_t tab_index_on, uint8_t functionality_on);
static void CCMHelper_CalculateRequiredTime(void);

static void CalibrationCheckMenu_ScreenCallback(UI_Screen* screen);
static void CalibrationCheckMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
static void CalibrationCheckMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
static void CalibrationCheckMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);

//==================================
// Helpers
//==================================

static int16_t CCMHelper_GetValPosFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case speed_item_id: val_pos = CCM_POS_SPEED_VAL; break;
		case area_item_id: val_pos = CCM_POS_AREA_DIVIDER_VAL; break;
		case mass_per_turn_item_id: val_pos = CCM_POS_MASS_PER_TURN_VAL; break;
		case time_item_id: val_pos = CCM_POS_TIME_SEC_VAL; break;
		case counted_mass_item_id: val_pos = CCM_POS_COUNTED_MASS_VAL; break;
		case actual_mass_item_id: val_pos = CCM_POS_ACTUAL_MASS_VAL; break;
		case deviation_item_id: val_pos = CCM_POS_DEVIATION_VAL; break;
		case new_mass_val_id: val_pos = CCM_POS_NEW_MASS_VAL; break;
	}
	return val_pos;
}

static uint8_t CCMHelper_CheckValPosValidity(int16_t val_pos)
{
	if ( (val_pos >= CCM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return 0;
	
	return 1;
}

static void CCMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	if (!CCMHelper_CheckValPosValidity(val_pos)) return;
				
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos], length_after_dot = val_lengths_after_dot[val_pos];
	uint32_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	
	if (val_pos == CCM_POS_AREA_DIVIDER_VAL)
	{
		sprintf(final_string, "%d", value);
		if (selected_slot_gone)
		{
			for (uint8_t i = 0; i < strlen(final_string); i++)
			{
				final_string[i] = ' ';
			}
		}
		strcat(final_string, type);
	}
	else
	{
		utils_val_to_text_converter(final_string, length, length_after_dot, value, type, selected_slot, selected_slot_gone);
	}
	
	ui_editText(e, final_string, e->data.text.font);
}

static void CCMHelper_UpdateValStr_SlotBlink(UI_Screen* screen, uint8_t slot_gone)
{
	time_save_ui = sys_timer;
	selected_slot_gone = slot_gone;
	
	CCMHelper_ConvertValToText(screen, CCMHelper_GetValPosFromItemId(screen->hovered->id));
}

static UI_Element_Visual* CCMHelper_SetElementFunctionality(UI_Screen* screen, uint8_t item_pos, uint8_t tab_index_on, uint8_t functionality_on)
{
		if ( item_pos >= CCM_ELEMENT_TEXT_COUNT ) return NULL;
		
		UI_Element_Visual *e = ui_findVisualById(screen, label_ids[item_pos]);
		if (e == NULL) return NULL;
		
		(tab_index_on) ? (e->tab_index = label_tab_ids[item_pos]) : (e->tab_index = 0);
		
		UI_Callback main = NULL, selection = NULL, onselect = NULL;
		if (functionality_on)
		{
			main = CalibrationCheckMenu_OnItemPressed_Main;
			int16_t val_pos = CCMHelper_GetValPosFromItemId(label_ids[item_pos]);
			if (CCMHelper_CheckValPosValidity(val_pos))
			{
				selection = CalibrationCheckMenu_OnItemPressed_Selection;
				onselect = CalibrationCheckMenu_OnItemPressed_OnSelect;
			}
		}
		
		ui_bindInteractable(screen, e, main, selection, onselect);
		return e;
}
static void CCMHelper_SetElementFunctionality_Array(UI_Screen* screen, uint8_t* elem_array, uint8_t elem_count, uint8_t tab_index_on, uint8_t functionality_on)
{
	for (uint8_t i = 0; i < elem_count; i++)
	{
		uint8_t item_pos = elem_array[i];
		CCMHelper_SetElementFunctionality(screen, item_pos, tab_index_on, functionality_on);
	}
}

static void CCMHelper_CalculateRequiredTime()
{
	initial_time_total = calculateTimeMillis_fromArea(speed_val, getUserParameterInt(USER_PARAM_SEEDER_WIDTH), area_divider_val);
	uint32_t time_copy = initial_time_total / MILLIS_IN_SECOND;
	
	time_hour_val = time_copy / SECONDS_IN_MINUTE / MINUTES_IN_HOUR;
	time_copy -= time_hour_val * SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
	
	time_min_val = time_copy / SECONDS_IN_MINUTE;
	time_copy -= time_min_val * SECONDS_IN_MINUTE;
	
	time_sec_val = time_copy;
}

//==================================
// Main functionality
//==================================

static void CalibrationCheckMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save_ui > slot_blink_period)
	{
		CCMHelper_UpdateValStr_SlotBlink(screen, !selected_slot_gone);
		ui_update_required = true;
	}
	
	if (curr_logic_state == LSTATE_CAN_PROCEDURE)
	{
		total_motor_movement_time += screen->callback_interval;
		counted_mass_val = round( (float)mass_per_turn_val * (current_can_motor_speed * (float)total_motor_movement_time / SECONDS_IN_MINUTE / MILLIS_IN_SECOND) );
		CCMHelper_ConvertValToText(screen, CCM_POS_COUNTED_MASS_VAL);
		
		uint32_t time_left = (initial_time_total > total_motor_movement_time) ? (initial_time_total - total_motor_movement_time) : 0;
		if (total_motor_movement_time % 1000 == 0)
		{
			if (time_sec_val != 0 || time_min_val != 0 || time_hour_val != 0)
			{
				uint32_t time_copy = time_left / MILLIS_IN_SECOND;
				
				time_hour_val = time_copy / SECONDS_IN_MINUTE / MINUTES_IN_HOUR;
				time_copy -= time_hour_val * SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
				
				time_min_val = time_copy / SECONDS_IN_MINUTE;
				time_copy -= time_min_val * SECONDS_IN_MINUTE;
				
				time_sec_val = time_copy;
			}
			CCMHelper_ConvertValToText(screen, CCM_POS_TIME_HOUR_VAL);
			CCMHelper_ConvertValToText(screen, CCM_POS_TIME_MIN_VAL);
			CCMHelper_ConvertValToText(screen, CCM_POS_TIME_SEC_VAL);
		}
		
		if (time_left == 0)
		{
			curr_logic_state = LSTATE_NONE;
			
			CCMHelper_SetElementFunctionality(screen, CCM_POS_ACTUAL_MASS_ITEM, 1, 1);
			if (actual_mass_val > 0)
			{
				CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNT_PARAMS, 1, 1);
			}
			
			
			if (screen->hovered != NULL && (screen->hovered->id == begin_check_id || screen->hovered->id == stop_check_id))
			{
				ui_hoverNext(screen, 1);
			}
			CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 0, 0);
			CCMHelper_SetElementFunctionality(screen, CCM_POS_STOP_CHECK, 0, 0);
		}
		
		ui_update_required = true;
	}
	
	CCMHelper_SetElementFunctionality(screen, CCM_POS_BACK, switch_to_start_menu_allowed, switch_to_start_menu_allowed);
}

static void CalibrationCheckMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			{
				int16_t val_pos = CCMHelper_GetValPosFromItemId(element_id);
				if (!CCMHelper_CheckValPosValidity(val_pos)) return;
				
				uint32_t *val_ptr = val_ptrs[val_pos];
				
				if (val_pos == CCM_POS_AREA_DIVIDER_VAL)
				{
					uint8_t possible_divider_vals_count = sizeof(possible_area_divider_vals);
					selected_area_divider_pos = (press_type == PRESS_TYPE_UP) ? ( (selected_area_divider_pos + 1) % possible_divider_vals_count ) : ( (selected_area_divider_pos == 0) ? (possible_divider_vals_count - 1) : (selected_area_divider_pos - 1) );
					area_divider_val = possible_area_divider_vals[selected_area_divider_pos];
				}
				else
				{
					if (val_pos == CCM_POS_SPEED_VAL)
					{
						uint32_t min_val = ceil(getUserParameterFloat(USER_PARAM_SPEED_MIN));
						uint32_t max_val = floor(getUserParameterFloat(USER_PARAM_SPEED_MAX));
						utils_edit_value_by_slot_with_min_max(val_ptr, selected_slot, val_allowed_lengths[val_pos], press_type, min_val, max_val);
					}
					else
					{
						utils_edit_value_by_slot(val_ptr, selected_slot, press_type, 0);
					}
				}
				CCMHelper_UpdateValStr_SlotBlink(screen, false);
			}
			break;
		case PRESS_TYPE_OK:
			switch (element_id)
			{
				case back_id: UI_BuildSeederOptionsMenu(screen); break;
				case begin_check_id:
				{
					current_can_motor_speed = calculateMotorSpeed_fromTime(getUserParameterInt(USER_PARAM_QUOTA), area_divider_val, mass_per_turn_val, initial_time_total);
					
					uint8_t deselection_array[] = { CCM_POS_SPEED_ITEM, CCM_POS_AREA_ITEM };
					CCMHelper_SetElementFunctionality_Array(screen, deselection_array, sizeof(deselection_array), 1, 0);
					
					uint8_t deselection_array_2[] = { CCM_POS_ACTUAL_MASS_ITEM, CCM_POS_COUNT_PARAMS, CCM_POS_BACK };
					CCMHelper_SetElementFunctionality_Array(screen, deselection_array_2, sizeof(deselection_array_2), 0, 1);
					
					CCMHelper_SetElementFunctionality(screen, CCM_POS_STOP_CHECK, 1, 1);
					
					ui_hoverNext(screen, 1);
					CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 0, 0);
					
					setCurrentLogicState(LSTATE_CAN_PROCEDURE);
					
					ui_update_required = true;
					
					break;
				}
				case stop_check_id:
				{
					CCMHelper_SetElementFunctionality(screen, CCM_POS_ACTUAL_MASS_ITEM, 1, 1);
					if (actual_mass_val > 0)
					{
						CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNT_PARAMS, 1, 1);
					}
					
					CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 1, 1);
					
					ui_hoverNext(screen, 0);
					CCMHelper_SetElementFunctionality(screen, CCM_POS_STOP_CHECK, 0, 0);
					
					setCurrentLogicState(LSTATE_NONE);
						
					ui_update_required = true;
					
					break;
				}
				case count_params_id:
				{
					uint32_t deviation_scaler = getPow10(val_lengths_after_dot[CCM_POS_DEVIATION_VAL]);
					new_mass_val = round( (float)actual_mass_val / (current_can_motor_speed * (float)total_motor_movement_time / SECONDS_IN_MINUTE / MILLIS_IN_SECOND) );
					deviation_percent_val = round( ((float)abs((int32_t)new_mass_val - (int32_t)mass_per_turn_val)) * (float)deviation_scaler * 100 / (float)mass_per_turn_val );
					
					CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 0, 0);
					
					if (new_mass_val < 10 * GRAMS_IN_KILOGRAM && deviation_percent_val < 50 * deviation_scaler)
					{
						CCMHelper_SetElementFunctionality(screen, CCM_POS_APPLY, 1, 1);
					}
					else
					{
						CCMHelper_SetElementFunctionality(screen, CCM_POS_APPLY, 0, 0);
					}
					
					
					CCMHelper_ConvertValToText(screen, CCM_POS_NEW_MASS_VAL);
					CCMHelper_ConvertValToText(screen, CCM_POS_DEVIATION_VAL);
				
					CCMHelper_SetElementFunctionality(screen, CCM_POS_NEW_MASS_ITEM, 1, 0);
					CCMHelper_SetElementFunctionality(screen, CCM_POS_DEVIATION_ITEM, 1, 0);
					
					ui_update_required = true;
					break;
				}
				case apply_id:
				{
					setUserParameterInt(USER_PARAM_MASS_PER_TURN, new_mass_val);
					
					uint32_t user_seeder_width = getUserParameterInt(USER_PARAM_SEEDER_WIDTH), user_quota = getUserParameterInt(USER_PARAM_QUOTA);
					float min_speed_val = calculateMinSpeed_fromMotorSpeed(user_quota, user_seeder_width, new_mass_val), max_speed_val = calculateMaxSpeed_fromMotorSpeed(user_quota, user_seeder_width, new_mass_val);
					if (max_speed_val <= min_speed_val)
					{
						min_speed_val = 0;
						max_speed_val = 0;
					}
					
					setUserParameterFloat(USER_PARAM_SPEED_MIN, min_speed_val);
					setUserParameterFloat(USER_PARAM_SPEED_MAX, max_speed_val);
					
					break;
				}
			}
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}

static void CalibrationCheckMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case speed_item_id: case area_item_id: case time_item_id: case counted_mass_item_id: case actual_mass_item_id: case deviation_item_id: case new_mass_item_id:
		{
			int16_t val_pos = CCMHelper_GetValPosFromItemId(element_id);
			if (!CCMHelper_CheckValPosValidity(val_pos)) return;
			
			if (!screen->item_is_selected)
			{
				CCMHelper_UpdateValStr_SlotBlink(screen, false);
				ui_selectItem(screen, 0, 1); // Force select
			}
			else
			{
				CCMHelper_UpdateValStr_SlotBlink(screen, false);
				
				selected_slot = (selected_slot + 1) % (val_allowed_lengths[val_pos]);
				if (selected_slot == 0)
				{
					ui_selectItem(screen, 0, 0); // Force deselect
				}
			}
			break;
		}
	}
}

static void CalibrationCheckMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case speed_item_id: case area_item_id: case time_item_id: case counted_mass_item_id: case actual_mass_item_id: case deviation_item_id: case new_mass_item_id:
		{
			int16_t val_pos = CCMHelper_GetValPosFromItemId(element_id);
			if (!CCMHelper_CheckValPosValidity(val_pos)) return;
			
			if (screen->item_is_selected)
			{
				selected_slot = 0;
			}
			else
			{
				// Main deselection callback
				switch (element_id)
				{
					case speed_item_id: case area_item_id:
					{
						CCMHelper_CalculateRequiredTime();
						
						CCMHelper_ConvertValToText(screen, CCM_POS_TIME_HOUR_VAL);
						CCMHelper_ConvertValToText(screen, CCM_POS_TIME_MIN_VAL);
						CCMHelper_ConvertValToText(screen, CCM_POS_TIME_SEC_VAL);
						
						uint8_t motor_speed_check = 0;
						uint32_t user_quota = getUserParameterInt(USER_PARAM_QUOTA);
						if (initial_time_total != 0 && user_quota != 0 && mass_per_turn_val != 0)
						{
							counted_mass_val = round( (float)user_quota * GRAMS_IN_KILOGRAM / (float)area_divider_val );
							CCMHelper_ConvertValToText(screen, CCM_POS_COUNTED_MASS_VAL);
							
							motor_speed_check = performMotorSpeedCheck( calculateMotorSpeed_fromTime(user_quota, area_divider_val, mass_per_turn_val, initial_time_total) );
						}
						
						if (motor_speed_check)
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 1, 1);
							CCMHelper_SetElementFunctionality(screen, CCM_POS_TIME_ITEM, 1, 0);
							CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNTED_MASS_ITEM, 1, 0);
						}
						else
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 0, 0);
							CCMHelper_SetElementFunctionality(screen, CCM_POS_TIME_ITEM, 0, 0);
							CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNTED_MASS_ITEM, 0, 0);
						}
						
						ui_update_required = true;
						
						break;
					}
					case actual_mass_item_id:
					{
						if (actual_mass_val != 0)
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNT_PARAMS, 1, 1);
						}
						else
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNT_PARAMS, 0, 1);
						}
						break;
					}
				}
			}
		}
	}
}

//==================================
// Initial setup
//==================================

void UI_BuildCalibrationCheckMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	//seconds_selected = seconds_selected_default;
	selected_slot = 0;
	selected_slot_gone = false;
	selected_area_divider_pos = 0;
	total_motor_movement_time = 0;
	
	float user_speed_min = getUserParameterFloat(USER_PARAM_SPEED_MIN),
				user_speed_max = getUserParameterFloat(USER_PARAM_SPEED_MAX);
	uint32_t user_quota = getUserParameterInt(USER_PARAM_QUOTA),
					 user_seeder_width = getUserParameterInt(USER_PARAM_SEEDER_WIDTH),
					 user_mass_per_turn = getUserParameterInt(USER_PARAM_MASS_PER_TURN);
	
  for (uint8_t i = 0; i < CCM_ELEMENT_TEXT_COUNT; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        text_xpos[i],                 // pos_x
        text_ypos[i],            // pos_y
				WHITE,							// color
			  label_tab_ids[i],              // tab index
				(text_offset_scalers[i] + 3) * CHAR_BASE_WIDTH,  // cursor offset
			  labels[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
		vis->id = label_ids[i];

    // ---------------- Interactable ----------------
		if (label_ids[i] == back_id)
		{
			CCMHelper_SetElementFunctionality(screen, i, 1, 1);
		}
		else if (label_ids[i] == speed_item_id || label_ids[i] == area_item_id || label_ids[i] == mass_per_turn_item_id)
		{
			if ( label_ids[i] != mass_per_turn_item_id && (user_speed_max != 0 && user_quota != 0 && user_seeder_width != 0 && user_mass_per_turn != 0) )
			{
				CCMHelper_SetElementFunctionality(screen, i, 1, 1);
			}
			else
			{
				CCMHelper_SetElementFunctionality(screen, i, 1, 0);
			}
		}
		else
		{
			CCMHelper_SetElementFunctionality(screen, i, 0, 0);
		}
  }
	
  for (uint8_t i = 0; i < CCM_ELEMENT_VAL_COUNT; i++)
  {
		*val_ptrs[i] = 0;
	}
	mass_per_turn_val = user_mass_per_turn;
	speed_val = ceil(user_speed_min);
	area_divider_val = possible_area_divider_vals[selected_area_divider_pos];
	
	if (speed_val != 0 && area_divider_val != 0 && mass_per_turn_val != 0 && user_quota != 0 && user_seeder_width != 0)
	{
		CCMHelper_CalculateRequiredTime();
		
		uint8_t motor_speed_check = performMotorSpeedCheck( calculateMotorSpeed_fromTime(user_quota, area_divider_val, mass_per_turn_val, initial_time_total) );
		if (motor_speed_check)
		{
			counted_mass_val = round( (float)user_quota * GRAMS_IN_KILOGRAM / (float)area_divider_val );
			CCMHelper_ConvertValToText(screen, CCM_POS_COUNTED_MASS_VAL);
			
			CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CHECK, 1, 1);
			CCMHelper_SetElementFunctionality(screen, CCM_POS_TIME_ITEM, 1, 0);
			CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNTED_MASS_ITEM, 1, 0);
		}
	}
	
  for (uint8_t i = 0; i < CCM_ELEMENT_VAL_COUNT; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        val_xpos[i],                 // pos_x
        val_ypos[i],            // pos_y
				WHITE,							// color
			  0,              // tab index
				-1,  // cursor offset
			  "",          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );
		
    // Optional: assign visual ID
		vis->id = val_ids[i];
		
		CCMHelper_ConvertValToText(screen, i);
  }
	
	
	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	screen->general_callback = CalibrationCheckMenu_ScreenCallback;
	screen->callback_interval = 50;
	
	setCurrentLogicState(LSTATE_NONE);
}
