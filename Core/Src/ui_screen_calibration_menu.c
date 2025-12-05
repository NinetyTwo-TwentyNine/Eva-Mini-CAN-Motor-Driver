#include <ui_screen_calibration_menu.h>

static const uint8_t element_text_count = 12;
static char* labels[element_text_count] = { "Ширина:", "Норма:", "Заполнить катушки", "Время:", "Об.мотора:", "Начать калибровку", "Масса:", "Рассчитать массу", "Масса:", "MIN:", "MAX:", "назад" };
static uint8_t text_xpos[element_text_count] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t text_ypos[element_text_count] = { 4, 20, 36, 52, 68, 84, 100, 116, 132, 148, 164, 180 };
static uint8_t text_offset_scalers[element_text_count] = { 3, 8, 0, 8, 4, 0, 7, 0, 10, 6, 6, 0 };
static const uint8_t width_item_id = 1, quota_item_id = 2, fill_motor_id = 3, time_item_id = 4, spins_item_id = 5, begin_calibration_id = 6, mass_item_id = 7, count_params_id = 8, counted_mass_item_id = 9, min_speed_item_id = 10, max_speed_item_id = 11, back_id = 12;
static const uint8_t width_item_tab_id = 1, quota_item_tab_id = 2, fill_motor_tab_id = 3, time_item_tab_id = 4, spins_item_tab_id = 5, begin_calibration_tab_id = 6, mass_item_tab_id = 7, count_params_tab_id = 8, counted_mass_item_tab_id = 0, min_speed_item_tab_id = 0, max_speed_item_tab_id = 0, back_tab_id = 12;

static uint8_t label_ids[element_text_count] = { width_item_id, quota_item_id, fill_motor_id, time_item_id, spins_item_id, begin_calibration_id, mass_item_id, count_params_id, counted_mass_item_id, min_speed_item_id, max_speed_item_id, back_id };
static uint8_t label_tab_ids[element_text_count] = { width_item_tab_id, quota_item_tab_id, fill_motor_tab_id, time_item_tab_id, spins_item_tab_id, begin_calibration_tab_id, mass_item_tab_id, count_params_tab_id, counted_mass_item_tab_id, min_speed_item_tab_id, max_speed_item_tab_id, back_tab_id };

static const uint8_t element_val_count = 9;
static uint16_t width_val, quota_val, time_min_val, time_sec_val, spins_val, mass_val, counted_mass_val, counted_min_speed_val, counted_max_speed_val;
static char* val_types[element_val_count] = { "м", "кг/га", "мин", "с", "об", "кг", "кг/об", "км/ч", "км/ч" };
static uint8_t val_xpos[element_val_count] = { 46, 40, 40, 70, 64, 40, 40, 28, 28 };
static uint8_t val_ypos[element_val_count] = { 4, 20, 52, 52, 68, 100, 132, 148, 164 };
static uint8_t val_allowed_lengths[element_val_count] = { 2, 3, 2, 2, 2, 4, 4, 2, 2 };
static const uint8_t width_val_id = 21, quota_val_id = 22, time_min_val_id = 23, time_sec_val_id = 24, spins_val_id = 25, mass_val_id = 26, counted_mass_val_id = 27, counted_min_speed_val_id = 28, counted_max_speed_val_id = 29;

static uint8_t val_ids[element_val_count] = { width_val_id, quota_val_id, time_min_val_id, time_sec_val_id, spins_val_id, mass_val_id, counted_mass_val_id, counted_min_speed_val_id, counted_max_speed_val_id };
static uint16_t* val_ptrs[element_val_count] = { &width_val, &quota_val, &time_min_val, &time_sec_val, &spins_val, &mass_val, &counted_mass_val, &counted_min_speed_val, &counted_max_speed_val };


static uint8_t selected_slot, seconds_selected, selected_slot_gone;
static const uint8_t seconds_selected_default = true;

static uint64_t time_save = 0;
static const uint16_t slot_blink_period = 400;

static int16_t CalibrationMenu_GetValIdFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case width_item_id: val_pos = 0; break;
		case quota_item_id: val_pos = 1; break;
		case time_item_id: val_pos = 2 + seconds_selected; break;
		case spins_item_id: val_pos = 4; break;
		case mass_item_id: val_pos = 5; break;
		case counted_mass_item_id: val_pos = 6; break;
		case counted_min_speed_val_id: val_pos = 7; break;
		case counted_max_speed_val_id: val_pos = 8; break;
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
		time_save = sys_timer;
		selected_slot_gone = slot_gone;
		
		CalibrationMenu_ConvertValToText(screen, CalibrationMenu_GetValIdFromItemId(screen->hovered->id));
}


static void CalibrationMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save > slot_blink_period)
	{
		CalibrationMenu_SlotBlinkUpdate(screen, !selected_slot_gone);
		ui_update_required = true;
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
				int16_t val_pos = CalibrationMenu_GetValIdFromItemId(element_id);
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
				case back_id: UI_BuildStartMenu(screen); break;
				case width_item_id: case quota_item_id: case time_item_id: case spins_item_id: case mass_item_id: case counted_mass_item_id: case min_speed_item_id: case max_speed_item_id:
				{
					int16_t val_pos = CalibrationMenu_GetValIdFromItemId(element_id);
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
	
  for (uint8_t i = 0; i < element_text_count; i++)
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
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
      vis,
      CalibrationMenu_OnItemPressed
    );

    // Optional: assign interactable ID
    //inter->id = 0;
  }
	
  for (uint8_t i = 0; i < element_val_count; i++)
  {
		*val_ptrs[i] = 0;
		
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
	
	switch_to_start_menu_allowed = false;
}
