#include <ui_screen_calibration_check_menu.h>

#define CCM_ELEMENT_TEXT_COUNT 12
#define CCM_POS_WIDTH_ITEM 0
#define CCM_POS_QUOTA_ITEM 1
#define CCM_POS_FILL_MOTOR 2
#define CCM_POS_TIME_ITEM 3
#define CCM_POS_SPINS_ITEM 4
#define CCM_POS_BEGIN_CALIBRATION 5
#define CCM_POS_MASS_ITEM 6
#define CCM_POS_COUNT_PARAMS 7
#define CCM_POS_COUNTED_MASS_ITEM 8
#define CCM_POS_MIN_SPEED_ITEM 9
#define CCM_POS_MAX_SPEED_ITEM 10
#define CCM_POS_BACK 11

static char* labels[CCM_ELEMENT_TEXT_COUNT] = { "Ширина:", "Норма:", "Заполнить катушки", "Время:", "Об.мотора:", "Начать калибровку", "Масса:", "Рассчитать массу", "Масса:", "MIN:", "MAX:", "назад" };
static uint8_t text_xpos[CCM_ELEMENT_TEXT_COUNT] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t text_ypos[CCM_ELEMENT_TEXT_COUNT] = { 4, 20, 36, 52, 68, 84, 100, 116, 132, 148, 164, 180 };
static uint8_t text_offset_scalers[CCM_ELEMENT_TEXT_COUNT] = { 3, 8, 0, 8, 4, 0, 7, 0, 10, 6, 6, 0 };
static const uint8_t width_item_id = 1, quota_item_id = 2, fill_motor_id = 3, time_item_id = 4, spins_item_id = 5, begin_calibration_id = 6, mass_item_id = 7, count_params_id = 8, counted_mass_item_id = 9, min_speed_item_id = 10, max_speed_item_id = 11, back_id = 12;
static uint8_t label_ids[CCM_ELEMENT_TEXT_COUNT] = { width_item_id, quota_item_id, fill_motor_id, time_item_id, spins_item_id, begin_calibration_id, mass_item_id, count_params_id, counted_mass_item_id, min_speed_item_id, max_speed_item_id, back_id };
static uint8_t label_tab_ids[CCM_ELEMENT_TEXT_COUNT] = { 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 12 };

#define CCM_ELEMENT_VAL_COUNT 9
#define CCM_POS_WIDTH_VAL 0
#define CCM_POS_QUOTA_VAL 1
#define CCM_POS_TIME_MIN_VAL 2
#define CCM_POS_TIME_SEC_VAL 3
#define CCM_POS_SPINS_VAL 4
#define CCM_POS_MASS_VAL 5
#define CCM_POS_COUNTED_MASS_VAL 6
#define CCM_POS_COUNTED_MIN_SPEED_VAL 7
#define CCM_POS_COUNTED_MAX_SPEED_VAL 8

static uint16_t width_val, quota_val, time_min_val, time_sec_val, spins_val, mass_val, counted_mass_val, counted_min_speed_val, counted_max_speed_val;
static uint16_t* val_ptrs[CCM_ELEMENT_VAL_COUNT] = { &width_val, &quota_val, &time_min_val, &time_sec_val, &spins_val, &mass_val, &counted_mass_val, &counted_min_speed_val, &counted_max_speed_val };
static char* val_types[CCM_ELEMENT_VAL_COUNT] = { "м", "кг/га", "мин", "с", "об", "кг", "кг/об", "км/ч", "км/ч" };
static uint8_t val_xpos[CCM_ELEMENT_VAL_COUNT] = { 46, 40, 40, 70, 64, 40, 40, 28, 28 };
static uint8_t val_ypos[CCM_ELEMENT_VAL_COUNT] = { 4, 20, 52, 52, 68, 100, 132, 148, 164 };
static uint8_t val_allowed_lengths[CCM_ELEMENT_VAL_COUNT] = { 2, 3, 2, 2, 2, 4, 4, 2, 2 };
static const uint8_t width_val_id = 21, quota_val_id = 22, time_min_val_id = 23, time_sec_val_id = 24, spins_val_id = 25, mass_val_id = 26, counted_mass_val_id = 27, counted_min_speed_val_id = 28, counted_max_speed_val_id = 29;
static uint8_t val_ids[CCM_ELEMENT_VAL_COUNT] = { width_val_id, quota_val_id, time_min_val_id, time_sec_val_id, spins_val_id, mass_val_id, counted_mass_val_id, counted_min_speed_val_id, counted_max_speed_val_id };


static uint8_t selected_slot, seconds_selected, selected_slot_gone;
static const uint8_t seconds_selected_default = true;

static uint32_t initial_time_total, total_motor_movement_time;
static uint64_t time_save_ui = 0;
static const uint16_t slot_blink_period = 400;


static void CalibrationCheckMenu_ScreenCallback(UI_Screen* screen);
static void CalibrationCheckMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
static void CalibrationCheckMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
static void CalibrationCheckMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);

static int16_t CCMHelper_GetValPosFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case width_item_id: val_pos = CCM_POS_WIDTH_VAL; break;
		case quota_item_id: val_pos = CCM_POS_QUOTA_VAL; break;
		case time_item_id: val_pos = CCM_POS_TIME_MIN_VAL + seconds_selected; break;
		case spins_item_id: val_pos = CCM_POS_SPINS_VAL; break;
		case mass_item_id: val_pos = CCM_POS_MASS_VAL; break;
		case counted_mass_item_id: val_pos = CCM_POS_COUNTED_MASS_VAL; break;
		case counted_min_speed_val_id: val_pos = CCM_POS_COUNTED_MIN_SPEED_VAL; break;
		case counted_max_speed_val_id: val_pos = CCM_POS_COUNTED_MAX_SPEED_VAL; break;
	}
	return val_pos;
}

static void CCMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos];
	uint16_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	utils_val_to_text_converter(final_string, length, value, type, selected_slot, selected_slot_gone);
	ui_editText(e, final_string, e->data.text.font);
}

static void CCMHelper_SlotBlinkUpdate(UI_Screen* screen, uint8_t slot_gone)
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
			if ( (val_pos < CCM_ELEMENT_VAL_COUNT && val_pos > 0) && val_ptrs[val_pos] != NULL)
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


static void CalibrationCheckMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save_ui > slot_blink_period)
	{
		CCMHelper_SlotBlinkUpdate(screen, !selected_slot_gone);
		ui_update_required = true;
	}
	
	if (can_procedure_in_progress)
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
			CCMHelper_ConvertValToText(screen, CCM_POS_TIME_MIN_VAL);
			CCMHelper_ConvertValToText(screen, CCM_POS_TIME_SEC_VAL);
			
			if (time_sec_val == 0 && time_min_val == 0)
			{
				can_procedure_in_progress = false;
				
				CCMHelper_SetElementFunctionality(screen, CCM_POS_MASS_ITEM, 1, 1);
				if (mass_val > 0)
				{
					CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNT_PARAMS, 1, 1);
				}
				
				if (screen->hovered != NULL && screen->hovered->id == begin_calibration_id)
				{
					ui_hoverNext(screen, 1);
				}
				
				UI_Element_Visual *e = CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CALIBRATION, 0, 0);
				if (e != NULL)
				{
					ui_editText(e, labels[CCM_POS_BEGIN_CALIBRATION], 0);
				}
			}
			
			ui_update_required = true;
		}
	}
	
	if (sys_timer - can_last_send_time > 5000)
	{
		switch_to_start_menu_allowed = true;
		
		CCMHelper_SetElementFunctionality(screen, CCM_POS_BACK, 1, 1);
	}
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
				if ( (val_pos >= CCM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return;
				
				uint16_t *val_ptr = val_ptrs[val_pos];
				uint8_t max_slot_val = 10;
				if (element_id == time_item_id && selected_slot == 1)
				{
					max_slot_val = 6;
				}
				
				utils_edit_value_by_slot(val_ptr, selected_slot, press_type, max_slot_val);
				CCMHelper_SlotBlinkUpdate(screen, false);
			}
			break;
		case PRESS_TYPE_OK:
			switch (element_id)
			{
				case back_id: UI_BuildSeederOptionsMenu(screen); break;
				case fill_motor_id:
				{
					uint8_t selection_array[] = { CCM_POS_TIME_ITEM, CCM_POS_SPINS_ITEM };
					CCMHelper_SetElementFunctionality_Array(screen, selection_array, sizeof(selection_array), 1, 1);
					
					uint8_t deselection_array[] = { CCM_POS_WIDTH_ITEM, CCM_POS_QUOTA_ITEM };
					CCMHelper_SetElementFunctionality_Array(screen, deselection_array, sizeof(deselection_array), 1, 0);
					
					CCMHelper_SetElementFunctionality(screen, CCM_POS_BACK, 0, 1);
					
					ui_hoverNext(screen, 1);
					CCMHelper_SetElementFunctionality(screen, CCM_POS_FILL_MOTOR, 0, 0);
					
					switch_to_start_menu_allowed = false;
					can_should_send_test_package = true;
					break;
				}
				case begin_calibration_id:
				{
					if (can_procedure_in_progress)
					{
						CCMHelper_SetElementFunctionality(screen, CCM_POS_MASS_ITEM, 1, 1);
						if (mass_val > 0)
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_COUNT_PARAMS, 1, 1);
						}
						
						switch_to_start_menu_allowed = true;
						can_procedure_in_progress = false;
						
						ui_editText(element->visual, labels[CCM_POS_BEGIN_CALIBRATION], 0);
						ui_update_required = true;
					}
					else
					{
						current_can_motor_speed = ( (float)spins_val / ((float)initial_time_total / 60) );
						
						uint8_t deselection_array[] = { CCM_POS_SPINS_ITEM, CCM_POS_TIME_ITEM };
						CCMHelper_SetElementFunctionality_Array(screen, deselection_array, sizeof(deselection_array), 1, 0);
						
						uint8_t deselection_array_2[] = { CCM_POS_MASS_ITEM, CCM_POS_COUNT_PARAMS, CCM_POS_BACK };
						CCMHelper_SetElementFunctionality_Array(screen, deselection_array_2, sizeof(deselection_array_2), 0, 1);
						
						switch_to_start_menu_allowed = false;
						can_procedure_in_progress = true;
						
						ui_editText(element->visual, "Остановить", 0);
						ui_update_required = true;
					}
					break;
				}
				case count_params_id:
				{
					counted_mass_val = round( (float)mass_val / (float)spins_val * ((float)total_motor_movement_time / 1000) / (float)initial_time_total );
					counted_min_speed_val = 2;
					counted_max_speed_val = 12;
					
					user_mass_per_turn = counted_mass_val;
					user_min_speed = counted_min_speed_val;
					user_max_speed = counted_max_speed_val;
					
					CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CALIBRATION, 0, 0);
					
					CCMHelper_ConvertValToText(screen, CCM_POS_COUNTED_MASS_VAL);
					CCMHelper_ConvertValToText(screen, CCM_POS_COUNTED_MIN_SPEED_VAL);
					CCMHelper_ConvertValToText(screen, CCM_POS_COUNTED_MAX_SPEED_VAL);
					
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

static void CalibrationCheckMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case width_item_id: case quota_item_id: case time_item_id: case spins_item_id: case mass_item_id: case counted_mass_item_id: case min_speed_item_id: case max_speed_item_id:
		{
			int16_t val_pos = CCMHelper_GetValPosFromItemId(element_id);
			if ( (val_pos >= CCM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return;
			
			if (!screen->item_is_selected)
			{
				CCMHelper_SlotBlinkUpdate(screen, false);
				ui_selectItem(screen, 0, 1); // Force select
			}
			else
			{
				CCMHelper_SlotBlinkUpdate(screen, false);
				
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

static void CalibrationCheckMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case width_item_id: case quota_item_id: case time_item_id: case spins_item_id: case mass_item_id: case counted_mass_item_id: case min_speed_item_id: case max_speed_item_id:
		{
			int16_t val_pos = CCMHelper_GetValPosFromItemId(element_id);
			if ( (val_pos >= CCM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return;
			
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
						// TODO: Handle width and quota being saved to FLASH memory
						if (element_id == width_item_id)
						{
							user_seeder_width = width_val;
						}
						else
						{
							user_quota = quota_val;
						}
						
						if (width_val != 0 && quota_val != 0)
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_FILL_MOTOR, 1, 1);
						}
						else
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_FILL_MOTOR, 0, 1);
						}
						break;
					}
					case time_item_id: case spins_item_id:
					{
						initial_time_total = time_min_val * 60 + time_sec_val;
						uint32_t motor_speed_check = 0;
						if (time_sec_val != 0)
						{
							motor_speed_check = round( ( (float)spins_val / ((float)initial_time_total / 60) ) * 10 );
						}
						
						if (time_sec_val != 0 && spins_val != 0 && motor_speed_check > 0 && motor_speed_check < 2560)
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CALIBRATION, 1, 1);
						}
						else
						{
							CCMHelper_SetElementFunctionality(screen, CCM_POS_BEGIN_CALIBRATION, 0, 1);
						}
						break;
					}
					case mass_item_id:
					{
						if (mass_val != 0)
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

void UI_BuildCalibrationCheckMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	seconds_selected = seconds_selected_default;
	selected_slot = 0;
	selected_slot_gone = false;
	total_motor_movement_time = 0;
	
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
		if (label_ids[i] == width_item_id || label_ids[i] == quota_item_id || label_ids[i] == back_id)
		{
			CCMHelper_SetElementFunctionality(screen, i, 1, 1);
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
	width_val = user_seeder_width;
	quota_val = user_quota;
	if (width_val != 0 && quota_val != 0)
	{
		CCMHelper_SetElementFunctionality(screen, CCM_POS_FILL_MOTOR, 1, 1);
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
	
	main_functionality_active = false;
	switch_to_start_menu_allowed = true;
}
