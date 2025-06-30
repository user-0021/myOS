#include <stddef.h>
#include <multiboot2.h>
#include <multiboot2_utils.h>

struct multiboot_tag_framebuffer_common* frame;

void fill_screen(uint32_t color);

void init_frame_buffer(char* info_table){
	frame = (void*)find_tag((uint32_t)info_table,MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

	if(frame != NULL){

	}
}
