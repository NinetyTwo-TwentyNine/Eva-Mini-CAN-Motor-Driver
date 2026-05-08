#include <ui_screen_calibration_menu.h>

#define CM_ELEMENT_TEXT_COUNT 12
#define CM_POS_WIDTH_ITEM 0
#define CM_POS_QUOTA_ITEM 1
#define CM_POS_FILL_MOTOR 2
#define CM_POS_TIME_ITEM 3
#define CM_POS_SPINS_ITEM 4
#define CM_POS_BEGIN_CALIBRATION 5
#define CM_POS_MASS_ITEM 6
#define CM_POS_COUNT_PARAMS 7
#define CM_POS_COUNTED_MASS_ITEM 8
#define CM_POS_MIN_SPEED_ITEM 9
#define CM_POS_MAX_SPEED_ITEM 10
#define CM_POS_BACK 11

static char* labels[CM_ELEMENT_TEXT_COUNT] = { "Ширина:", "Норма:", "Заполнить катушки", "Время:", "Об.мотора:", "Начать калибровку", "Масса:", "Рассчитать массу", "Масса:", "MIN:", "MAX:", "назад" };
static uint8_t text_xpos[CM_ELEMENT_TEXT_COUNT] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t text_ypos[CM_ELEMENT_TEXT_COUNT] = { 4, 20, 36, 52, 68, 84, 100, 116, 132, 148, 164, 180 };
static uint8_t text_offset_scalers[CM_ELEMENT_TEXT_COUNT] = { 3, 8, 0, 8, 4, 0, 7, 0, 10, 9, 9, 0 };
static const uint8_t width_item_id = 1, quota_item_id = 2, fill_motor_id = 3, time_item_id = 4, spins_item_id = 5, begin_calibration_id = 6, mass_item_id = 7, count_params_id = 8, counted_mass_item_id = 9, min_speed_item_id = 10, max_speed_item_id = 11, back_id = 12;
static uint8_t label_ids[CM_ELEMENT_TEXT_COUNT] = { width_item_id, quota_item_id, fill_motor_id, time_item_id, spins_item_id, begin_calibration_id, mass_item_id, count_params_id, counted_mass_item_id, min_speed_item_id, max_speed_item_id, back_id };
static uint8_t label_tab_ids[CM_ELEMENT_TEXT_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

#define CM_ELEMENT_VAL_COUNT 9
#define CM_POS_WIDTH_VAL 0
#define CM_POS_QUOTA_VAL 1
#define CM_POS_TIME_MIN_VAL 2
#define CM_POS_TIME_SEC_VAL 3
#define CM_POS_SPINS_VAL 4
#define CM_POS_MASS_VAL 5
#define CM_POS_COUNTED_MASS_VAL 6
#define CM_POS_COUNTED_MIN_SPEED_VAL 7
#define CM_POS_COUNTED_MAX_SPEED_VAL 8

static uint32_t width_val, quota_val, time_min_val, time_sec_val, spins_val, mass_val, counted_mass_val, counted_min_speed_val, counted_max_speed_val;
static uint32_t* val_ptrs[CM_ELEMENT_VAL_COUNT] = { &width_val, &quota_val, &time_min_val, &time_sec_val, &spins_val, &mass_val, &counted_mass_val, &counted_min_speed_val, &counted_max_speed_val };
static char* val_types[CM_ELEMENT_VAL_COUNT] = { "м", "кг/га", "мин", "с", "об", "кг", "кг/об", "км/ч", "км/ч" };
static uint8_t val_xpos[CM_ELEMENT_VAL_COUNT] = { 46, 40, 40, 70, 64, 40, 40, 28, 28 };
static uint8_t val_ypos[CM_ELEMENT_VAL_COUNT] = { 4, 20, 52, 52, 68, 100, 132, 148, 164 };
static uint8_t val_allowed_lengths[CM_ELEMENT_VAL_COUNT] = { 2, 3, 2, 2, 2, 4, 4, 4, 4 };
static uint8_t val_lengths_after_dot[CM_ELEMENT_VAL_COUNT] = { 0, 0, 0, 0, 0, 3, 3, 2, 2 };
static const uint8_t width_val_id = 21, quota_val_id = 22, time_min_val_id = 23, time_sec_val_id = 24, spins_val_id = 25, mass_val_id = 26, counted_mass_val_id = 27, counted_min_speed_val_id = 28, counted_max_speed_val_id = 29;
static uint8_t val_ids[CM_ELEMENT_VAL_COUNT] = { width_val_id, quota_val_id, time_min_val_id, time_sec_val_id, spins_val_id, mass_val_id, counted_mass_val_id, counted_min_speed_val_id, counted_max_speed_val_id };


static uint8_t selected_slot, seconds_selected, selected_slot_gone;
static const uint8_t seconds_selected_default = true;

static uint32_t initial_time_total, total_motor_movement_time;
static uint64_t time_save_ui = 0;
static const uint16_t slot_blink_period = 400;

static int16_t CMHelper_GetValPosFromItemId(uint8_t item_id);
static uint8_t CMHelper_CheckValPosValidity(int16_t val_pos);
static void CMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos);
static void CMHelper_UpdateValStr_SlotBlink(UI_Screen* screen, uint8_t slot_gone);
static UI_Element_Visual* CMHelper_SetElementFunctionality(UI_Screen* screen, uint8_t item_pos, uint8_t tab_index_on, uint8_t functionality_on);
static void CMHelper_SetElementFunctionality_Array(UI_Screen* screen, uint8_t* elem_array, uint8_t elem_count, uint8_t tab_index_on, uint8_t functionality_on);
static float CMHelper_CalculateMotorSpeed(void);

static void CalibrationMenu_ScreenCallback(UI_Screen* screen);
static void CalibrationMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
static void CalibrationMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
static void CalibrationMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);

//==================================
// Helpers
//==================================

static int16_t CMHelper_GetValPosFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case width_item_id: val_pos = CM_POS_WIDTH_VAL; break;
		case quota_item_id: val_pos = CM_POS_QUOTA_VAL; break;
		case time_item_id: val_pos = CM_POS_TIME_MIN_VAL + seconds_selected; break;
		case spins_item_id: val_pos = CM_POS_SPINS_VAL; break;
		case mass_item_id: val_pos = CM_POS_MASS_VAL; break;
		case counted_mass_item_id: val_pos = CM_POS_COUNTED_MASS_VAL; break;
		case counted_min_speed_val_id: val_pos = CM_POS_COUNTED_MIN_SPEED_VAL; break;
		case counted_max_speed_val_id: val_pos = CM_POS_COUNTED_MAX_SPEED_VAL; break;
	}
	return val_pos;
}

static uint8_t CMHelper_CheckValPosValidity(int16_t val_pos)
{
	if ( (val_pos >= CM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return 0;
	
	return 1;
}

static void CMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	if (!CMHelper_CheckValPosValidity(val_pos)) return;
	
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos], length_after_dot = val_lengths_after_dot[val_pos];
	uint32_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	utils_val_to_text_converter(final_string, length, length_after_dot, value, type, selected_slot, selected_slot_gone);
	ui_editText(e, final_string, e->data.text.font);
}

static void CMHelper_UpdateValStr_SlotBlink(UI_Screen* screen, uint8_t slot_gone)
{
	time_save_ui = sys_timer;
	selected_slot_gone = slot_gone;
		
	CMHelper_ConvertValToText(screen, CMHelper_GetValPosFromItemId(screen->hovered->id));
}

static UI_Element_Visual* CMHelper_SetElementFunctionality(UI_Screen* screen, uint8_t item_pos, uint8_t tab_index_on, uint8_t functionality_on)
{
		if ( item_pos >= CM_ELEMENT_TEXT_COUNT ) return NULL;
		
		UI_Element_Visual *e = ui_findVisualById(screen, label_ids[item_pos]);
		if (e == NULL) return NULL;
		
		if (!tab_index_on && screen->hovered == e)
		{
			ui_hoverNext(screen, 1);
		}
		ui_setElementFlags(e, UI_ELEMENT_FLAG_ALT_COLOR | UI_ELEMENT_FLAG_TAB_OFF, !tab_index_on);
		ui_setElementFlags(e, UI_ELEMENT_FLAG_FUNC_OFF, !functionality_on);
		return e;
}
static void CMHelper_SetElementFunctionality_Array(UI_Screen* screen, uint8_t* elem_array, uint8_t elem_count, uint8_t tab_index_on, uint8_t functionality_on)
{
	for (uint8_t i = 0; i < elem_count; i++)
	{
		uint8_t item_pos = elem_array[i];
		CMHelper_SetElementFunctionality(screen, item_pos, tab_index_on, functionality_on);
	}
}

static float CMHelper_CalculateMotorSpeed()
{
	if (initial_time_total != 0)
	{
		return ( (float)spins_val / ((float)initial_time_total / SECONDS_IN_MINUTE) );
	}
	return 0;
}

//==================================
// Main functionality
//==================================

static void CalibrationMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save_ui > slot_blink_period)
	{
		CMHelper_UpdateValStr_SlotBlink(screen, !selected_slot_gone);
		ui_update_required = true;
	}
	
	if (curr_logic_state == LSTATE_CAN_PROCEDURE)
	{
		total_motor_movement_time += screen->callback_interval;
		if (total_motor_movement_time % 1000 == 0)
		{
			if (time_sec_val != 0 || time_min_val != 0)
			{
				if (time_sec_val == 0)
				{
					time_sec_val = 59;
					time_min_val -= 1;
				}
				else
				{
					time_sec_val -= 1;
				}
			}
			CMHelper_ConvertValToText(screen, CM_POS_TIME_MIN_VAL);
			CMHelper_ConvertValToText(screen, CM_POS_TIME_SEC_VAL);
			
			if (time_sec_val == 0 && time_min_val == 0)
			{
				setCurrentLogicState(LSTATE_NONE);
				
				CMHelper_SetElementFunctionality(screen, CM_POS_MASS_ITEM, 1, 1);
				if (mass_val > 0)
				{
					CMHelper_SetElementFunctionality(screen, CM_POS_COUNT_PARAMS, 1, 1);
				}
				
				UI_Element_Visual *e = CMHelper_SetElementFunctionality(screen, CM_POS_BEGIN_CALIBRATION, 0, 0);
				if (e != NULL)
				{
					ui_editText(e, labels[CM_POS_BEGIN_CALIBRATION], 0);
				}
			}
			
			ui_update_required = true;
		}
	}
	
	CMHelper_SetElementFunctionality(screen, CM_POS_BACK, switch_to_start_menu_allowed, switch_to_start_menu_allowed);
}

static void CalibrationMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			{
				int16_t val_pos = CMHelper_GetValPosFromItemId(element_id);
				if (!CMHelper_CheckValPosValidity(val_pos)) return;
				
				uint32_t *val_ptr = val_ptrs[val_pos];
				uint8_t max_slot_val = 10;
				if (element_id == time_item_id && selected_slot == 1)
				{
					max_slot_val = 6;
				}
				
				utils_edit_value_by_slot(val_ptr, selected_slot, press_type, max_slot_val);
				CMHelper_UpdateValStr_SlotBlink(screen, false);
			}
			break;
		case PRESS_TYPE_OK:
			switch (element_id)
			{
				case back_id: UI_BuildSeederOptionsMenu(screen); break;
				case fill_motor_id:
				{
					uint8_t selection_array[] = { CM_POS_TIME_ITEM, CM_POS_SPINS_ITEM };
					CMHelper_SetElementFunctionality_Array(screen, selection_array, sizeof(selection_array), 1, 1);
					
					uint8_t deselection_array[] = { CM_POS_WIDTH_ITEM, CM_POS_QUOTA_ITEM };
					CMHelper_SetElementFunctionality_Array(screen, deselection_array, sizeof(deselection_array), 1, 0);
					
					CMHelper_SetElementFunctionality(screen, CM_POS_BACK, 0, 1);
					
					ui_hoverNext(screen, 1);
					CMHelper_SetElementFunctionality(screen, CM_POS_FILL_MOTOR, 0, 0);
					
					setCurrentLogicState(LSTATE_CAN_TEST);
					break;
				}
				case begin_calibration_id:
				{
					if (curr_logic_state == LSTATE_CAN_PROCEDURE)
					{
						CMHelper_SetElementFunctionality(screen, CM_POS_MASS_ITEM, 1, 1);
						if (mass_val > 0)
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_COUNT_PARAMS, 1, 1);
						}
						
						setCurrentLogicState(LSTATE_NONE);
						
						ui_editText(element->visual, labels[CM_POS_BEGIN_CALIBRATION], 0);
						ui_update_required = true;
					}
					else
					{
						current_can_motor_speed = CMHelper_CalculateMotorSpeed();
						
						uint8_t deselection_array[] = { CM_POS_SPINS_ITEM, CM_POS_TIME_ITEM };
						CMHelper_SetElementFunctionality_Array(screen, deselection_array, sizeof(deselection_array), 1, 0);
						
						uint8_t deselection_array_2[] = { CM_POS_MASS_ITEM, CM_POS_COUNT_PARAMS, CM_POS_BACK };
						CMHelper_SetElementFunctionality_Array(screen, deselection_array_2, sizeof(deselection_array_2), 0, 1);
						
						setCurrentLogicState(LSTATE_CAN_PROCEDURE);
						
						ui_editText(element->visual, "Остановить", 0);
						ui_update_required = true;
					}
					break;
				}
				case count_params_id:
				{
					counted_mass_val = round( (float)mass_val / ((float)spins_val * ((float)total_motor_movement_time / MILLIS_IN_SECOND) / (float)initial_time_total) );
					float min_speed_val = calculateMinSpeed_fromMotorSpeed(quota_val, width_val, counted_mass_val), max_speed_val = calculateMaxSpeed_fromMotorSpeed(quota_val, width_val, counted_mass_val);
					if (max_speed_val <= min_speed_val)
					{
						min_speed_val = 0;
						max_speed_val = 0;
					}
					counted_min_speed_val = round(min_speed_val * getPow10(val_lengths_after_dot[CM_POS_COUNTED_MIN_SPEED_VAL]));
					counted_max_speed_val = round(max_speed_val * getPow10(val_lengths_after_dot[CM_POS_COUNTED_MAX_SPEED_VAL]));
					
					setUserParameterInt(USER_PARAM_MASS_PER_TURN, counted_mass_val);
					setUserParameterFloat(USER_PARAM_SPEED_MIN, min_speed_val);
					setUserParameterFloat(USER_PARAM_SPEED_MAX, max_speed_val);
					
					CMHelper_SetElementFunctionality(screen, CM_POS_BEGIN_CALIBRATION, 0, 0);
					
					
					uint8_t text_items_array[] = {CM_POS_COUNTED_MASS_ITEM, CM_POS_MIN_SPEED_ITEM, CM_POS_MAX_SPEED_ITEM, CM_POS_COUNTED_MASS_VAL, CM_POS_COUNTED_MIN_SPEED_VAL, CM_POS_COUNTED_MAX_SPEED_VAL};
					for (uint8_t i = 0; i < sizeof(text_items_array)/2; i++)
					{
						CMHelper_ConvertValToText(screen, text_items_array[sizeof(text_items_array)/2 + i]);
						CMHelper_SetElementFunctionality(screen, text_items_array[i], 1, 0);
					}
					
					ui_update_required = true;
					break;
				}
			}
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}

static void CalibrationMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case width_item_id: case quota_item_id: case time_item_id: case spins_item_id: case mass_item_id: case counted_mass_item_id: case min_speed_item_id: case max_speed_item_id:
		{
			int16_t val_pos = CMHelper_GetValPosFromItemId(element_id);
			if (!CMHelper_CheckValPosValidity(val_pos)) return;
			
			if (!screen->item_is_selected)
			{
				CMHelper_UpdateValStr_SlotBlink(screen, false);
				ui_selectItem(screen, 0, 1); // Force select
			}
			else
			{
				CMHelper_UpdateValStr_SlotBlink(screen, false);
				
				selected_slot = (selected_slot + 1) % (val_allowed_lengths[val_pos]);
				if (selected_slot == 0)
				{
					if (element_id == time_item_id && seconds_selected == seconds_selected_default)
					{
						seconds_selected = !seconds_selected;
						selected_slot = 0;
					}
					else
					{
						ui_selectItem(screen, 0, 0); // Force deselect
					}
				}
			}
			break;
		}
	}
}

static void CalibrationMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case width_item_id: case quota_item_id: case time_item_id: case spins_item_id: case mass_item_id: case counted_mass_item_id: case min_speed_item_id: case max_speed_item_id:
		{
			int16_t val_pos = CMHelper_GetValPosFromItemId(element_id);
			if (!CMHelper_CheckValPosValidity(val_pos)) return;
			
			if (screen->item_is_selected)
			{
				selected_slot = 0;
				if (element_id == time_item_id)
				{
					seconds_selected = seconds_selected_default;
				}
			}
			else
			{
				// Main deselection callback
				switch (element_id)
				{
					case width_item_id: case quota_item_id:
					{
						if (width_val != getUserParameterInt(USER_PARAM_SEEDER_WIDTH) || quota_val != getUserParameterInt(USER_PARAM_QUOTA))
						{
							counted_mass_val = 0;
							counted_min_speed_val = 0;
							counted_max_speed_val = 0;
							
							setUserParameterInt(USER_PARAM_MASS_PER_TURN, 0);
							setUserParameterFloat(USER_PARAM_SPEED_MIN, 0);
							setUserParameterFloat(USER_PARAM_SPEED_MAX, 0);
							
							CMHelper_ConvertValToText(screen, CM_POS_COUNTED_MASS_VAL);
							CMHelper_ConvertValToText(screen, CM_POS_COUNTED_MIN_SPEED_VAL);
							CMHelper_ConvertValToText(screen, CM_POS_COUNTED_MAX_SPEED_VAL);
							
							if (width_val != getUserParameterInt(USER_PARAM_SEEDER_WIDTH))
							{
								setUserParameterFloat(USER_PARAM_AREA_SESSION, 0.0);
							}
						}
						
						setUserParameterInt(USER_PARAM_SEEDER_WIDTH, width_val);
						setUserParameterInt(USER_PARAM_QUOTA, quota_val);
						
						if (width_val != 0 && quota_val != 0)
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_FILL_MOTOR, 1, 1);
						}
						else
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_FILL_MOTOR, 0, 1);
						}
						break;
					}
					case time_item_id: case spins_item_id:
					{
						initial_time_total = time_min_val * SECONDS_IN_MINUTE + time_sec_val;
						uint8_t motor_speed_check = performMotorSpeedCheck( CMHelper_CalculateMotorSpeed() );
						
						if (spins_val != 0 && motor_speed_check)
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_BEGIN_CALIBRATION, 1, 1);
						}
						else
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_BEGIN_CALIBRATION, 0, 1);
						}
						break;
					}
					case mass_item_id:
					{
						if (mass_val != 0)
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_COUNT_PARAMS, 1, 1);
						}
						else
						{
							CMHelper_SetElementFunctionality(screen, CM_POS_COUNT_PARAMS, 0, 1);
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

void UI_BuildCalibrationMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	seconds_selected = seconds_selected_default;
	selected_slot = 0;
	selected_slot_gone = false;
	total_motor_movement_time = 0;
	
  for (uint8_t i = 0; i < CM_ELEMENT_TEXT_COUNT; i++)
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
		vis->alt_color = DARKGRAY;
		
    // ---------------- Interactable ----------------
		UI_Callback main = CalibrationMenu_OnItemPressed_Main, selection = NULL, onselect = NULL;
		int16_t val_pos = CMHelper_GetValPosFromItemId(label_ids[i]);
		if (CMHelper_CheckValPosValidity(val_pos))
		{
			selection = CalibrationMenu_OnItemPressed_Selection;
			onselect = CalibrationMenu_OnItemPressed_OnSelect;
		}
		ui_bindInteractable(screen, vis, main, selection, onselect);
		
		if (label_ids[i] == width_item_id || label_ids[i] == quota_item_id || label_ids[i] == back_id)
		{
			CMHelper_SetElementFunctionality(screen, i, 1, 1);
		}
		else
		{
			CMHelper_SetElementFunctionality(screen, i, 0, 0);
		}
  }
	
  for (uint8_t i = 0; i < CM_ELEMENT_VAL_COUNT; i++)
  {
		*val_ptrs[i] = 0;
	}
	counted_mass_val = getUserParameterInt(USER_PARAM_MASS_PER_TURN);
	counted_min_speed_val = round(getUserParameterFloat(USER_PARAM_SPEED_MIN) * getPow10(val_lengths_after_dot[CM_POS_COUNTED_MIN_SPEED_VAL]));
	counted_max_speed_val = round(getUserParameterFloat(USER_PARAM_SPEED_MAX) * getPow10(val_lengths_after_dot[CM_POS_COUNTED_MAX_SPEED_VAL]));
	
	width_val = getUserParameterInt(USER_PARAM_SEEDER_WIDTH);
	quota_val = getUserParameterInt(USER_PARAM_QUOTA);
	if (width_val != 0 && quota_val != 0)
	{
		CMHelper_SetElementFunctionality(screen, CM_POS_FILL_MOTOR, 1, 1);
	}
	
  for (uint8_t i = 0; i < CM_ELEMENT_VAL_COUNT; i++)
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
		
		CMHelper_ConvertValToText(screen, i);
  }
	
	
	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	screen->general_callback = CalibrationMenu_ScreenCallback;
	screen->callback_interval = 50;
	
	setCurrentLogicState(LSTATE_NONE);
}
