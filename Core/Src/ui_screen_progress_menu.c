#include <ui_screen_progress_menu.h>

#define PM_ELEMENT_TEXT_COUNT 4
#define PM_POS_SCREEN_LABEL 0
#define PM_POS_TOTAL_AREA_ITEM 1
#define PM_POS_SESSION_AREA_ITEM 2
#define PM_POS_BACK 3

static char *labels[PM_ELEMENT_TEXT_COUNT] = { "Наработка:", "", "За сеанс:", "назад" };
static const uint8_t screen_label_id = 1, total_area_id = 2, session_area_id = 3, back_id = 4;
static uint8_t xpos[PM_ELEMENT_TEXT_COUNT] = { 8, 8, 8, 8 };
static uint8_t ypos[PM_ELEMENT_TEXT_COUNT] = { 4, 20, 36, 52 };
static uint8_t text_offset_scalers[PM_ELEMENT_TEXT_COUNT] = { 0, 8, 8, 0 };
static uint8_t label_ids[PM_ELEMENT_TEXT_COUNT] = { screen_label_id, total_area_id, session_area_id, back_id };
static uint8_t label_tab_ids[PM_ELEMENT_TEXT_COUNT] = { 0, 2, 3, 4 };

#define PM_ELEMENT_VAL_COUNT 2
#define PM_POS_AREA_TOTAL_VAL 0
#define PM_POS_AREA_SESSION_VAL 1

static uint32_t area_total_val, area_session_val;
static uint32_t* val_ptrs[PM_ELEMENT_VAL_COUNT] = { &area_total_val, &area_session_val };
static char* val_types[PM_ELEMENT_VAL_COUNT] = { "га", "га" };
static uint8_t val_xpos[PM_ELEMENT_VAL_COUNT] = { 8, 62 };
static uint8_t val_ypos[PM_ELEMENT_VAL_COUNT] = { 20, 36 };
static uint8_t val_allowed_lengths[PM_ELEMENT_VAL_COUNT] = { 4, 4 };
static uint8_t val_lengths_after_dot[PM_ELEMENT_VAL_COUNT] = { 1, 2 };
static const uint8_t total_area_val_id = 21, session_area_val_id = 22;
static uint8_t val_ids[PM_ELEMENT_VAL_COUNT] = { total_area_val_id, session_area_val_id };

//==================================
// Helpers
//==================================

static uint8_t PMHelper_CheckValPosValidity(int16_t val_pos)
{
	if ( (val_pos >= PM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return 0;
	
	return 1;
}

static void PMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	if (!PMHelper_CheckValPosValidity(val_pos)) return;
	
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos], length_after_dot = val_lengths_after_dot[val_pos];
	uint32_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	utils_val_to_text_converter(final_string, length, length_after_dot, value, type, 0, false);
	ui_editText(e, final_string, e->data.text.font);
}

//==================================
// Main functionality
//==================================

static void ProgressMenu_ScreenCallback(UI_Screen* screen)
{
	area_total_val = floor(getUserParameterFloat(USER_PARAM_AREA_TOTAL) * getPow10(val_lengths_after_dot[PM_POS_AREA_TOTAL_VAL]));
	area_session_val = floor(getUserParameterFloat(USER_PARAM_AREA_SESSION) * getPow10(val_lengths_after_dot[PM_POS_AREA_SESSION_VAL]));
	
	PMHelper_ConvertValToText(screen, PM_POS_AREA_TOTAL_VAL);
	PMHelper_ConvertValToText(screen, PM_POS_AREA_SESSION_VAL);
	
	ui_update_required = true;
}

static void ProgressMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch(press_type)
	{
		case PRESS_TYPE_OK:
			switch (element_id)
			{
				case session_area_id:
				{
					setUserParameterFloat(USER_PARAM_AREA_SESSION, 0);
					
					PMHelper_ConvertValToText(screen, PM_POS_AREA_SESSION_VAL);
	
					ui_update_required = true;
					
					break;
				}
				case back_id: UI_BuildOptionsMenu(screen); break;
			}
			break;
		default: return;
	}
}

//==================================
// Initial setup
//==================================

void UI_BuildProgressMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);

  for (uint8_t i = 0; i < PM_ELEMENT_TEXT_COUNT; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        xpos[i],                 // pos_x
        ypos[i],            // pos_y
				WHITE,							// color
			  label_tab_ids[i],              // tab index
				(text_offset_scalers[i] + 3) * CHAR_BASE_WIDTH,  // cursor offset
			  labels[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
		vis->id = label_ids[i];

    // ---------------- Interactable ----------------
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
			vis,
			ProgressMenu_OnItemPressed_Main, NULL, NULL
		);
  }
	
	area_total_val = floor(getUserParameterFloat(USER_PARAM_AREA_TOTAL) * getPow10(val_lengths_after_dot[PM_POS_AREA_TOTAL_VAL]));
	area_session_val = floor(getUserParameterFloat(USER_PARAM_AREA_SESSION) * getPow10(val_lengths_after_dot[PM_POS_AREA_SESSION_VAL]));
	
  for (uint8_t i = 0; i < PM_ELEMENT_VAL_COUNT; i++)
  {
    // ---------------- Visual ----------------
    UI_Element_Visual* vis = ui_addText(
        screen,
        val_xpos[i],                 // pos_x
        val_ypos[i],            // pos_y
				WHITE,							// color
			  0,              // tab index
				-1,  // cursor offset
			  "",          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );
		
    // Optional: assign visual ID
		vis->id = val_ids[i];
		
		PMHelper_ConvertValToText(screen, i);
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	screen->general_callback = ProgressMenu_ScreenCallback;
	screen->callback_interval = 50;
}
