#include <ui_screen_main_menu.h>

static const uint8_t element_text_count = 4;
static char *norm_text_lbl = "Норма:", *fan_text_lbl = "В:", *motor_text_lbl = "М:", *area_text_lbl = "S:";
static uint8_t text_xpos[] = { 4, 4, 4, 80 };
static uint8_t text_ypos[] = { 20, 36, 52, 52 };

static const uint8_t element_type_count = 4;
static char *norm_type_lbl = "кг/га", *fan_type_lbl = "об/мин", *motor_type_lbl = "об/мин", *area_type_lbl = "га";
static uint8_t type_xpos[] = { 94, 44, 36, 112 };
static uint8_t type_ypos[] = { 20, 36, 52, 52 };

static const uint8_t element_val_count = 6;
static uint8_t norm_val_id = 1, fan_val_id = 2, motor_val_id = 3, area_val_id = 4, error_val_id = 5, seeder_val_id = 6;
static char *norm_val_lbl = "000(000)", *fan_val_lbl = "0000", *motor_val_lbl = "000", *area_val_lbl = "000", *error_val_lbl = "Бункер пуст", *seeder_val_lbl = "Т";
static uint8_t val_xpos[] = { 42, 16, 16, 92, 16, 115 };
static uint8_t val_ypos[] = { 20, 36, 52, 52, 4, 36 };

static const uint8_t element_icon_count = 2;
static uint8_t error_icon_id = 7, seeder_icon_id = 8;
static uint8_t icon_xpos[] = { 4, 105 };
static uint8_t icon_ypos[] = { 4, 36 };


void UI_BuildMainMenu(UI_Screen* screen)
{
	ui_clearScreen(screen);

	char* texts[element_text_count] = { norm_text_lbl, fan_text_lbl, motor_text_lbl, area_text_lbl };
	char* types[element_type_count] = { norm_type_lbl, fan_type_lbl, motor_type_lbl, area_type_lbl };
	
	char* val_defaults[element_val_count] = { norm_val_lbl, fan_val_lbl, motor_val_lbl, area_val_lbl, error_val_lbl, seeder_val_lbl };
	uint8_t val_ids[element_val_count] = { norm_val_id, fan_val_id, motor_val_id, area_val_id, error_val_id, seeder_val_id };
	
	uint8_t icon_ids[element_icon_count] = { error_icon_id, seeder_icon_id };

  for (uint8_t i = 0; i < element_text_count; i++)
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
    text_elem->id = 0;
	}

  for (uint8_t i = 0; i < element_type_count; i++)
  {
    // ---------------- Visual (Type) ----------------
    UI_Element_Visual* type_elem = ui_addText(
        screen,
        type_xpos[i],            // pos_x
        type_ypos[i],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  types[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
    type_elem->id = 0;
	}
	
	for (uint8_t i = 0; i < element_val_count; i++)
	{
    // ---------------- Visual (Value) ----------------
    UI_Element_Visual* val_elem = ui_addText(
        screen,
        val_xpos[i],            // pos_x
        val_ypos[i],            // pos_y
				WHITE,							// color
			  0,              // tab index
				CHAR_BASE_WIDTH*3,  // cursor offset
			  val_defaults[i],          		// text
        UI_MAIN_TEXT_SIZE   // font size
    );

    // Optional: assign visual ID
    val_elem->id = val_ids[i];
  }
	
	
	for (uint8_t i = 0; i < element_icon_count; i++)
	{
		uint8_t* bitmap_data = (i % 2 == 0)? logo_error_alert : logo_seeder_state;
		uint8_t bitmap_width = (i % 2 == 0)? LOGO_ERROR_ALERT_WIDTH : LOGO_SEEDER_STATE_WIDTH;
		uint8_t bitmap_height = (i % 2 == 0)? LOGO_ERROR_ALERT_HEIGHT : LOGO_SEEDER_STATE_HEIGHT;
		
		UI_Element_Visual* icon_elem = ui_addBitmap(
      screen,
      icon_xpos[i],                 // pos_x
      icon_ypos[i],            			// pos_y
			WHITE,							// color
      bitmap_width, 	// width
			bitmap_height,	// height
			bitmap_data		// bitmap
		);
		icon_elem->id = icon_ids[i];
	}
	
	screen->should_draw_cursor = false;
  // Default hover index: None
	
	
	switch_to_start_menu_allowed = true;
}
