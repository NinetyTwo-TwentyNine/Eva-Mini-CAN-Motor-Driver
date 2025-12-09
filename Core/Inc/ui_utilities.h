#ifndef _UI_UTILS
#define _UI_UTILS

#include "ui_screen_system.h"
#include <stdio.h>

void utils_sprintf_slot_helper(char* destination_string, char* sprintf_params, uint32_t value, uint8_t selected_slot, uint8_t selected_slot_gone);
void utils_val_to_text_converter(char* destination_string, uint8_t length, uint8_t length_before_dot, uint32_t value, char* type, uint8_t selected_slot, uint8_t selected_slot_gone);

void utils_edit_value_by_slot(uint32_t* val_ptr, uint8_t selected_slot, UI_Element_Press_Type press_type, uint8_t max_slot_val);
void utils_edit_value_by_slot_with_min_max(uint32_t* val_ptr, uint8_t selected_slot, uint8_t allowed_length, UI_Element_Press_Type press_type, uint32_t min_val, uint32_t max_val);

#endif // _UI_UTILS
