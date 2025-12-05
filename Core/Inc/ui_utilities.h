#ifndef _UI_UTILS
#define _UI_UTILS

#include "ui_screen_system.h"
#include <stdio.h>

void utils_sprintf_slot_helper(char* destination_string, char* sprintf_params, uint16_t value, uint8_t selected_slot, uint8_t selected_slot_gone);
void utils_edit_value_by_slot(uint16_t* val_ptr, uint8_t selected_slot, UI_Element_Press_Type press_type, uint8_t max_slot_val);
void utils_val_to_text_converter(char* destination_string, uint8_t length, uint16_t value, char* type, uint8_t selected_slot, uint8_t selected_slot_gone);

#endif // _UI_UTILS
