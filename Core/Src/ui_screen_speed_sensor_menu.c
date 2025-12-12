#include <ui_screen_speed_sensor_menu.h>

#define SSM_ELEMENT_TEXT_COUNT 4
#define SSM_POS_SCREEN_LABEL 0
#define SSM_POS_DIAMETER_ITEM 1
#define SSM_POS_PULSES_ITEM 2
#define SSM_POS_BACK 3

static char *labels[SSM_ELEMENT_TEXT_COUNT] = { "Датчик скорости", "Д-р колеса:", "К-во импульсов:", "назад" };
static const uint8_t screen_label_id = 1, diameter_item_id = 2, pulses_item_id = 3, back_id = 4;
static uint8_t xpos[SSM_ELEMENT_TEXT_COUNT] = { 4, 4, 4, 4 };
static uint8_t ypos[SSM_ELEMENT_TEXT_COUNT] = { 4, 20, 36, 52 };
static uint8_t text_offset_scalers[SSM_ELEMENT_TEXT_COUNT] = { 0, 3, 2, 0 };
static uint8_t label_ids[SSM_ELEMENT_TEXT_COUNT] = { screen_label_id, diameter_item_id, pulses_item_id, back_id };
static uint8_t label_tab_ids[SSM_ELEMENT_TEXT_COUNT] = { 0, 2, 3, 4 };

#define SSM_ELEMENT_VAL_COUNT 2
#define SSM_POS_WHEEL_DIAMETER_VAL 0
#define SSM_POS_SENSOR_PULSES_VAL 1

static uint32_t diameter_val, pulses_val;
static uint32_t* val_ptrs[SSM_ELEMENT_VAL_COUNT] = { &diameter_val, &pulses_val };
static char* val_types[SSM_ELEMENT_VAL_COUNT] = { "м", "" };
static uint8_t val_xpos[SSM_ELEMENT_VAL_COUNT] = { 70, 94 };
static uint8_t val_ypos[SSM_ELEMENT_VAL_COUNT] = { 20, 36 };
static uint8_t val_allowed_lengths[SSM_ELEMENT_VAL_COUNT] = { 2, 2 };
static uint8_t val_lengths_after_dot[SSM_ELEMENT_VAL_COUNT] = { 1, 0 };
static const uint8_t diameter_val_id = 21, pulses_val_id = 22;
static uint8_t val_ids[SSM_ELEMENT_VAL_COUNT] = { diameter_val_id, pulses_val_id };


static uint8_t selected_slot, selected_slot_gone;
static uint64_t time_save_ui = 0;
static const uint16_t slot_blink_period = 400;

//==================================
// Helpers
//==================================

static int16_t SSMHelper_GetValPosFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case diameter_item_id: val_pos = SSM_POS_WHEEL_DIAMETER_VAL; break;
		case pulses_item_id: val_pos = SSM_POS_SENSOR_PULSES_VAL; break;
	}
	return val_pos;
}

static uint8_t SSMHelper_CheckValPosValidity(int16_t val_pos)
{
	if ( (val_pos >= SSM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return 0;
	
	return 1;
}

static void SSMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	if (!SSMHelper_CheckValPosValidity(val_pos)) return;
	
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos];
	uint32_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	utils_val_to_text_converter(final_string, length, val_lengths_after_dot[val_pos], value, type, selected_slot, selected_slot_gone);
	ui_editText(e, final_string, e->data.text.font);
}

static void SSMHelper_UpdateValStr_SlotBlink(UI_Screen* screen, uint8_t slot_gone)
{
	time_save_ui = sys_timer;
	selected_slot_gone = slot_gone;
		
	SSMHelper_ConvertValToText(screen, SSMHelper_GetValPosFromItemId(screen->hovered->id));
}

//==================================
// Main functionality
//==================================

static void SpeedSensorMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save_ui > slot_blink_period)
	{
		SSMHelper_UpdateValStr_SlotBlink(screen, !selected_slot_gone);
		ui_update_required = true;
	}
}

static void SpeedSensorMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			{
				int16_t val_pos = SSMHelper_GetValPosFromItemId(element_id);
				if (!SSMHelper_CheckValPosValidity(val_pos)) return;
				
				uint32_t *val_ptr = val_ptrs[val_pos];
				utils_edit_value_by_slot(val_ptr, selected_slot, press_type, 0);
				SSMHelper_UpdateValStr_SlotBlink(screen, false);
			}
			break;
		case PRESS_TYPE_OK:
			switch (element_id)
			{
				case back_id: UI_BuildOptionsMenu(screen); break;
			}
			break;
		case PRESS_TYPE_OTHER:
			break;
		default: return;
	}
}

static void SpeedSensorMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case diameter_item_id: case pulses_item_id:
		{
			int16_t val_pos = SSMHelper_GetValPosFromItemId(element_id);
			if (!SSMHelper_CheckValPosValidity(val_pos)) return;
			
			if (!screen->item_is_selected)
			{
				SSMHelper_UpdateValStr_SlotBlink(screen, false);
				ui_selectItem(screen, 0, 1); // Force select
			}
			else
			{
				SSMHelper_UpdateValStr_SlotBlink(screen, false);
				
				selected_slot = (selected_slot + 1) % (val_allowed_lengths[val_pos]);
				if (selected_slot == 0)
				{
						ui_selectItem(screen, 0, 0); // Force deselect
				}
			}
			break;
		}
	}
}

static void SpeedSensorMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case diameter_item_id: case pulses_item_id:
		{
			int16_t val_pos = SSMHelper_GetValPosFromItemId(element_id);
			if (!SSMHelper_CheckValPosValidity(val_pos)) return;
			
			if (screen->item_is_selected)
			{
				selected_slot = 0;
			}
			else
			{
				// Main deselection callback
				switch (element_id)
				{
					case diameter_item_id:
					{
						// TODO: Handle wheel diameter value being saved to FLASH memory
						setUserParameter(USER_PARAM_WHEEL_DIAMETER, diameter_val);
						break;
					}
					case pulses_item_id:
					{
						// TODO: Handle sensor pulses value being saved to FLASH memory
						setUserParameter(USER_PARAM_WHEEL_PULSES, pulses_val);
						break;
					}
				}
			}
		}
	}
}

//==================================
// Initial setup
//==================================

void UI_BuildSpeedSensorMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	selected_slot = 0;
	selected_slot_gone = false;

  for (uint8_t i = 0; i < SSM_ELEMENT_TEXT_COUNT; i++)
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
		UI_Callback main = SpeedSensorMenu_OnItemPressed_Main, selection = NULL, onselect = NULL;
		if (SSMHelper_CheckValPosValidity(SSMHelper_GetValPosFromItemId(label_ids[i])))
		{
			selection = SpeedSensorMenu_OnItemPressed_Selection;
			onselect = SpeedSensorMenu_OnItemPressed_OnSelect;
		}
		
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
			vis,
			main, selection, onselect
		);
  }
	
  for (uint8_t i = 0; i < SSM_ELEMENT_VAL_COUNT; i++)
  {
		*val_ptrs[i] = 0;
	}
	diameter_val = getUserParameter(USER_PARAM_WHEEL_DIAMETER);
	pulses_val = getUserParameter(USER_PARAM_WHEEL_PULSES);
	
  for (uint8_t i = 0; i < SSM_ELEMENT_VAL_COUNT; i++)
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
		
		SSMHelper_ConvertValToText(screen, i);
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	screen->general_callback = SpeedSensorMenu_ScreenCallback;
	screen->callback_interval = 50;
	
	//main_functionality_active = true;
	switch_to_start_menu_allowed = true;
}
