#include <ui_screen_fan_sensor_menu.h>

#define FSM_ELEMENT_TEXT_COUNT 5
#define FSM_POS_SCREEN_LABEL 0
#define FSM_POS_MIN_ITEM 1
#define FSM_POS_MAX_ITEM 2
#define FSM_POS_PULSES_ITEM 3
#define FSM_POS_BACK 4

static char *labels[FSM_ELEMENT_TEXT_COUNT] = { "Датчик вентилятора", "min:", "max:", "К-во импульсов:", "назад" };
static const uint8_t screen_label_id = 1, min_item_id = 2, max_item_id = 3, pulses_item_id = 4, back_id = 5;
static uint8_t xpos[FSM_ELEMENT_TEXT_COUNT] = { 8, 8, 8, 8, 8 };
static uint8_t ypos[FSM_ELEMENT_TEXT_COUNT] = { 4, 20, 36, 52, 68 };
static uint8_t text_offset_scalers[FSM_ELEMENT_TEXT_COUNT] = { 0, 10, 10, 2, 0 };
static uint8_t label_ids[FSM_ELEMENT_TEXT_COUNT] = { screen_label_id, min_item_id, max_item_id, pulses_item_id, back_id };
static uint8_t label_tab_ids[FSM_ELEMENT_TEXT_COUNT] = { 0, 2, 3, 4, 5 };

#define FSM_ELEMENT_VAL_COUNT 3
#define FSM_POS_SENSOR_MIN_VAL 0
#define FSM_POS_SENSOR_MAX_VAL 1
#define FSM_POS_SENSOR_PULSES_VAL 2

static uint32_t min_sensor_val, max_sensor_val, pulses_val;
static uint32_t* val_ptrs[FSM_ELEMENT_VAL_COUNT] = { &min_sensor_val, &max_sensor_val, &pulses_val };
static uint32_t val_allowed_lengths[FSM_ELEMENT_VAL_COUNT] = { 4, 4, 2 };
static char* val_types[FSM_ELEMENT_VAL_COUNT] = { "об/мин", "об/мин", "" };
static uint8_t val_xpos[FSM_ELEMENT_VAL_COUNT] = { 32, 32, 98 };
static uint8_t val_ypos[FSM_ELEMENT_VAL_COUNT] = { 20, 36, 52 };
static const uint8_t min_sensor_val_id = 21, max_sensor_val_id = 22, pulses_val_id = 23;
static uint8_t val_ids[FSM_ELEMENT_VAL_COUNT] = { min_sensor_val_id, max_sensor_val_id, pulses_val_id };


static uint8_t selected_slot, selected_slot_gone;
static uint64_t time_save_ui = 0;
static const uint16_t slot_blink_period = 400;

//==================================
// Helpers
//==================================

static int16_t FSMHelper_GetValPosFromItemId(uint8_t item_id)
{
	int16_t val_pos = -1;
	switch (item_id)
	{
		case min_item_id: val_pos = FSM_POS_SENSOR_MIN_VAL; break;
		case max_item_id: val_pos = FSM_POS_SENSOR_MAX_VAL; break;
		case pulses_item_id: val_pos = FSM_POS_SENSOR_PULSES_VAL; break;
	}
	return val_pos;
}

static uint8_t FSMHelper_CheckValPosValidity(int16_t val_pos)
{
	if ( (val_pos >= FSM_ELEMENT_VAL_COUNT || val_pos < 0) || val_ptrs[val_pos] == NULL) return 0;
	
	return 1;
}

static void FSMHelper_ConvertValToText(UI_Screen* screen, uint8_t val_pos)
{
	if (!FSMHelper_CheckValPosValidity(val_pos)) return;
	
	uint8_t val_id = val_ids[val_pos], length = val_allowed_lengths[val_pos];
	uint32_t value = *val_ptrs[val_pos];
	char* type = val_types[val_pos];
	
	UI_Element_Visual *e = ui_findVisualById(screen, val_id);
	if (e == NULL || e->type != VISUAL_TYPE_TEXT) return;
	
	char final_string[UI_ELEMENT_MAX_CHAR_COUNT] = "";
	utils_val_to_text_converter(final_string, length, 0, value, type, selected_slot, selected_slot_gone);
	ui_editText(e, final_string, e->data.text.font);
}

static void FSMHelper_UpdateValStr_SlotBlink(UI_Screen* screen, uint8_t slot_gone)
{
	time_save_ui = sys_timer;
	selected_slot_gone = slot_gone;
		
	FSMHelper_ConvertValToText(screen, FSMHelper_GetValPosFromItemId(screen->hovered->id));
}

//==================================
// Main functionality
//==================================

static void FanSensorMenu_ScreenCallback(UI_Screen* screen)
{
	if (screen->item_is_selected && screen->hovered != NULL && sys_timer - time_save_ui > slot_blink_period)
	{
		FSMHelper_UpdateValStr_SlotBlink(screen, !selected_slot_gone);
		ui_update_required = true;
	}
}

static void FanSensorMenu_OnItemPressed_Main(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch(press_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			{
				int16_t val_pos = FSMHelper_GetValPosFromItemId(element_id);
				if (!FSMHelper_CheckValPosValidity(val_pos)) return;
				
				uint32_t *val_ptr = val_ptrs[val_pos];
				if (val_pos == FSM_POS_SENSOR_MIN_VAL || val_pos == FSM_POS_SENSOR_MAX_VAL)
				{
					uint32_t max_val = 0, min_val = 0, scaler = 1, allowed_length = val_allowed_lengths[val_pos];
					for (uint8_t i = 0; i < allowed_length; i++)
					{
						max_val += 9 * scaler;
						scaler *= 10;
					}
				
					if (val_pos == FSM_POS_SENSOR_MIN_VAL)
					{
						max_val -= SENSOR_VALUE_FAN_MIN_RANGE;
					}
					else if (val_pos == FSM_POS_SENSOR_MAX_VAL)
					{
						min_val += SENSOR_VALUE_FAN_MIN_RANGE;
					}
					utils_edit_value_by_slot_with_min_max(val_ptr, selected_slot, allowed_length, press_type, min_val, max_val);
				}
				else
				{
					utils_edit_value_by_slot(val_ptr, selected_slot, press_type, 0);
				}
				FSMHelper_UpdateValStr_SlotBlink(screen, false);
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

static void FanSensorMenu_OnItemPressed_Selection(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case min_item_id: case max_item_id: case pulses_item_id:
		{
			int16_t val_pos = FSMHelper_GetValPosFromItemId(element_id);
			if (!FSMHelper_CheckValPosValidity(val_pos)) return;
			
			if (!screen->item_is_selected)
			{
				FSMHelper_UpdateValStr_SlotBlink(screen, false);
				ui_selectItem(screen, 0, 1); // Force select
			}
			else
			{
				FSMHelper_UpdateValStr_SlotBlink(screen, false);
				
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

static void FanSensorMenu_OnItemPressed_OnSelect(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element)
{
	if (element->visual == NULL || element->visual->id == 0) return;
	uint8_t element_id = element->visual->id;
	
	switch (element_id)
	{
		case min_item_id: case max_item_id: case pulses_item_id:
		{
			int16_t val_pos = FSMHelper_GetValPosFromItemId(element_id);
			if (!FSMHelper_CheckValPosValidity(val_pos)) return;
			
			if (screen->item_is_selected)
			{
				selected_slot = 0;
			}
			else
			{
				// Main deselection callback
				switch (element_id)
				{
					case min_item_id:
					{
						if (min_sensor_val + SENSOR_VALUE_FAN_MIN_RANGE > max_sensor_val)
						{
							max_sensor_val = min_sensor_val + SENSOR_VALUE_FAN_MIN_RANGE;
							FSMHelper_ConvertValToText(screen, FSM_POS_SENSOR_MAX_VAL);
						}
						setUserParameterInt(USER_PARAM_FAN_SPEED_MIN, min_sensor_val);
						setUserParameterInt(USER_PARAM_FAN_SPEED_MAX, max_sensor_val);
						break;
					}
					case max_item_id:
					{
						if (max_sensor_val - SENSOR_VALUE_FAN_MIN_RANGE < min_sensor_val)
						{
							min_sensor_val = max_sensor_val - SENSOR_VALUE_FAN_MIN_RANGE;
							FSMHelper_ConvertValToText(screen, FSM_POS_SENSOR_MIN_VAL);
						}
						setUserParameterInt(USER_PARAM_FAN_SPEED_MIN, min_sensor_val);
						setUserParameterInt(USER_PARAM_FAN_SPEED_MAX, max_sensor_val);
						break;
					}
					case pulses_item_id:
					{
						setUserParameterInt(USER_PARAM_FAN_PULSES, pulses_val);
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

void UI_BuildFanSensorMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);
	
	selected_slot = 0;
	selected_slot_gone = false;

  for (uint8_t i = 0; i < FSM_ELEMENT_TEXT_COUNT; i++)
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
		UI_Callback main = FanSensorMenu_OnItemPressed_Main, selection = NULL, onselect = NULL;
		if (FSMHelper_CheckValPosValidity(FSMHelper_GetValPosFromItemId(label_ids[i])))
		{
			selection = FanSensorMenu_OnItemPressed_Selection;
			onselect = FanSensorMenu_OnItemPressed_OnSelect;
		}
		
		UI_Element_Interactable* inter = ui_bindInteractable(
			screen,
			vis,
			main, selection, onselect
		);
  }
	
  for (uint8_t i = 0; i < FSM_ELEMENT_VAL_COUNT; i++)
  {
		*val_ptrs[i] = 0;
	}
	min_sensor_val = getUserParameterInt(USER_PARAM_FAN_SPEED_MIN);
	max_sensor_val = getUserParameterInt(USER_PARAM_FAN_SPEED_MAX);
	pulses_val = getUserParameterInt(USER_PARAM_FAN_PULSES);
	
  for (uint8_t i = 0; i < FSM_ELEMENT_VAL_COUNT; i++)
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
		
		FSMHelper_ConvertValToText(screen, i);
  }

	screen->should_draw_cursor = true;
	screen->cursor_left_or_right = 1;
	
  // Default hover index
	ui_hoverNext(screen, 1);
	
	screen->general_callback = FanSensorMenu_ScreenCallback;
	screen->callback_interval = 50;
	
	setCurrentLogicState(LSTATE_NONE);
}
