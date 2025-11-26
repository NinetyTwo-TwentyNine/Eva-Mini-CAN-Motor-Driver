#include <ui_screen_main_menu.h>

static void OnItemPressed(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
    // printf("Pressed: %s\n", element->id);
}


void UI_BuildSimpleMenu(UI_Screen* screen)
{
	clearElements(screen);

	char* labels[4] = { "Item 1", "Item 2", "Item 3", "Item 4" };
	const uint8_t ypos[4] = { 16, 32, 48, 64 };

  for (uint8_t i = 0; i < 4; i++)
  {
    // ---------------- Visual ----------------
		char* label = labels[i];
    UI_Element_Visual* vis = addText(
        screen,
        8,                  // pos_x
        ypos[i],            // pos_y
				1,									// color (white)
			  i + 1,              // tab index
			  label,          		// text
        0                  // font size
    );

    // Optional: assign better ID
    vis->id[0] = 'T';
    vis->id[1] = '0' + (i + 1);
    vis->id[2] = '\0';

    // ---------------- Interactable ----------------
		UI_Element_Interactable* inter = bindInteractable(
			screen,
      vis,
      OnItemPressed
    );

    // Optional: assign interactable ID
    inter->id[0] = 'I';
    inter->id[1] = '0' + (i + 1);
    inter->id[2] = '\0';
  }

  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = &screen->visuals[0];
}
