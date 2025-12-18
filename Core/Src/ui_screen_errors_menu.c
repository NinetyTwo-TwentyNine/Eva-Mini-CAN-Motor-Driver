#include <ui_screen_errors_menu.h>

#define EM_ELEMENT_COUNT 12
#define EM_POS_BUNKER_ERROR 0
#define EM_POS_FAN_ERROR 2
#define EM_POS_MOTOR_ERROR 4
#define EM_POS_CAN_ERROR 6
#define EM_POS_SPEED_ERROR 8
#define EM_POS_QUOTA_ERROR 10

static char *error_labels[EM_ELEMENT_COUNT] = { "Бункер пуст: напол", "ните бункер", "Ошибка вентилятора:", "скорость вне рамок", "Ошибка мотора: отсут", "ствует подключение", "Ошибка мотора: отсут", "свтует CAN сигнал", "Ошибка скорости: ско", "рость вне рамок", "Ошибка высева: норма", "не равна заданной" };
static uint8_t xpos[EM_ELEMENT_COUNT] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
static uint8_t ypos[EM_ELEMENT_COUNT] = { 6, 18, 38, 50, 70, 82, 102, 114, 134, 146, 166, 178 };


//==================================
// Helpers
//==================================

static int16_t EMHelper_GetErrorPosFromType(uint8_t error_type)
{
	int16_t error_pos = -1;
	
	if (error_type > ERROR_COUNT_TOTAL)
	{
		return error_pos;
	}
	
	switch(error_type)
	{
		case ERROR_TYPE_EMPTY:
			return EM_POS_BUNKER_ERROR;
		case ERROR_TYPE_FAN:
			return EM_POS_FAN_ERROR;
		case ERROR_TYPE_MOTOR:
			return EM_POS_MOTOR_ERROR;
		case ERROR_TYPE_CAN:
			return EM_POS_CAN_ERROR;
		case ERROR_TYPE_SPEED:
			return EM_POS_SPEED_ERROR;
		case ERROR_TYPE_QUOTA:
			return EM_POS_QUOTA_ERROR;
		default:
			return error_pos;
	}
}

//==================================
// Main functionality
//==================================

static void ErrorsMenu_ScreenCallback(UI_Screen* screen)
{
	uint8_t errors_free = true;
	
	for (uint8_t i = 0; i < screen->visuals_count; i++)
	{
		ui_editText(&(screen->visuals[i]), "", 0);
		screen->visuals[i].tab_index = 0;
	}
	
	uint8_t error_count = 0;
	int16_t error_curr_array[ERROR_COUNT_TOTAL];
	for (uint8_t i = 0; i < ERROR_COUNT_TOTAL; i++)
	{
		if (error_state_array[ERROR_STATE_ACTIVE][i])
		{
			errors_free = false;
			
			error_curr_array[error_count++] = i;
		}
	}
	
	uint8_t n = error_count;
	while (n > 1)
	{
    uint8_t new_n = 0;
    for (uint8_t i = 0; i + 1 < n; i++)
    {
			if (error_on_time[error_curr_array[i]] < error_on_time[error_curr_array[i + 1]])
      {
				swap(error_curr_array[i], error_curr_array[i + 1]);
				new_n = i + 1;
			}
		}
		n = new_n;
	}
	
	for (uint8_t i = 0; i < error_count; i++)
	{
		int16_t error_pos = EMHelper_GetErrorPosFromType(error_curr_array[i]);
		if (error_pos == -1) continue;

		char *error_text1, *error_text2;
    error_text1 = error_labels[error_pos];
    error_text2 = error_labels[error_pos + 1];
      
    ui_editText(&(screen->visuals[i * 2]), error_text1, 0);
    ui_editText(&(screen->visuals[i * 2 + 1]), error_text2, 0);
		
		screen->visuals[i * 2 + 1].tab_index = i + 1;
	}
	
	if (errors_free)
	{
		ui_editText(&(screen->visuals[0]), "Сейчас ошибок нету", 0);
		screen->visuals[0].tab_index = 1;
		screen->hovered = &(screen->visuals[0]);
	}
	else
	{
		if (screen->hovered == NULL || screen->hovered->tab_index == 0)
		{
			if (screen->hovered != NULL && screen->hovered->id % 2 == 0)
			{
				UI_Element_Visual *e = ui_findVisualById(screen, screen->hovered->id + 1);
				if (e == NULL || e->tab_index == 0)
				{
					ui_hoverNext(screen, 0);
				}
				else
				{
					screen->hovered = e;
				}
			}
			else
			{
				ui_hoverNext(screen, 0);
			}
		}
	}
	
	ui_update_required = true;
}

//==================================
// Initial setup
//==================================

void UI_BuildErrorsMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
  for (uint8_t i = 0; i < EM_ELEMENT_COUNT / 2; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        xpos[i * 2],            // pos_x
        ypos[i * 2],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  "",          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
		vis->id = i * 2;
		
    UI_Element_Visual* vis2 = ui_addText(
        screen,
        xpos[i * 2 + 1],                 // pos_x
        ypos[i * 2 + 1],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  "",          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );
		
		// Optional: assign y offset down
		vis2->offset_y_up = CHAR_BASE_HEIGHT * 3 / 2;

    // Optional: assign visual ID
		vis2->id = i * 2 + 1;
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index: None
	
	screen->callback_interval = 100;
	screen->general_callback = ErrorsMenu_ScreenCallback;
	
	screen->general_callback(screen);
	ui_last_callback_time = sys_timer;
}
