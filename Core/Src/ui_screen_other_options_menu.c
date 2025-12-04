#include <ui_screen_other_options_menu.h>

static const uint8_t element_count = 4;
static char *fan_sensor_lbl = "Датчик вентилятора", *speed_sensor_lbl = "Датчик скорости", *quota_check_lbl = "Наработка", *back_lbl = "назад";
static int8_t fan_sensor_id = 1, speed_sensor_id = 2, quota_check_id = 3, back_id = 4;
static uint8_t xpos[] = { 4, 4, 4, 4 };
static uint8_t ypos[] = { 4, 20, 36, 52 };

static void OptionsMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			break;
		case PRESS_TYPE_OK:
			if (element->visual->id == 0)
				break;
			
			if (element->visual->id == back_id)
			{
				UI_BuildStartMenu(screen);
			}
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}


void UI_BuildOptionsMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	char* labels[element_count] = { fan_sensor_lbl, speed_sensor_lbl, quota_check_lbl, back_lbl };
	int8_t ids[element_count] = { fan_sensor_id, speed_sensor_id, quota_check_id, back_id };

  for (uint8_t i = 0; i < element_count; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        xpos[i],                 // pos_x
        ypos[i],            // pos_y
				WHITE,							// color
			  i + 1,              // tab index
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
      OptionsMenu_OnItemPressed
    );

    // Optional: assign interactable ID
    //inter->id = 0;
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = &screen->visuals[0];
	
	switch_to_start_menu_allowed = true;
}
