#pragma once

#include <stdint.h>

#define BIT_SIZE 3
#define SIMPLE_CUI_NUM_COLORS 5
/**
 * @brief simple cui color
 * @details if you want select color you must use this enum value
 */
typedef enum {
	SIMPLE_CUI_BLACK,
	SIMPLE_CUI_WHITE,
	SIMPLE_CUI_RED,
	SIMPLE_CUI_GREEN,
	SIMPLE_CUI_BLUE
} SIMPLE_CUI_COLOR;

extern char ascii_bitmap[256][8];

/**
 * @brief init simple_cui 
 * 
 * @param info_table[in] multiboot2 info tag table
 */
void init_simple_cui(char* info_table);

/**
 * @brief fill screen
 * 
 * @param [in] color fill color.if in ega text mode, this param ignore.
 */
void fill_screen(SIMPLE_CUI_COLOR color);

/**
 * @brief print text (like printf)
 * 
 * @param [in] text      text
 * @param [in] textColor text color
 * @param [in] backColor back color
 * @param [in] ...	argment
 */
void print_text(char* text,SIMPLE_CUI_COLOR textColor,SIMPLE_CUI_COLOR backColor,...);