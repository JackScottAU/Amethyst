/*
	vgaConsole.h - Video Driver Declaration File.
	Last updated: 2011-10-20.
*/


#ifndef __VGACONSOLE_H
#define __VGACONSOLE_H

#include <Types.h>

void vgaConsole_clearScreen(void);
void vgaConsole_putChar(unsigned char c);
void vgaConsole_putString(const char *Text);
void vgaConsole_setColour(uint8 foreColour, uint8 backColour);
void vgaConsole_updateCursor(void);

	//VGA Text Palette, in easy to use form :P.
	#define VID_BLACK		0x0
	#define VID_BLUE		0x1
	#define VID_GREEN		0x2
	#define VID_CYAN		0x3
	#define VID_RED			0x4
	#define VID_MAGENTA		0x5
	#define VID_BROWN		0x6
	#define VID_LIGHT_GREY		0x7
	#define VID_DARK_GREY		0x8
	#define VID_LIGHT_BLUE		0x9
	#define VID_LIGHT_GREEN		0xA
	#define VID_LIGHT_CYAN		0xB
	#define VID_LIGHT_RED		0xC
	#define VID_LIGHT_MAGENTA	0xD
	#define VID_LIGHT_BROWN		0xE
	#define VID_WHITE		0xF

#endif // __VGACONSOLE_H