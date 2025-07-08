#include <stddef.h>
#include <boot/simple_cui.h>
#include <boot/multiboot2.h>
#include <boot/multiboot2_utils.h>

struct multiboot_tag_framebuffer* frame = NULL;

uint32_t simple_cui_color_list[SIMPLE_CUI_NUM_COLORS] = {0};
uint32_t cursor_pos[2];

uint32_t _color_comp(uint8_t l_red,uint8_t l_green,uint8_t l_blue,uint8_t r_red,uint8_t r_green,uint8_t r_blue);
void _init_for_indexd();
void _init_for_rgb();
void _init_for_ega_text();
void _put_char_indexd(char c,SIMPLE_CUI_COLOR text_color,SIMPLE_CUI_COLOR back_color);
void _put_char_rgb(char c,SIMPLE_CUI_COLOR text_color,SIMPLE_CUI_COLOR back_color);
void _put_char_ega_text(char c,uint16_t color);

/**
 * @brief init simple_cui 
 * 
 * @param info_table multiboot2 info tag table
 */
void init_simple_cui(char* info_table){
	//find frame buffer tag
	frame = (void*)find_tag((uint32_t)info_table,MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

	if(frame != NULL){

		switch (frame->common.framebuffer_type)
		{
		case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
			_init_for_indexd();
			break;
		case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
			_init_for_rgb();
			break;
		case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
			_init_for_ega_text();
			break;
		default:
			break;
		}

		fill_screen(SIMPLE_CUI_BLACK);
	}
}

/**
 * @brief fill screen
 * 
 * @param color fill color.if in ega text mode, this param ignore.
 */
void fill_screen(SIMPLE_CUI_COLOR color){

	//check
	if(frame == NULL)
		return;

	//init cursor pos
	cursor_pos[0] = 0;
	cursor_pos[1] = 0;
	
	//fill
	uint32_t i,j,k;
	uint8_t bit_per_byte = frame->common.framebuffer_bpp >> 3;
	char * buffer = (void*)((uint32_t)frame->common.framebuffer_addr);
	for(i = 0;i < frame->common.framebuffer_height;i++){
		char* line = buffer + i * frame->common.framebuffer_pitch;
		for(j = 0;j < frame->common.framebuffer_width;j++){
			for(k = 0;k < bit_per_byte;k++){
				line[k] = ((char*)(&simple_cui_color_list[color]))[k];
			}
			line += bit_per_byte;
		}
	}
}


/**
 * @brief print text
 * 
 * @param text      text
 * @param textColor text color
 * @param backColor 
 */
void print_text(char* text,SIMPLE_CUI_COLOR textColor,SIMPLE_CUI_COLOR backColor){
	
	if(frame != NULL){

		switch (frame->common.framebuffer_type)
		{
		case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
			while (*text != '\0')
			{
				_put_char_indexd(*text,textColor,backColor);
				text++;
			}
			break;
		case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
			while (*text != '\0')
			{
				_put_char_rgb(*text,textColor,backColor);
				text++;
			}
			break;
		case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
			while (*text != '\0')
			{
				_put_char_ega_text(*text,(simple_cui_color_list[textColor] & 0x0F00) | (simple_cui_color_list[backColor] & 0xF000));
				text++;
			}
			break;
		default:
			break;
		}
	}
}

/**
 * @brief compare color
 * 
 * @param l_red   left color red
 * @param l_green left color green
 * @param l_blue  left color blue
 * @param r_red   right color red
 * @param r_green right color green
 * @param r_blue  right color blue
 * @return uint32_t two color distance in 3 axsis space
 */
uint32_t _color_comp(uint8_t l_red,uint8_t l_green,uint8_t l_blue,uint8_t r_red,uint8_t r_green,uint8_t r_blue){
	//calc diff
	uint16_t red_diff   = l_red   - r_red;
	uint16_t green_diff = l_green - r_green;
	uint16_t blue_diff  = l_blue  - r_blue;

	//calc diff
	red_diff   *= red_diff;
	green_diff *= green_diff;
	blue_diff  *= blue_diff;

	return red_diff + green_diff + blue_diff;
}

/**
 * @brief init for indexd mode
 * 
 */
void _init_for_indexd(){
	uint32_t i , j;
	for(i = 0;i < frame->framebuffer_palette_num_colors;i++){
		for(j = 0;j < SIMPLE_CUI_NUM_COLORS;j++){
			switch (j)
			{
			case SIMPLE_CUI_BLACK:
				if(_color_comp(0x00,0x00,0x00,frame->framebuffer_palette[i].red,frame->framebuffer_palette[i].green,frame->framebuffer_palette[i].blue)
				< _color_comp(0x00,0x00,0x00,frame->framebuffer_palette[simple_cui_color_list[j]].red,frame->framebuffer_palette[simple_cui_color_list[j]].green,frame->framebuffer_palette[simple_cui_color_list[j]].blue))
					simple_cui_color_list[j] = i;
				break;
			case SIMPLE_CUI_WHITE:
				if(_color_comp(0xFF,0xFF,0xFF,frame->framebuffer_palette[i].red,frame->framebuffer_palette[i].green,frame->framebuffer_palette[i].blue)
				< _color_comp(0xFF,0xFF,0xFF,frame->framebuffer_palette[simple_cui_color_list[j]].red,frame->framebuffer_palette[simple_cui_color_list[j]].green,frame->framebuffer_palette[simple_cui_color_list[j]].blue))
					simple_cui_color_list[j] = i;
				break;
			case SIMPLE_CUI_RED:
				if(_color_comp(0xFF,0x00,0x00,frame->framebuffer_palette[i].red,frame->framebuffer_palette[i].green,frame->framebuffer_palette[i].blue)
				< _color_comp(0xFF,0x00,0x00,frame->framebuffer_palette[simple_cui_color_list[j]].red,frame->framebuffer_palette[simple_cui_color_list[j]].green,frame->framebuffer_palette[simple_cui_color_list[j]].blue))
					simple_cui_color_list[j] = i;
				break;
			case SIMPLE_CUI_GREEN:
				if(_color_comp(0x00,0xFF,0x00,frame->framebuffer_palette[i].red,frame->framebuffer_palette[i].green,frame->framebuffer_palette[i].blue)
				< _color_comp(0x00,0xFF,0x00,frame->framebuffer_palette[simple_cui_color_list[j]].red,frame->framebuffer_palette[simple_cui_color_list[j]].green,frame->framebuffer_palette[simple_cui_color_list[j]].blue))
					simple_cui_color_list[j] = i;
				break;
			case SIMPLE_CUI_BLUE:
				if(_color_comp(0x00,0x00,0xFF,frame->framebuffer_palette[i].red,frame->framebuffer_palette[i].green,frame->framebuffer_palette[i].blue)
				< _color_comp(0x00,0x00,0xFF,frame->framebuffer_palette[simple_cui_color_list[j]].red,frame->framebuffer_palette[simple_cui_color_list[j]].green,frame->framebuffer_palette[simple_cui_color_list[j]].blue))
					simple_cui_color_list[j] = i;
				break;
		
			default:
				break;
			}
		}
	}
}

/**
 * @brief init for rgb mode
 * 
 */
void _init_for_rgb(){
	uint32_t red_mask = 0;
	uint32_t green_mask = 0;
	uint32_t blue_mask = 0;
	uint32_t alpha_mask = 0;
	switch (frame->common.framebuffer_bpp)
	{
	case 16:
		red_mask   = 0xF800;
		green_mask = 0x07E0;
		blue_mask  = 0x001F;
		break;
	case 24:
		red_mask   = 0xFF0000;
		green_mask = 0x00FF00;
		blue_mask  = 0x0000FF;
		break;
	case 32:
		red_mask   = 0x00FF0000;
		green_mask = 0x0000FF00;
		blue_mask  = 0x000000FF;
		alpha_mask = 0xFF000000;
		break;
	default:// unknown type
		red_mask   = 0x00F;
		green_mask = 0x0F0;
		blue_mask  = 0xF00;
		break;
	}

	uint32_t i;
	for(i = 0;i < SIMPLE_CUI_NUM_COLORS;i++){
		switch (i)
		{
		case SIMPLE_CUI_BLACK:
			simple_cui_color_list[i] = alpha_mask;
			break;
		case SIMPLE_CUI_WHITE:
			simple_cui_color_list[i] = red_mask | green_mask | blue_mask | alpha_mask;
			break;
		case SIMPLE_CUI_RED:
			simple_cui_color_list[i] = red_mask | alpha_mask;
			break;
		case SIMPLE_CUI_GREEN:
			simple_cui_color_list[i] = green_mask | alpha_mask;
			break;
		case SIMPLE_CUI_BLUE:
			simple_cui_color_list[i] = blue_mask | alpha_mask;
			break;	
		default:
			break;
		}
	}
}

/**
 * @brief init for efa text mode
 * 
 */
void _init_for_ega_text(){

	uint32_t i;
	for(i = 0;i < SIMPLE_CUI_NUM_COLORS;i++){
		switch (i)
		{
		case SIMPLE_CUI_BLACK:
			simple_cui_color_list[i] = (' ') | (((0  << 4) |  0) << 8);
			break;
		case SIMPLE_CUI_WHITE:
			simple_cui_color_list[i] = (' ') | (((15 << 4) | 15) << 8);
			break;
		case SIMPLE_CUI_RED:
			simple_cui_color_list[i] = (' ') | (((4  << 4) |  4) << 8);
			break;
		case SIMPLE_CUI_GREEN:
			simple_cui_color_list[i] = (' ') | (((2  << 4) |  2) << 8);
			break;
		case SIMPLE_CUI_BLUE:
			simple_cui_color_list[i] = (' ') | (((1  << 4) |  1) << 8);
			break;	
		default:
			break;
		}
	}
}


void _put_char_indexd(char c,SIMPLE_CUI_COLOR text_color,SIMPLE_CUI_COLOR back_color){
	uint32_t width = (frame->common.framebuffer_width / BIT_SIZE) >> 3;
	uint32_t height = (frame->common.framebuffer_height / BIT_SIZE) >> 3;

	//check
	if(width == 0 || height == 0)
		return;

	//put char
	if(c == '\n' || c == '\r'){
		if(c == '\r')
			cursor_pos[0] = 0;
		else
			cursor_pos[1]++;
	}else{
		//copy ascii from bit map
		uint32_t i,j,k;
		uint32_t bytes = frame->common.framebuffer_bpp >> 3;
		char * print_point = ((char*)(uint32_t)frame->common.framebuffer_addr) + (cursor_pos[0] << 3) * bytes * BIT_SIZE +   (cursor_pos[1] << 3) * frame->common.framebuffer_pitch * BIT_SIZE;
		
		for(i = 0;i < 8 * BIT_SIZE;i++){
			char line_d = ascii_bitmap[(unsigned char)c][i / BIT_SIZE];
			
			for(j = 0;j < 8 * BIT_SIZE;j++){
				SIMPLE_CUI_COLOR color;
				char* line_s = print_point + bytes * j;
				
				//check bit
				if((line_d >> (7 - j / BIT_SIZE)) & 1)
					color = text_color;
				else
					color = back_color;

				for(k = 0;k < bytes;k++){
					line_s[k] = ((char*)&simple_cui_color_list[color])[k];
				}
			}

			print_point += frame->common.framebuffer_pitch;
		}
		
		cursor_pos[0]++;
	}


	//check x pos
	if(cursor_pos[0] >= width){
		cursor_pos[0] = 0;
		cursor_pos[1]++;
	}


	//check y pos
	if(cursor_pos[1] >= height){
		cursor_pos[1]--;

		//copy buffer
		uint32_t i,j;
		for(i = 0;i < (height - 1);i ++){
			char* line_d = ((char*)(uint32_t)frame->common.framebuffer_addr) + i       * (frame->common.framebuffer_pitch << 3);
			char* line_s = ((char*)(uint32_t)frame->common.framebuffer_addr) + (i + 1) * (frame->common.framebuffer_pitch << 3);

			//copy line
			for(j = 0;j < (frame->common.framebuffer_pitch << 3);j ++){
				line_d[j] = line_s[j];
			}
		}

		//clear last line
		char* line = ((char*)(uint32_t)frame->common.framebuffer_addr) + (height - 1) * (frame->common.framebuffer_pitch << 3);
		for(i = 0;i < (frame->common.framebuffer_pitch << 3);i ++){
			line[i] = (uint16_t)simple_cui_color_list[SIMPLE_CUI_BLACK];
		}
	}
}

void _put_char_rgb(char c,SIMPLE_CUI_COLOR text_color,SIMPLE_CUI_COLOR back_color){
	uint32_t width = (frame->common.framebuffer_width / BIT_SIZE) >> 3;
	uint32_t height = (frame->common.framebuffer_height / BIT_SIZE) >> 3;

	//check
	if(width == 0 || height == 0)
		return;

	//put char
	if(c == '\n' || c == '\r'){
		if(c == '\r')
			cursor_pos[0] = 0;
		else
			cursor_pos[1]++;
	}else{
		//copy ascii from bit map
		uint32_t i,j,k;
		uint32_t bytes = frame->common.framebuffer_bpp >> 3;
		char * print_point = ((char*)(uint32_t)frame->common.framebuffer_addr) + (cursor_pos[0] << 3) * bytes * BIT_SIZE +   (cursor_pos[1] << 3) * frame->common.framebuffer_pitch * BIT_SIZE;
		
		for(i = 0;i < 8 * BIT_SIZE;i++){
			char line_d = ascii_bitmap[(unsigned char)c][i / BIT_SIZE];
			
			for(j = 0;j < 8 * BIT_SIZE;j++){
				SIMPLE_CUI_COLOR color;
				char* line_s = print_point + bytes * j;
				
				//check bit
				if((line_d >> (7 - j / BIT_SIZE)) & 1)
					color = text_color;
				else
					color = back_color;

				for(k = 0;k < bytes;k++){
					line_s[k] = ((char*)&simple_cui_color_list[color])[k];
				}
			}

			print_point += frame->common.framebuffer_pitch;
		}
		
		cursor_pos[0]++;
	}


	//check x pos
	if(cursor_pos[0] >= width){
		cursor_pos[0] = 0;
		cursor_pos[1]++;
	}


	//check y pos
	if(cursor_pos[1] >= height){
		cursor_pos[1]--;

		//copy buffer
		uint32_t i,j;
		for(i = 0;i < (height - 1);i ++){
			char* line_d = ((char*)(uint32_t)frame->common.framebuffer_addr) + i       * (frame->common.framebuffer_pitch << 3);
			char* line_s = ((char*)(uint32_t)frame->common.framebuffer_addr) + (i + 1) * (frame->common.framebuffer_pitch << 3);

			//copy line
			for(j = 0;j < (frame->common.framebuffer_pitch << 3);j ++){
				line_d[j] = line_s[j];
			}
		}

		//clear last line
		char* line = ((char*)(uint32_t)frame->common.framebuffer_addr) + (height - 1) * (frame->common.framebuffer_pitch << 3);
		for(i = 0;i < (frame->common.framebuffer_pitch << 3);i ++){
			line[i] = (uint16_t)simple_cui_color_list[SIMPLE_CUI_BLACK];
		}
	}
}

/**
 * @brief put char in ega text mode
 * 
 * @param c 
 * @param color 
 */
void _put_char_ega_text(char c,uint16_t color){
	//put char
	if(c == '\n' || c == '\r'){
		if(c == '\r')
			cursor_pos[0] = 0;
		else
			cursor_pos[1]++;
	}else{
		uint16_t* dist = (uint16_t*)(((char*)(uint32_t)frame->common.framebuffer_addr) + (cursor_pos[0] << 1) + cursor_pos[1] * frame->common.framebuffer_pitch);
		*dist = c | color;
		cursor_pos[0]++;
	}

	//check x pos
	if(cursor_pos[0] >= frame->common.framebuffer_width){
		cursor_pos[0] = 0;
		cursor_pos[1]++;
	}


	//check y pos
	if(cursor_pos[1] >= frame->common.framebuffer_height){
		cursor_pos[1]--;

		//copy buffer
		uint32_t i,j;
		for(i = 0;i < (frame->common.framebuffer_height - 1);i ++){
			uint16_t* line_d = ((uint16_t*)(uint32_t)frame->common.framebuffer_addr) + i       * frame->common.framebuffer_width;
			uint16_t* line_s = ((uint16_t*)(uint32_t)frame->common.framebuffer_addr) + (i + 1) * frame->common.framebuffer_width;

			//copy line
			for(j = 0;j < frame->common.framebuffer_width;j ++){
				line_d[j] = line_s[j];
			}
		}

		//clear last line
		uint16_t* line = ((uint16_t*)(uint32_t)frame->common.framebuffer_addr) + (frame->common.framebuffer_height - 1) * frame->common.framebuffer_width;
		for(i = 0;i < frame->common.framebuffer_width;i ++){
			line[i] = (uint16_t)simple_cui_color_list[SIMPLE_CUI_BLACK];
		}
	}
}