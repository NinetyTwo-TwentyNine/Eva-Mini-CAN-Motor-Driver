#include <ui_screen_seeder_options_menu.h>

static const uint8_t element_count = 4;
static char *calibration_lbl = "Калибровка", *calibration_check_lbl = "Проверка", *calibration_check_2_lbl = "калибровки", *back_lbl = "назад";
static const uint8_t calibration_id = 1, calibration_check_id = 0, calibration_check_2_id = 2, back_id = 3;
static const uint8_t calibration_tab_id = 1, calibration_check_tab_id = 0, calibration_check_2_tab_id = 2, back_tab_id = 3;
static uint8_t xpos[element_count] = { 8, 8, 8, 8 };
static uint8_t ypos[element_count] = { 4, 22, 34, 52 };

static void SeederOptionsMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			break;
		case PRESS_TYPE_OK:
			switch(element->visual->id)
			{
				case calibration_id: UI_BuildCalibrationMenu(screen); break;
				case back_id: UI_BuildStartMenu(screen); break;
			}
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}


void UI_BuildSeederOptionsMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	char* labels[element_count] = { calibration_lbl, calibration_check_lbl, calibration_check_2_lbl, back_lbl };
	int8_t ids[element_count] = { calibration_id, calibration_check_id, calibration_check_2_id, back_id };
	int8_t tab_ids[element_count] = { calibration_tab_id, calibration_check_tab_id, calibration_check_2_tab_id, back_tab_id };

  for (uint8_t i = 0; i < element_count; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        xpos[i],                 // pos_x
        ypos[i],            // pos_y
				WHITE,							// color
			  tab_ids[i],              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  labels[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
		vis->id = ids[i];

    // ---------------- Interactable ----------------
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
      vis,
      SeederOptionsMenu_OnItemPressed
    );

    // Optional: assign interactable ID
    //inter->id = 0;
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = &screen->visuals[0];
	
	main_functionality_active = true;
	switch_to_start_menu_allowed = true;
}
