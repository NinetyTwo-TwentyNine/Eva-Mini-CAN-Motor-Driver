#include <ui_screen_calibration_menu.h>

static const uint8_t element_text_count = 12;
static char* labels[element_text_count] = { "Ширина:", "Норма:", "Заполнить катушки", "Время:", "Об.мотора:", "Начать калибровку", "Масса:", "Рассчитать массу", "Масса:", "MIN:", "MAX:", "назад" };
static uint8_t text_xpos[element_text_count] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t text_ypos[element_text_count] = { 4, 20, 36, 52, 68, 84, 100, 116, 132, 148, 164, 180 };
static uint8_t text_offset_scalers[element_text_count] = { 3, 8, 0, 8, 4, 0, 7, 0, 10, 6, 6, 0 };
static const uint8_t width_item_id = 1, quota_item_id = 2, fill_motor_id = 3, time_item_id = 4, spins_item_id = 5, begin_calibration_id = 6, mass_item_id = 7, count_params_id = 8, counted_mass_item_id = 9, min_speed_item_id = 10, max_speed_item_id = 11, back_id = 12;
static uint8_t label_ids[element_text_count] = { width_item_id, quota_item_id, fill_motor_id, time_item_id, spins_item_id, begin_calibration_id, mass_item_id, count_params_id, counted_mass_item_id, min_speed_item_id, max_speed_item_id, back_id };
static uint8_t label_tab_ids[element_text_count] = { 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 12 };

static const uint8_t width_item_pos = 0, quota_item_pos = 1, fill_motor_pos = 2, time_item_pos = 3, spins_item_pos = 4, begin_calibration_pos = 5, mass_item_pos = 6, count_params_pos = 7, counted_mass_item_pos = 8, min_speed_item_pos = 9, max_speed_item_pos = 10, back_pos = 11;

static const uint8_t element_val_count = 9;
static uint16_t width_val, quota_val, time_min_val, time_sec_val, spins_val, mass_val, counted_mass_val, counted_min_speed_val, counted_max_speed_val;
static uint16_t* val_ptrs[element_val_count] = { &width_val, &quota_val, &time_min_val, &time_sec_val, &spins_val, &mass_val, &counted_mass_val, &counted_min_speed_val, &counted_max_speed_val };
static char* val_types[element_val_count] = { "м", "кг/га", "мин", "с", "об", "кг", "кг/об", "км/ч", "км/ч" };
static uint8_t val_xpos[element_val_count] = { 46, 40, 40, 70, 64, 40, 40, 28, 28 };
static uint8_t val_ypos[element_val_count] = { 4, 20, 52, 52, 68, 100, 132, 148, 164 };
static uint8_t val_allowed_lengths[element_val_count] = { 2, 3, 2, 2, 2, 4, 4, 2, 2 };
static const uint8_t width_val_id = 21, quota_val_id = 22, time_min_val_id = 23, time_sec_val_id = 24, spins_val_id = 25, mass_val_id = 26, counted_mass_val_id = 27, counted_min_speed_val_id = 28, counted_max_speed_val_id = 29;
static uint8_t val_ids[element_val_count] = { width_val_id, quota_val_id, time_min_val_id, time_sec_val_id, spins_val_id, mass_val_id, counted_mass_val_id, counted_min_speed_val_id, counted_max_speed_val_id };

static const uint8_t width_val_pos = 0, quota_val_pos = 1, time_min_val_pos = 2, time_sec_val_pos = 3, spins_val_pos = 4, mass_val_pos = 5, counted_mass_val_pos = 6, counted_min_speed_val_pos = 7, counted_max_speed_val_pos = 8;


static uint8_t selected_slot, seconds_selected, selected_slot_gone;
static const uint8_t seconds_selected_default = true;

static uint32_t initial_time_total, total_motor_movement_time;
static uint64_t time_save_ui = 0;
static const uint16_t slot_blink_period = 400;

static int16_t CalibrationMenu_GetValPosFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case width_item_id: val_pos = width_val_pos; break;
		case quota_item_id: val_pos = quota_val_pos; break;
		case time_item_id: val_pos = time_min_val_pos + seconds_selected; break;
		case spins_item_id: val_pos = spins_val_pos; break;
		case mass_item_id: val_pos = mass_val_pos; break;
		case counted_mass_item_id: val_pos = counted_mass_val_pos; break;
		case counted_min_speed_val_id: val_pos = counted_min_speed_val_pos; break;
		case counted_max_speed_val_id: val_pos = counted_max_speed_val_pos; break;
	}
	return val_pos;
}

static void CalibrationMenu_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
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

static void CalibrationMenu_SlotBlinkUpdate(UI_Screen* screen, uint8_t slot_gone)
{
		time_save_ui = sys_timer;
		selected_slot_gone = slot_gone;
		
		CalibrationMenu_ConvertValToText(screen, CalibrationMenu_GetValPosFromItemId(screen->hovered->id));
}


static void CalibrationMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save_ui > slot_blink_period)
	{
		CalibrationMenu_SlotBlinkUpdate(screen, !selected_slot_gone);
		ui_update_required = true;
	}
	
	if (can_procedure_in_progress)
	{
		total_motor_movement_time += screen->callback_interval;
		if (time_sec_val == 0 && time_min_val == 0)
		{
			can_procedure_in_progress = false;
			UI_Element_Visual *e = ui_findVisualById(screen, label_ids[begin_calibration_pos]);
			if (e != NULL)
			{
				ui_editText(e, labels[begin_calibration_pos], 0);
				e->id = 0;
				e->tab_index = 0;
			}
			
			e = ui_findVisualById(screen, mass_item_id);
			if (e != NULL) e->tab_index = label_tab_ids[mass_item_pos];
			if (mass_val > 0)
			{
				e = ui_findVisualById(screen, label_ids[count_params_pos]);
				if (e != NULL) e->tab_index = label_tab_ids[count_params_pos];
			}
		}
		else if (total_motor_movement_time % 1000 == 0)
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
			CalibrationMenu_ConvertValToText(screen, time_min_val_pos);
			CalibrationMenu_ConvertValToText(screen, time_sec_val_pos);
			ui_update_required = true;
		}
	}
	
	if (sys_timer - can_last_send_time > 5000)
	{
		switch_to_start_menu_allowed = true;
		UI_Element_Visual *e = ui_findVisualById(screen, back_id);
		if (e != NULL) e->tab_index = label_tab_ids[back_pos];
	}
}

static void CalibrationMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			if (screen->item_is_selected)
			{
				int16_t val_pos = CalibrationMenu_GetValPosFromItemId(element_id);
				if ( (val_pos >= element_val_count || val_pos < 0) || val_ptrs[val_pos] == NULL) return;
				
				uint16_t *val_ptr = val_ptrs[val_pos];
				uint8_t max_slot_val = 10;
				if (element_id == time_item_id && selected_slot == 1)
				{
					max_slot_val = 6;
				}
				
				utils_edit_value_by_slot(val_ptr, selected_slot, press_type, max_slot_val);
				CalibrationMenu_SlotBlinkUpdate(screen, false);
			}
			break;
		case PRESS_TYPE_OK:
			switch (element_id)
			{
				case back_id: UI_BuildSeederOptionsMenu(screen); break;
				case fill_motor_id:
				{
					UI_Element_Visual *e;
					
					uint8_t selection_array[] = { time_item_pos, spins_item_pos };
					for (uint8_t i = 0; i < sizeof(selection_array); i++)
					{
						e = ui_findVisualById(screen, label_ids[selection_array[i]]);
						if (e != NULL) e->tab_index = label_tab_ids[selection_array[i]];
					}
					
					uint8_t deselection_array[] = { width_item_pos, quota_item_pos };
					for (uint8_t i = 0; i < sizeof(deselection_array); i++)
					{
						e = ui_findVisualById(screen, label_ids[deselection_array[i]]);
						if (e != NULL) e->tab_index = 0;
					}
					
					e = ui_findVisualById(screen, label_ids[back_pos]);
					if (e != NULL) e->tab_index = 0;
					
					element->visual->tab_index = 0;
					element->visual->id = 0; // Disable element functionality
					
					switch_to_start_menu_allowed = false;
					can_should_send_test_package = true;
					break;
				}
				case begin_calibration_id:
				{
					UI_Element_Visual *e;
					
					if (can_procedure_in_progress)
					{
						switch_to_start_menu_allowed = true;
						can_procedure_in_progress = false;
						
						e = ui_findVisualById(screen, mass_item_id);
						if (e != NULL) e->tab_index = label_tab_ids[mass_item_pos];
						if (mass_val > 0)
						{
							e = ui_findVisualById(screen, label_ids[count_params_pos]);
							if (e != NULL) e->tab_index = label_tab_ids[count_params_pos];
						}
						
						ui_editText(element->visual, labels[begin_calibration_pos], 0);
						ui_update_required = true;
					}
					else
					{
						current_can_motor_speed = ( (float)spins_val / ((float)initial_time_total / 60) );
						switch_to_start_menu_allowed = false;
						can_procedure_in_progress = true;
						
						uint8_t deselection_array[] = { spins_item_pos, time_item_pos, mass_item_pos, count_params_pos, back_pos };
						for (uint8_t i = 0; i < sizeof(deselection_array); i++)
						{
							e = ui_findVisualById(screen, label_ids[deselection_array[i]]);
							if (e != NULL) e->tab_index = 0;
						}
						
						ui_editText(element->visual, "Остановить", 0);
						ui_update_required = true;
					}
					break;
				}
				case count_params_id:
				{
					UI_Element_Visual *e;	
					
					counted_mass_val = round( (float)mass_val / (float)spins_val * ((float)total_motor_movement_time / 1000) / (float)initial_time_total );
					counted_min_speed_val = 2;
					counted_max_speed_val = 12;
					
					user_mass_per_turn = counted_mass_val;
					user_min_speed = counted_min_speed_val;
					user_max_speed = counted_max_speed_val;
					
					e = ui_findVisualById(screen, begin_calibration_id);
					if (e != NULL) { e->tab_index = 0; e->id = 0; }
					
					CalibrationMenu_ConvertValToText(screen, counted_mass_val_pos);
					CalibrationMenu_ConvertValToText(screen, counted_min_speed_val_pos);
					CalibrationMenu_ConvertValToText(screen, counted_max_speed_val_pos);
					
					ui_update_required = true;
					break;
				}
				case width_item_id: case quota_item_id: case time_item_id: case spins_item_id: case mass_item_id: case counted_mass_item_id: case min_speed_item_id: case max_speed_item_id:
				{
					int16_t val_pos = CalibrationMenu_GetValPosFromItemId(element_id);
					if ( (val_pos >= element_val_count || val_pos < 0) || val_ptrs[val_pos] == NULL) return;
					
					if (!screen->item_is_selected)
					{
						CalibrationMenu_SlotBlinkUpdate(screen, false);
						ui_selectItem(screen, 0, 1); // Force select
						
						selected_slot = 0;
						if (element_id == time_item_id)
						{
							seconds_selected = seconds_selected_default;
						}
					}
					else
					{
						CalibrationMenu_SlotBlinkUpdate(screen, false);
						
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
											UI_Element_Visual *e = ui_findVisualById(screen, fill_motor_id);
											if (e != NULL) e->tab_index = label_tab_ids[fill_motor_pos];
										}
										else
										{
											UI_Element_Visual *e = ui_findVisualById(screen, fill_motor_id);
											if (e != NULL) e->tab_index = 0;
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
											UI_Element_Visual *e = ui_findVisualById(screen, begin_calibration_id);
											if (e != NULL) e->tab_index = label_tab_ids[begin_calibration_pos];
										}
										else
										{
											UI_Element_Visual *e = ui_findVisualById(screen, begin_calibration_id);
											if (e != NULL) e->tab_index = 0;
										}
										break;
									}
									case mass_item_id:
									{
										if (mass_val != 0)
										{
											UI_Element_Visual *e = ui_findVisualById(screen, count_params_id);
											if (e != NULL) e->tab_index = label_tab_ids[count_params_pos];
										}
										else
										{
											UI_Element_Visual *e = ui_findVisualById(screen, count_params_id);
											if (e != NULL) e->tab_index = 0;
										}
										break;
									}
								}
							}
						}
					}
					break;
				}
			}
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}


void UI_BuildCalibrationMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	seconds_selected = seconds_selected_default;
	selected_slot = 0;
	selected_slot_gone = false;
	total_motor_movement_time = 0;
	
  for (uint8_t i = 0; i < element_text_count; i++)
  {
		uint8_t tab_id = 0;
		if (label_ids[i] == width_item_id || label_ids[i] == quota_item_id || label_ids[i] == back_id)
		{
			tab_id = label_tab_ids[i];
		}
		
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        text_xpos[i],                 // pos_x
        text_ypos[i],            // pos_y
				WHITE,							// color
			  tab_id,              // tab index
				(text_offset_scalers[i] + 3) * CHAR_BASE_WIDTH,  // cursor offset
			  labels[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
		vis->id = label_ids[i];

    // ---------------- Interactable ----------------
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
      vis,
      CalibrationMenu_OnItemPressed
    );
  }
	
  for (uint8_t i = 0; i < element_val_count; i++)
  {
		*val_ptrs[i] = 0;
	}
	width_val = user_seeder_width;
	quota_val = user_quota;
	if (width_val != 0 && quota_val != 0)
	{
		UI_Element_Visual *e = ui_findVisualById(screen, fill_motor_id);
		if (e != NULL) e->tab_index = label_tab_ids[fill_motor_pos];
	}
	
  for (uint8_t i = 0; i < element_val_count; i++)
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
		
		CalibrationMenu_ConvertValToText(screen, i);
  }
	
	
	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = ui_findVisualById(screen, 1);
	
	screen->general_callback = CalibrationMenu_ScreenCallback;
	screen->callback_interval = 50;
	
	main_functionality_active = false;
	switch_to_start_menu_allowed = true;
}
