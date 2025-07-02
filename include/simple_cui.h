#pragma once

#include <stdint.h>

/**
 * @brief simple cui color
 * @details if you want select color you must use this enum value
 */
#define SIMPLE_CUI_NUM_COLORS 5
typedef enum {
	SIMPLE_CUI_BLACK,
	SIMPLE_CUI_WHITE,
	SIMPLE_CUI_RED,
	SIMPLE_CUI_GREEN,
	SIMPLE_CUI_BLUE
} SIMPLE_CUI_COLOR;

extern char ascii_bitmap[256][8];

void init_simple_cui(char* info_table);

void fill_screen(SIMPLE_CUI_COLOR color);

void print_text(char* text,SIMPLE_CUI_COLOR textColor,SIMPLE_CUI_COLOR backColor);