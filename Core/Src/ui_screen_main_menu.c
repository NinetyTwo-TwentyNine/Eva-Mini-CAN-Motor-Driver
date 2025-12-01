#include <ui_screen_main_menu.h>

static void MainMenu_OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			if (screen->item_is_selected)
			{
				// Handle action
			}
			break;
		case PRESS_TYPE_OK:
			ui_selectItem(screen, 1, 0); // Toggle
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}


void UI_BuildMainMenu(UI_Screen* screen)
{
	ui_clearElements(screen);

	char* labels[5] = { "Итем 1", "Item 2", "Item 3", "Item 4", "Item 25" };
	const uint8_t ypos[5] = { 4, 20, 36, 52, 68 };

  for (uint8_t i = 0; i < 5; i++)
  {
    // ---------------- Visual ----------------
		char* label = utf8rus(labels[i]);
    UI_Element_Visual* vis = ui_addText(
        screen,
        50,                 // pos_x
        ypos[i],            // pos_y
				WHITE,							// color
			  i + 1,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  label,          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
    vis->id = 20 + i;

    // ---------------- Interactable ----------------
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
      vis,
      MainMenu_OnItemPressed
    );

    // Optional: assign interactable ID
		//inter->id = -1;
  }
	
  UI_Element_Visual* vis = ui_addBitmap(
      screen,
      20,                 // pos_x
      36,            			// pos_y
			WHITE,							// color
      LOGO_SEEDER_STATE_WIDTH, 	// width
			LOGO_SEEDER_STATE_HEIGHT,	// height
			logo_seeder_state		// bitmap
  );
	
	vis = ui_addBitmap(
      screen,
      20,                 // pos_x
      52,            			// pos_y
			WHITE,							// color
      LOGO_ERROR_ALERT_WIDTH, 	// width
			LOGO_ERROR_ALERT_HEIGHT,	// height
			logo_error_alert		// bitmap
  );
	

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = &screen->visuals[0];
}
