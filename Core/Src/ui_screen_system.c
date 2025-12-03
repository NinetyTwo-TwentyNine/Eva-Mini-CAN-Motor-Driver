#include "ui_screen_system.h"

char* utf8rus(const char* source) // utf8 conversion for cyrillic letters
{
  uint16_t src_i,trg_i,k;
	
	if (!source) return NULL;
	char* target = NULL;
	for (uint16_t i = 0; (i < 10000 && target == NULL); i++)
	{	
		target = malloc(strlen(source) + 1); // +1 for '\0'
	}
	if (!target)
	{
		return NULL;
	}
	
  unsigned n;

  k = strlen(source); src_i = 0; trg_i = 0;

  while (src_i < k) {
    n = source[src_i]; src_i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[src_i]; src_i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[src_i]; src_i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
		target[trg_i++] = n;
  }
	target[trg_i] = '\0';
	
	return target;
}


void ui_clearElements(UI_Screen* screen)
{
  // Free allocated text in all used visuals
  for (uint8_t i = 0; i < screen->visuals_count; i++) {
      UI_Element_Visual *e = &screen->visuals[i];
      if (e->type == VISUAL_TYPE_TEXT && e->data.text.text != NULL) {
          free(e->data.text.text);
          e->data.text.text = NULL;
      }
			memset(e, 0, sizeof(*e));
  }

  memset(screen->visuals, 0, sizeof(screen->visuals));
  memset(screen->interactables, 0, sizeof(screen->interactables));
	
	screen->offset_y = 0;
	screen->visuals_count = 0;
	screen->hovered = NULL;
	screen->interactables_count = 0;
	screen->item_is_selected = 0;
}

UI_Element_Visual* ui_addVisualElement(UI_Screen *screen, UI_Element_Visual_Type type, uint8_t pos_x, uint8_t pos_y, uint8_t color, int8_t tab_index, uint8_t cursor_offset)
{
    if (screen->visuals_count >= UI_MAX_ELEMENT_COUNT)
        return NULL;

    UI_Element_Visual *e = &screen->visuals[screen->visuals_count++];
		
		memset(e, 0, sizeof(UI_Element_Visual));

		e->id = -1;
    e->type = type;
    e->pos_x = pos_x;
    e->pos_y = pos_y;
    e->color = color;
    e->tab_index = tab_index;
		e->cursor_offset = cursor_offset;
    e->context = screen;

    return e;
}

UI_Element_Visual* ui_addBitmap(UI_Screen* screen, uint8_t x, uint8_t y, uint8_t color, uint8_t w, uint8_t h, uint8_t* bitmap)
{
    UI_Element_Visual* e =
        ui_addVisualElement(screen, VISUAL_TYPE_BITMAP, x, y, color, -1, 0);

    if (!e)
        return NULL;

    e->data.bitmap.w = w;
    e->data.bitmap.h = h;
		e->data.bitmap.data = bitmap;

    return e;
}

UI_Element_Visual* ui_addText(UI_Screen* screen, uint8_t x, uint8_t y, uint8_t color, int8_t tab_index, uint8_t cursor_offset, char* text, uint8_t font)
{
    UI_Element_Visual *e =
        ui_addVisualElement(screen, VISUAL_TYPE_TEXT, x, y, color, tab_index, cursor_offset);

    if (!e)
        return NULL;

    e->data.text.text = utf8rus(text);
    e->data.text.font = font;

    return e;
}

UI_Element_Interactable* ui_bindInteractable(UI_Screen *screen, UI_Element_Visual *v, UI_Callback callback)
{
    if (screen->interactables_count >= UI_MAX_ELEMENT_COUNT)
        return NULL;
		
		UI_Element_Interactable* i = &(screen->interactables[screen->interactables_count++]);
		
		memset(i, 0, sizeof(UI_Element_Interactable));
		
		i->id = -1;
    i->visual = v;
		i->callback = callback;
		i->context = screen;
		
		return i;
}


void ui_hoverNext(UI_Screen* screen, uint8_t direction)
{
	int16_t tabindex_prev;
	if (screen->hovered == NULL)
	{
		tabindex_prev = direction ? -1 : 0xFF;
	}
	else
	{
		tabindex_prev = screen->hovered->tab_index;
	}
	
	uint8_t curr_diff = 0xFF;
	UI_Element_Visual *best = NULL;

	if (direction)  // forward
	{
		for (uint8_t i = 0; i < screen->visuals_count; i++)
		{
			int8_t ti = screen->visuals[i].tab_index;
			if (ti == -1)
				continue;

			if (ti > tabindex_prev)
			{
				uint8_t diff = (uint8_t)(ti - tabindex_prev);
				if (diff < curr_diff)
				{
					curr_diff = diff;
					best = &screen->visuals[i];
				}
			}
		}
	}
	else  // backward
	{
		for (uint8_t i = 0; i < screen->visuals_count; i++)
		{
			int8_t ti = screen->visuals[i].tab_index;
			if (ti == -1)
				continue;

			if (ti < tabindex_prev)
			{
				uint8_t diff = (uint8_t)(tabindex_prev - ti);
				if (diff < curr_diff)
				{
					curr_diff = diff;
					best = &screen->visuals[i];
				}
			}
		}
	}

	// If no candidate found (wrap)
	if (best == NULL)
	{
		// initialize best to first valid element
		for (uint8_t i = 0; i < screen->visuals_count; i++)
		{
			if (screen->visuals[i].tab_index != -1)
			{
				best = &screen->visuals[i];
				break;
			}
		}

		if (best != NULL)
		{
			// now find min or max tab_index depending on direction
			for (uint8_t i = 0; i < screen->visuals_count; i++)
			{
				int8_t ti = screen->visuals[i].tab_index;
				if (ti == -1)
					continue;

				if (direction)
				{
					// backward? lowest tab_index
					if (ti < best->tab_index)
						best = &screen->visuals[i];
				}
				else
				{
					// forward? highest tab_index
					if (ti > best->tab_index)
						best = &screen->visuals[i];
				}
			}
		}
	}

	screen->hovered = best;
}

void ui_selectItem(UI_Screen* screen, uint8_t toggle, uint8_t is_selected)
{
	if (toggle)
	{
			screen->item_is_selected = !screen->item_is_selected;
	}
	else
	{
			screen->item_is_selected = is_selected;
	}
}


void UI_PerformUserInteraction(UI_Screen* screen, UI_Element_Press_Type interaction_type)
{
	switch (interaction_type)
	{
		case PRESS_TYPE_UP: case PRESS_TYPE_DOWN:
			if (screen->hovered != NULL && screen->item_is_selected)
			{
				for (uint8_t i = 0; i < screen->interactables_count; i++)
				{
					UI_Element_Interactable* inter = &(screen->interactables[i]);
					if (inter->visual == screen->hovered)
					{
						inter->callback(screen, interaction_type, inter);
					}
				}
			}
			else
			{
				ui_hoverNext(screen, (interaction_type == PRESS_TYPE_UP)? 1 : 0);
			}
			break;
		case PRESS_TYPE_OK: case PRESS_TYPE_OTHER:
			if (screen->hovered != NULL)
			{
				for (uint8_t i = 0; i < screen->interactables_count; i++)
				{
					UI_Element_Interactable* inter = &(screen->interactables[i]);
					if (inter->visual == screen->hovered)
					{
						inter->callback(screen, interaction_type, inter);
					}
				}
			}
			break;
		default:
			return;
	}
}
