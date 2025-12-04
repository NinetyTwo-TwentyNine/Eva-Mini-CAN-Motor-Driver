#ifndef _UI_SYS
#define _UI_SYS

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define UI_ELEMENT_MAX_CHAR_COUNT 32
#define UI_ELEMENT_MAX_LINE_COUNT 32
#define UI_MAX_ELEMENT_COUNT 32

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
	PRESS_TYPE_UP,
	PRESS_TYPE_DOWN,
	PRESS_TYPE_OK,
	PRESS_TYPE_OTHER
} UI_Element_Press_Type;

typedef struct Struct_UI_Element_Visual UI_Element_Visual;
typedef struct Struct_UI_Element_Interactable UI_Element_Interactable;
typedef struct Struct_UI_Screen UI_Screen;

typedef void (*UI_Callback)(UI_Screen* screen, UI_Element_Press_Type press_type, UI_Element_Interactable* element);
typedef void (*UI_Callback_General)(UI_Screen* screen);

struct Struct_UI_Element_Visual {
	uint8_t id;
	
	UI_Element_Visual_Type type;
	uint16_t pos_x, pos_y;
	uint8_t color;
	
	uint8_t tab_index;
	int8_t cursor_offset;
	
	int8_t offset_y_up, offset_y_down;
	
	
	//UI_Element_Interactable* interactable;
	
	union {
		struct { char text[UI_ELEMENT_MAX_CHAR_COUNT]; uint8_t font; } text;
		struct { uint8_t x_n[UI_ELEMENT_MAX_LINE_COUNT]; uint8_t y_n[UI_ELEMENT_MAX_LINE_COUNT]; uint8_t line_count; } lines;
		struct { uint8_t x1; uint8_t y1; uint8_t x2; uint8_t y2; uint8_t is_hollow; } triangle;
		struct { uint8_t w; uint8_t h; uint8_t is_hollow; } rectangle;
		struct { uint8_t radius; uint8_t is_hollow; } circle;
		struct { uint8_t *data; uint16_t w, h; } bitmap;
	} data;
	
	UI_Screen* context;
};

struct Struct_UI_Element_Interactable {
	uint8_t id;
	
	UI_Element_Visual* visual;
	UI_Callback callback;
	
	UI_Screen* context;
};

struct Struct_UI_Screen {
	UI_Element_Interactable interactables[UI_MAX_ELEMENT_COUNT];
	uint8_t interactables_count;
	
	UI_Element_Visual visuals[UI_MAX_ELEMENT_COUNT];
	uint8_t visuals_count;
	
	UI_Element_Visual* hovered;
	uint8_t item_is_selected;
	
	uint8_t should_draw_cursor;
	uint8_t cursor_left_or_right;
	
	uint16_t offset_y;
	
	UI_Callback_General general_callback;
	uint16_t callback_interval;
};


char* utf8rus(const char* source, char* target);

void ui_clearScreen(UI_Screen* screen);

UI_Element_Visual* ui_addVisualElement(UI_Screen* screen, UI_Element_Visual_Type type, uint8_t pos_x, uint8_t pos_y, uint8_t color, uint8_t tab_index, int8_t cursor_offset);
UI_Element_Interactable* ui_bindInteractable(UI_Screen* screen, UI_Element_Visual* v, UI_Callback callback);

UI_Element_Visual* ui_addText(UI_Screen* screen, uint8_t x, uint8_t y, uint8_t color, uint8_t tab_index, int8_t cursor_offset, char* text, uint8_t font);
void ui_editText(UI_Element_Visual* e, char* new_text, uint8_t new_font);

UI_Element_Visual* ui_addBitmap(UI_Screen* screen, uint8_t x, uint8_t y, uint8_t color, uint8_t w, uint8_t h, uint8_t* bitmap);
void ui_editBitmap(UI_Element_Visual* e, uint8_t new_w, uint8_t new_h, uint8_t* new_bitmap);

UI_Element_Visual* ui_findVisualById(UI_Screen* screen, uint8_t id);
UI_Element_Interactable* ui_findInteractableById(UI_Screen* screen, uint8_t id);

void ui_hoverNext(UI_Screen* screen, uint8_t direction);
void ui_selectItem(UI_Screen* screen, uint8_t toggle, uint8_t is_selected);

void UI_PerformUserInteraction(UI_Screen* screen, UI_Element_Press_Type interaction_type);

#endif // _UI_SYS
