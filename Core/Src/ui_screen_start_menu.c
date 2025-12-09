#include <ui_screen_start_menu.h>

#define SM_ELEMENT_COUNT 4

static char *labels[SM_ELEMENT_COUNT] = { "Главная", "Настройки высева", "Ошибки", "Настройки" };
static const uint8_t main_menu_id = 1, seeder_options_id = 2, errors_id = 3, other_options_id = 4;
static uint8_t xpos[SM_ELEMENT_COUNT] = { 40, 16, 44, 36 };
static uint8_t ypos[SM_ELEMENT_COUNT] = { 4, 20, 36, 52 };
static uint8_t label_ids[SM_ELEMENT_COUNT] = { main_menu_id, seeder_options_id, errors_id, other_options_id };

static void StartMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	
	switch(press_type)
	{
		case PRESS_TYPE_OK:
			switch (element->visual->id)
			{
				case main_menu_id: UI_BuildMainMenu(screen); break;
				case seeder_options_id: UI_BuildSeederOptionsMenu(screen); break;
				case errors_id: UI_BuildErrorsMenu(screen); break;
				case other_options_id: UI_BuildOptionsMenu(screen); break;
			}
			break;
		default: return;
	}
}


void UI_BuildStartMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);

  for (uint8_t i = 0; i < SM_ELEMENT_COUNT; i++)
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
		vis->id = label_ids[i];

    // ---------------- Interactable ----------------
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
      vis,
      StartMenu_OnItemPressed, NULL, NULL
    );
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 0;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	//main_functionality_active = true;
	switch_to_start_menu_allowed = false;
}
