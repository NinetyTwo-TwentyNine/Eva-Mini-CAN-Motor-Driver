#include <ui_screen_main_menu.h>

#define MM_ELEMENT_TEXT_COUNT 7
#define MM_POS_ERROR_ITEM 0
#define MM_POS_QUOTA_ITEM 1
#define MM_POS_FAN_ITEM 2
#define MM_POS_MOTOR_ITEM 3
#define MM_POS_SPEED_ITEM 4
#define MM_POS_AREA_ITEM 5
#define MM_POS_SEEDER_ITEM 6

static const uint8_t error_item_id = 1, norm_item_id = 2, fan_item_id = 3, motor_item_id = 4, speed_item_id = 5, area_item_id = 6, seeder_item_id = 7;
static char *texts[MM_ELEMENT_TEXT_COUNT] = { "", "Норма:", "В:", "М:", "С:", "S:", "Т" };
static uint8_t text_xpos[MM_ELEMENT_TEXT_COUNT] = { 16, 4, 4, 4, 4, 80, 115 };
static uint8_t text_ypos[MM_ELEMENT_TEXT_COUNT] = { 4, 16, 28, 40, 52, 52, 40 };
static uint8_t label_ids[MM_ELEMENT_TEXT_COUNT] = { error_item_id, norm_item_id, fan_item_id, motor_item_id, speed_item_id, area_item_id, seeder_item_id };


#define MM_ELEMENT_VAL_COUNT 5
#define MM_POS_QUOTA_VAL 0
#define MM_POS_FAN_VAL 1
#define MM_POS_MOTOR_VAL 2
#define MM_POS_SPEED_VAL 2
#define MM_POS_AREA_VAL 3

static uint32_t quota_val, fan_speed_val, motor_speed_val, speed_val, area_session_val;
static uint32_t* val_ptrs[MM_ELEMENT_VAL_COUNT] = { &quota_val, &fan_speed_val, &motor_speed_val, &speed_val, &area_session_val };
static char *val_types[] = { "кг/га", "об/мин", "об/мин", "км/ч", "га" };
static uint8_t val_allowed_lengths[MM_ELEMENT_VAL_COUNT] = { 3, 4, 3, 3, 1 };
static uint8_t val_lengths_after_dot[MM_ELEMENT_VAL_COUNT] = { 0, 0, 0, 1, 0 };
static uint8_t val_xpos[MM_ELEMENT_VAL_COUNT] = { 42, 16, 16, 16, 92 };
static uint8_t val_ypos[MM_ELEMENT_VAL_COUNT] = { 16, 28, 40, 52, 52 };
static const uint8_t norm_val_id = 21, fan_val_id = 22, motor_val_id = 23, speed_val_id = 24, area_val_id = 25;
static uint8_t val_ids[MM_ELEMENT_VAL_COUNT] = { norm_val_id, fan_val_id, motor_val_id, speed_val_id, area_val_id };


#define MM_ELEMENT_ICON_COUNT 2
#define MM_POS_ERROR_ICON 0
#define MM_POS_SEEDER_ICON 1

static uint8_t* bitmaps[MM_ELEMENT_ICON_COUNT] = {logo_ok_mark, logo_seeder_state};
static uint8_t icon_width[MM_ELEMENT_ICON_COUNT] = {LOGO_OK_MARK_WIDTH, LOGO_SEEDER_STATE_WIDTH};
static uint8_t icon_height[MM_ELEMENT_ICON_COUNT] = {LOGO_OK_MARK_HEIGHT, LOGO_SEEDER_STATE_HEIGHT};
static uint8_t icon_xpos[MM_ELEMENT_ICON_COUNT] = { 4, 105 };
static uint8_t icon_ypos[MM_ELEMENT_ICON_COUNT] = { 2, 38 };
static const uint8_t error_icon_id = 31, seeder_icon_id = 32;
static uint8_t icon_ids[MM_ELEMENT_ICON_COUNT] = { error_icon_id, seeder_icon_id };

//==================================
// Helpers
//==================================

static uint8_t MMHelper_CheckValPosValidity(int16_t val_pos)
{
	if ( (val_pos >= MM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return 0;
	
	return 1;
}

static void MMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	if (!MMHelper_CheckValPosValidity(val_pos)) return;
	
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos], length_after_dot = val_lengths_after_dot[val_pos];;
	uint32_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	if (val_pos == MM_POS_QUOTA_VAL)
	{
		char actual_val_string[UI_ELEMENT_MAX_CHAR_COUNT / 2] = "";
		char theoretical_val_string[UI_ELEMENT_MAX_CHAR_COUNT / 2] = "";
		utils_val_to_text_converter(actual_val_string, length, 0, value, "", 0, false);
		utils_val_to_text_converter(theoretical_val_string, length, 0, getUserParameter(USER_PARAM_QUOTA), "", 0, false);
		
		strcat(final_string, actual_val_string);
		strcat(final_string, "("); strcat(final_string, theoretical_val_string); strcat(final_string, ")");
		strcat(final_string, type);
	}
	else
	{
		utils_val_to_text_converter(final_string, length, length_after_dot, value, type, 0, false);
	}
	ui_editText(e, final_string, e->data.text.font);
}

static void MMHelper_UpdateCurrentParams(UI_Screen* screen)
{
	quota_val = round(current_quota * (10^val_lengths_after_dot[MM_POS_QUOTA_VAL]));
	fan_speed_val = round(current_fan_speed * (10^val_lengths_after_dot[MM_POS_FAN_VAL]));
	motor_speed_val = round(current_actual_motor_speed * (10^val_lengths_after_dot[MM_POS_MOTOR_VAL]));
	speed_val = round(current_seeder_speed * (10^val_lengths_after_dot[MM_POS_SPEED_VAL]));
	area_session_val = floor(current_user_area_session * (10^val_lengths_after_dot[MM_POS_AREA_VAL]));
}

//==================================
// Main functionality
//==================================

static void MainMenu_ScreenCallback(UI_Screen* screen)
{
	MMHelper_UpdateCurrentParams(screen);
	
	for (uint8_t i = 0; i < MM_ELEMENT_VAL_COUNT; i++)
	{
		MMHelper_ConvertValToText(screen, i);
	}
	
	uint8_t errors_present = 0, chosen_error = 0;
	uint64_t time_comparison = 0;
	for (uint8_t i = 0; i < ERROR_COUNT_TOTAL; i++)
	{
		if (!error_state_array[ERROR_STATE_ACTIVE][i]) continue;
		
		errors_present = 1;
		if (error_on_time[i] > time_comparison)
		{
			time_comparison = error_on_time[i];
			chosen_error = i;
		}
	}
	
	UI_Element_Visual *bitmap = ui_findVisualById(screen, error_icon_id), *text = ui_findVisualById(screen, error_item_id);
	if (bitmap != NULL && text != NULL)
	{
		if (errors_present)
		{
			ui_editBitmap(bitmap, LOGO_ERROR_ALERT_WIDTH, LOGO_ERROR_ALERT_HEIGHT, logo_error_alert);
			switch(chosen_error)
			{
				case ERROR_TYPE_FAN: ui_editText(text, "Ошибка вент-ра", 0); break;
				case ERROR_TYPE_MOTOR: ui_editText(text, "Подключ. мотора", 0); break;
				case ERROR_TYPE_CAN: ui_editText(text, "CAN мотора", 0); break;
				case ERROR_TYPE_SPEED: ui_editText(text, "Скорость движения", 0); break;
				case ERROR_TYPE_QUOTA: ui_editText(text, "Норма высева", 0); break;
				case ERROR_TYPE_EMPTY: ui_editText(text, "Бункер пуст", 0); break;
				default: ui_editText(text, "Есть ошибки", 0);
			}
		}
		else if (!checkIfAllUserParamsAreSet())
		{
			ui_editBitmap(bitmap, LOGO_QUESTION_MARK_WIDTH, LOGO_QUESTION_MARK_HEIGHT, logo_question_mark);
			ui_editText(text, "Настройки", 0);
		}
		else
		{
			ui_editBitmap(bitmap, LOGO_OK_MARK_WIDTH, LOGO_OK_MARK_HEIGHT, logo_ok_mark);
			ui_editText(text, "ОК", 0);
		}
	}
	
	bitmap = ui_findVisualById(screen, seeder_icon_id);
	text = ui_findVisualById(screen, seeder_item_id);
	if (bitmap != NULL && text != NULL)
	{
		if (current_state_seeder_down)
		{
			ui_editText(text, "Р", 0);
		}
		else
		{
			ui_editText(text, "Т", 0);
		}
	}
	
	ui_update_required = true;
}

//==================================
// Initial setup
//==================================

void UI_BuildMainMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);

  for (uint8_t i = 0; i < MM_ELEMENT_TEXT_COUNT; i++)
  {
    // ---------------- Visual (Text) ----------------
    UI_Element_Visual* text_elem = ui_addText(
        screen,
        text_xpos[i],            // pos_x
        text_ypos[i],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  texts[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
    text_elem->id = label_ids[i];
	}
	
	MMHelper_UpdateCurrentParams(screen);
	
	for (uint8_t i = 0; i < MM_ELEMENT_VAL_COUNT; i++)
	{
    // ---------------- Visual (Value) ----------------
    UI_Element_Visual* val_elem = ui_addText(
        screen,
        val_xpos[i],            // pos_x
        val_ypos[i],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  "",          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
    val_elem->id = val_ids[i];
		
		MMHelper_ConvertValToText(screen, i);
  }
	
	
	for (uint8_t i = 0; i < MM_ELEMENT_ICON_COUNT; i++)
	{
		UI_Element_Visual* icon_elem = ui_addBitmap(
      screen,
      icon_xpos[i],                 // pos_x
      icon_ypos[i],            			// pos_y
			WHITE,							// color
      icon_width[i], 	// width
			icon_height[i],	// height
			bitmaps[i]		// bitmap
		);
		icon_elem->id = icon_ids[i];
	}
	
	screen->should_draw_cursor = false;
  // Default hover index: None
	
	screen->general_callback = MainMenu_ScreenCallback;
	screen->callback_interval = 100;
	
	setCurrentLogicState(LSTATE_MAIN_LOGIC);
	
	screen->general_callback(screen);
	ui_last_callback_time = sys_timer;
}
