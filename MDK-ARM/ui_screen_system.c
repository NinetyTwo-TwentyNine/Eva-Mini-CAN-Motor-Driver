#include "ui_screen_system.h"

struct UI_Element_Visual {
	char id[4];
	UI_Element_Visual_Type type;
	uint8_t pos_x, pos_y;
	uint8_t tab_index;
	uint16_t color;
	
	//UI_Element_Interactable* interactable;
	
	union {
		struct { char *text; uint8_t font; } text;
		struct { uint8_t x_n[32]; uint8_t y_n[32]; uint8_t line_count; } lines;
		struct { uint8_t x1; uint8_t y1; uint8_t x2; uint8_t y2; uint8_t is_hollow; } triangle;
		struct { uint8_t w; uint8_t h; uint8_t is_hollow; } rectangle;
		struct { uint8_t radius; uint8_t is_hollow; } circle;
		struct { uint8_t *data; uint16_t w, h; } bitmap;
	} data;
	
	UI_Screen* context;
};

struct UI_Element_Interactable {
	char id[4];
	
	UI_Element_Visual* visual;
	UI_Callback callback;
	
	UI_Screen* context;
};

struct UI_Screen {
	UI_Element_Interactable ineractables[32];
	uint8_t interactables_count;
	
	UI_Element_Visual visuals[32];
	uint8_t visuals_count;
	
	UI_Element_Visual* hovered;
	uint8_t item_is_selected;
};
