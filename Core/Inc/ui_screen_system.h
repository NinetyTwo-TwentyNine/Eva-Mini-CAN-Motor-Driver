#ifndef _UI_SYS
#define _UI_SYS

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
	VISUAL_TYPE_TEXT,
	VISUAL_TYPE_LINES,
	VISUAL_TYPE_TRIANGLE,
	VISUAL_TYPE_RECTANGLE,
	VISUAL_TYPE_CIRCLE,
	VISUAL_TYPE_BITMAP,
} UI_Element_Visual_Type;

typedef enum
{
	PRESS_TYPE_OK,
	PRESS_TYPE_UP,
	PRESS_TYPE_DOWN,
	PRESS_TYPE_OTHER
} UI_Element_Press_Type;

typedef struct Struct_UI_Element_Visual UI_Element_Visual;
typedef struct Struct_UI_Element_Interactable UI_Element_Interactable;
typedef struct Struct_UI_Screen UI_Screen;

typedef void (*UI_Callback)(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);

struct Struct_UI_Element_Visual {
	char id[4];
	UI_Element_Visual_Type type;
	uint8_t pos_x, pos_y;
	uint8_t color;
	
	int8_t tab_index;
	uint8_t cursor_offset;
	
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

struct Struct_UI_Element_Interactable {
	char id[4];
	
	UI_Element_Visual* visual;
	UI_Callback callback;
	
	UI_Screen* context;
};

struct Struct_UI_Screen {
	UI_Element_Interactable interactables[32];
	uint8_t interactables_count;
	
	UI_Element_Visual visuals[32];
	uint8_t visuals_count;
	
	UI_Element_Visual* hovered;
	uint8_t item_is_selected;
	
	uint8_t should_draw_cursor;
	uint8_t cursor_left_or_right;
};


void ui_hoverNext(UI_Screen* screen, uint8_t direction);
void ui_selectItem(UI_Screen* screen, uint8_t toggle, uint8_t is_selected);
void ui_clearElements(UI_Screen* screen);

UI_Element_Visual* ui_addVisualElement(UI_Screen* screen, UI_Element_Visual_Type type, uint8_t pos_x, uint8_t pos_y, uint8_t color, int8_t tab_index, uint8_t cursor_offset);
UI_Element_Visual* ui_addText(UI_Screen* screen, uint8_t x, uint8_t y, uint8_t color, int8_t tab_index, uint8_t cursor_offset, char* text, uint8_t font);
UI_Element_Visual* ui_addBitmap(UI_Screen* screen, uint8_t x, uint8_t y, uint8_t color, int8_t tab_index, uint8_t cursor_offset, uint8_t w, uint8_t h, uint8_t* bitmap);
UI_Element_Interactable* ui_bindInteractable(UI_Screen* screen, UI_Element_Visual* v, UI_Callback callback);

void UI_PerformUserInteraction(UI_Screen* screen, UI_Element_Press_Type interaction_type);

#endif // _UI_SYS
