#include <ui_screen_start_menu.h>

static const uint8_t element_count = 4;
static char *main_menu_lbl = "Главная", *seeder_options_lbl = "Настройки высева", *errors_lbl = "Ошибки", *other_options_lbl = "Настройки";
static uint8_t main_menu_id = 1, seeder_options_id = 2, errors_id = 3, other_options_id = 4;
static uint8_t xpos[] = { 40, 16, 44, 36 };
static uint8_t ypos[] = { 4, 20, 36, 52 };

static void StartMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			break;
		case PRESS_TYPE_OK:
			if (element->visual->id == 0)
				break;
			
			if (element->visual->id == main_menu_id)
			{
				UI_BuildMainMenu(screen);
			}
			else if (element->visual->id == seeder_options_id)
			{
				UI_BuildSeederOptionsMenu(screen);
			}
			else if (element->visual->id == errors_id)
			{
				UI_BuildErrorsMenu(screen);
			}
			else if (element->visual->id == other_options_id)
			{
				UI_BuildOptionsMenu(screen);
			}
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}


void UI_BuildStartMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	char* labels[element_count] = { main_menu_lbl, seeder_options_lbl, errors_lbl, other_options_lbl };
	uint8_t ids[element_count] = { main_menu_id, seeder_options_id, errors_id, other_options_id };

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
      StartMenu_OnItemPressed
    );

    // Optional: assign interactable ID
    //inter->id = 0;
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 0;
	
  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = &screen->visuals[0];
	
	switch_to_start_menu_allowed = false;
}
