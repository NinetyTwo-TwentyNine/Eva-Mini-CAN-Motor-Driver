#include <ui_screen_errors_menu.h>

static const uint8_t element_count = 12;
static char *bunker_error_lbl = "Бункер пуст: напол", *fan_error_lbl = "Ошибка вентилятора:", *motor_error_lbl = "Ошибка мотора: отсут", *can_error_lbl = "Ошибка мотора: отсут", *speed_error_lbl = "Ошибка скорости: ско", *quota_error_lbl = "Ошибка высева: норма";
static char *bunker_error_2_lbl = "ните бункер", *fan_error_2_lbl = "скорость вне рамок", *motor_error_2_lbl = "ствует подключение", *can_error_2_lbl = "CAN сигнал", *speed_error_2_lbl = "рость вне рамок", *quota_error_2_lbl = "не равна заданной";
static const uint8_t bunker_error_id = 1, fan_error_id = 2, motor_error_id = 3, can_error_id = 4, speed_error_id = 5, quota_error_id = 6;
static const uint8_t bunker_error_tab_id = 1, fan_error_tab_id = 2, motor_error_tab_id = 3, can_error_tab_id = 4, speed_error_tab_id = 5, quota_error_tab_id = 6;
static uint8_t xpos[element_count] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t ypos[element_count] = { 6, 18, 38, 50, 70, 82, 102, 114, 134, 146, 166, 178 };

static void ErrorsMenu_ScreenCallback(UI_Screen* screen)
{
	// TODO: handle error updates in real time
}


void UI_BuildErrorsMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	char* labels[element_count] = { bunker_error_lbl, bunker_error_2_lbl, fan_error_lbl, fan_error_2_lbl, motor_error_lbl, motor_error_2_lbl, can_error_lbl, can_error_2_lbl, speed_error_lbl, speed_error_2_lbl, quota_error_lbl, quota_error_2_lbl };
	uint8_t ids[element_count / 2] = { bunker_error_id, fan_error_id, motor_error_id, can_error_id, speed_error_id, quota_error_id };
	uint8_t tab_ids[element_count / 2] = { bunker_error_tab_id, fan_error_tab_id, motor_error_tab_id, can_error_tab_id, speed_error_tab_id, quota_error_tab_id };
	
  for (uint8_t i = 0; i < element_count / 2; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        xpos[i * 2],                 // pos_x
        ypos[i * 2],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  labels[i * 2],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
		vis->id = 0;
		
    UI_Element_Visual* vis2 = ui_addText(
        screen,
        xpos[i * 2 + 1],                 // pos_x
        ypos[i * 2 + 1],            // pos_y
				WHITE,							// color
			  tab_ids[i],              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  labels[i * 2 + 1],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );
		
		// Optional: assign y offset down
		vis2->offset_y_up = CHAR_BASE_HEIGHT * 3 / 2;

    // Optional: assign visual ID
		vis2->id = ids[i];
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
  if (screen->visuals_count > 0)
    screen->hovered = &screen->visuals[1];
	
	screen->callback_interval = 100;
	screen->general_callback = ErrorsMenu_ScreenCallback;
	
	main_functionality_active = true;
	switch_to_start_menu_allowed = true;
}
