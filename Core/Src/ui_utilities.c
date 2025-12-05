#include "ui_utilities.h"


void utils_sprintf_slot_helper(char* destination_string, char* sprintf_params, uint16_t value, uint8_t selected_slot, uint8_t selected_slot_gone)
{
	if (destination_string != NULL && sprintf_params != NULL)
	{
		sprintf(destination_string, sprintf_params, value);
		if (selected_slot_gone && selected_slot < strlen(destination_string))
		{
			destination_string[strlen(destination_string) - selected_slot - 1] = ' ';
		}
	}
}

void utils_edit_value_by_slot(uint16_t* val_ptr, uint8_t selected_slot, UI_Element_Press_Type press_type, uint8_t max_slot_val)
{
	if (max_slot_val == 0 || max_slot_val > 10)
	{
		max_slot_val = 10;
	}
	
	uint16_t scaler = 1;
	for (uint8_t i = 0; i < selected_slot; i++)
	{
		scaler *= 10;
	}
	uint8_t old_slot_val = (*val_ptr / scaler) % 10;
	uint8_t new_slot_val = (press_type == PRESS_TYPE_UP) ? ( (old_slot_val + 1) % max_slot_val ) : ( (old_slot_val == 0) ? (max_slot_val - 1) : (old_slot_val - 1) );
	
	*val_ptr = *val_ptr - old_slot_val * scaler + new_slot_val * scaler;
}

void utils_val_to_text_converter(char* destination_string, uint8_t length, uint16_t value, char* type, uint8_t selected_slot, uint8_t selected_slot_gone)
{
	if (destination_string == NULL) return;
	
	//char sprintf_params[] = "%00d";
	//sprintf_params[2] = ('0' + length);
	char sprintf_params[8];
	sprintf(sprintf_params, "%%0%dd", length);
	
	if (length <= 3)
	{
		utils_sprintf_slot_helper(destination_string, sprintf_params, value, selected_slot, selected_slot_gone);
	}
	else
	{
		char base_val_string[UI_ELEMENT_MAX_CHAR_COUNT] = {0};
		utils_sprintf_slot_helper(base_val_string, sprintf_params, value, selected_slot, selected_slot_gone);
		sprintf(destination_string, "%c.%s", base_val_string[0], base_val_string + 1);
	}
	strcat(destination_string, type);
}
