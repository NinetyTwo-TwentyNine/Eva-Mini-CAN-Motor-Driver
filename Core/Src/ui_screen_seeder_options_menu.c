#include <ui_screen_seeder_options_menu.h>

#define SOM_ELEMENT_COUNT 4

static char* labels[SOM_ELEMENT_COUNT] = { "Калибровка", "Проверка", "калибровки", "назад" };
static const uint8_t calibration_id = 1, calibration_check_id = 2, calibration_check_2_id = 3, back_id = 4;
static uint8_t xpos[SOM_ELEMENT_COUNT] = { 8, 8, 8, 8 };
static uint8_t ypos[SOM_ELEMENT_COUNT] = { 4, 22, 34, 52 };
static uint8_t tab_ids[SOM_ELEMENT_COUNT] = { 1, 0, 2, 3 };
static uint8_t ids[SOM_ELEMENT_COUNT] = { calibration_id, calibration_check_id, calibration_check_2_id, back_id };

static void SeederOptionsMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	
	switch(press_type)
	{
		case PRESS_TYPE_OK:
			switch(element->visual->id)
			{
				case calibration_id: UI_BuildCalibrationMenu(screen); break;
				case calibration_check_2_id: UI_BuildCalibrationCheckMenu(screen); break;
				case back_id: UI_BuildStartMenu(screen); break;
			}
			break;
		default: return;
	}
}


void UI_BuildSeederOptionsMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);

  for (uint8_t i = 0; i < SOM_ELEMENT_COUNT; i++)
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
		if (ids[i] != calibration_check_2_id || (user_max_speed != 0 && user_mass_per_turn != 0 && user_quota != 0 && user_seeder_width != 0))
		{
			UI_Element_Interactable* inter = ui_bindInteractable(
				screen,
				vis,
				SeederOptionsMenu_OnItemPressed, NULL, NULL
			);
		}
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	//main_functionality_active = true;
	switch_to_start_menu_allowed = true;
}
