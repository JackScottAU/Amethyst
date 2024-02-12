
#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>
#include "vesa_framebuffer.h"



void vga_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour) {
	uint32* screen = canvas->framebuffer;
	screen[(y * canvas->width) + x] = colour;
}

void vga_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour) {
	for(uint16 i = 0; i < h; i++) {
		for(uint16 j = 0; j < w; j++) {
			vga_putPixel(canvas, x + j, y + i, colour);
		}
	}
}

void vga_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a) {
	// go to top level.
	uint8* charData = font->characterData;
	charData += a * font->header->height;

	for(int r = 0; r < font->header->height; r++) {
		// draw each row.

		for(int c = 0; c < font->header->width; c++) {
			// draw each column/pixel.
			char yes = (charData[r] << c) & 0x80;

			if(yes) {
				vga_putPixel(canvas, x + c, y + r, colour);
			}
		}
	}
}

void vga_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char* a) {
	int i = 0;

	while(a[i]) {
		vga_drawChar(canvas, font, x + (i*font->header->width), y, colour, a[i]);
		i++;
	}
}