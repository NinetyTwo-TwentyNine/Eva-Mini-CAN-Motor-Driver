#ifndef _UI_SYS
#define _UI_SYS

#include "main.h"
#include "string.h"

typedef enum
{
	VISUAL_TYPE_TEXT,
	VISUAL_TYPE_CIRCLE,
	VISUAL_TYPE_TRIANGLE,
	VISUAL_TYPE_BITMAP,
} UI_Element_Visual_Type;

typedef struct UI_Element_Visual UI_Element_Visual;

typedef struct UI_Element_Interactable UI_Element_Interactable;

typedef struct UI_Screen UI_Screen;

typedef void (*UI_Callback)(UI_Screen* screen, UI_Element_Interactable* element);

#endif // _UI_SYS
