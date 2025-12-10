#include "ui_utilities.h"


void utils_sprintf_slot_helper(char* destination_string, char* sprintf_params, uint32_t value, uint8_t selected_slot, uint8_t selected_slot_gone)
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

void utils_val_to_text_converter(char* destination_string, uint8_t length, uint8_t length_before_dot, uint32_t value, char* type, uint8_t selected_slot, uint8_t selected_slot_gone)
{
	if (destination_string == NULL) return;
	
	//char sprintf_params[] = "%00d";
	//sprintf_params[2] = ('0' + length);
	char sprintf_params[8];
	sprintf(sprintf_params, "%%0%dd", length);
	
	if (length <= length_before_dot)
	{
		utils_sprintf_slot_helper(destination_string, sprintf_params, value, selected_slot, selected_slot_gone);
	}
	else
	{
		char base_val_string[UI_ELEMENT_MAX_CHAR_COUNT] = {0};
		utils_sprintf_slot_helper(base_val_string, sprintf_params, value, selected_slot, selected_slot_gone);
		uint16_t str_len = strlen(base_val_string);

		uint8_t dest_i = 0;
    for (uint8_t i = 0; i < str_len; i++)
		{
      destination_string[dest_i++] = base_val_string[i];

      if (i + 1 == str_len - length_before_dot && length_before_dot != 0)
			{
				destination_string[dest_i++] = '.';
      }
    }
    destination_string[dest_i] = '\0';
	}
	strcat(destination_string, type);
}


void utils_edit_value_by_slot(uint32_t* val_ptr, uint8_t selected_slot, UI_Element_Press_Type press_type, uint8_t max_slot_val)
{
	if (max_slot_val == 0 || max_slot_val > 10)
	{
		max_slot_val = 10;
	}
	
	uint32_t scaler = 1;
	for (uint8_t i = 0; i < selected_slot; i++)
	{
		scaler *= 10;
	}
	uint8_t old_slot_val = (*val_ptr / scaler) % 10;
	uint8_t new_slot_val = (press_type == PRESS_TYPE_UP) ? ( (old_slot_val + 1) % max_slot_val ) : ( (old_slot_val == 0) ? (max_slot_val - 1) : (old_slot_val - 1) );
	
	*val_ptr = *val_ptr - old_slot_val * scaler + new_slot_val * scaler;
}

void utils_edit_value_by_slot_with_min_max(uint32_t* val_ptr, uint8_t selected_slot, uint8_t allowed_length, UI_Element_Press_Type press_type, uint32_t min_val, uint32_t max_val)
{
	uint32_t scaler = 1;
	uint8_t all_senior_are_max = 1, all_senior_are_min = 1;
	for (uint8_t i = 0; i < allowed_length; i++)
	{
		if (i > selected_slot)
		{
			uint8_t slot_value = (*val_ptr / scaler) % 10, max_slot_val = (max_val / scaler) % 10, min_slot_val = (min_val / scaler) % 10;
			if (slot_value != max_slot_val)
			{
				all_senior_are_max = 0;
			}
			if (slot_value != min_slot_val)
			{
				all_senior_are_min = 0;
			}
		}
		scaler *= 10;
	}
	
	scaler = 1;
	for (uint8_t i = 0; i < selected_slot; i++)
	{
		scaler *= 10;
	}
	uint8_t curr_slot_val = (*val_ptr / scaler) % 10, max_slot_val = (max_val / scaler) % 10, min_slot_val = (min_val / scaler) % 10;
	if (!all_senior_are_max)
	{
		max_slot_val = 9;
	}
	if (!all_senior_are_min)
	{
		min_slot_val = 0;
	}
	
	
	uint8_t possible_slot_vals[10] = {0};
	uint8_t possible_vals_count = 0, curr_val_pos = 0;
	for (uint8_t i = min_slot_val; i <= max_slot_val; i++)
	{
		if (curr_slot_val == i)
		{
			curr_val_pos = possible_vals_count;
		}
		possible_slot_vals[possible_vals_count++] = i;
	}
	
	if (possible_vals_count <= 1)
	{
		return;
	}
	
	uint8_t old_slot_val = curr_slot_val;
	curr_val_pos = (press_type == PRESS_TYPE_UP) ? ( (curr_val_pos + 1) % possible_vals_count ) : ( (curr_val_pos == 0) ? (possible_vals_count - 1) : (curr_val_pos - 1) );
	curr_slot_val = possible_slot_vals[curr_val_pos];
	
	*val_ptr = *val_ptr - old_slot_val * scaler + curr_slot_val * scaler;
	if (*val_ptr > max_val)
	{
		*val_ptr = max_val;
	}
	else if (*val_ptr < min_val)
	{
		*val_ptr = min_val;
	}
}
