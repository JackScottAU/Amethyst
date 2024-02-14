/*
	vgaConsole.h - Video Driver Declaration File.
	Last updated: 2011-10-20.
*/

#include <Types.h>

#ifndef __VGACONSOLE_H
#define __VGACONSOLE_H

	void vgaConsole_clearScreen(void);
	void vgaConsole_printf(const char* formatString, ...);
	void vgaConsole_putChar(char c);
	void vgaConsole_putHexadecimal(uint32 arg, uint8 leadingZeroes);
	void vgaConsole_putDecimal(uint32 arg);
	void vgaConsole_putString(const char *Text);
	void vgaConsole_scroll(void);
	void vgaConsole_setColour(uint8 foreColour, uint8 backColour);
	void vgaConsole_updateCursor(void);

	void vga_initialise(void);
	void vgaConsole_setCursor(uint8 x, uint8 y);

	//VGA Text Palette, in easy to use form :P.
	#define VGACONSOLE_BLACK		0x0
	#define VGACONSOLE_BLUE			0x1
	#define VGACONSOLE_GREEN		0x2
	#define VGACONSOLE_CYAN			0x3
	#define VGACONSOLE_RED			0x4
	#define VGACONSOLE_MAGENTA		0x5
	#define VGACONSOLE_BROWN		0x6
	#define VGACONSOLE_LIGHT_GREY		0x7
	#define VGACONSOLE_DARK_GREY		0x8
	#define VGACONSOLE_LIGHT_BLUE		0x9
	#define VGACONSOLE_LIGHT_GREEN		0xA
	#define VGACONSOLE_LIGHT_CYAN		0xB
	#define VGACONSOLE_LIGHT_RED		0xC
	#define VGACONSOLE_LIGHT_MAGENTA	0xD
	#define VGACONSOLE_LIGHT_BROWN		0xE
	#define VGACONSOLE_WHITE		0xF

#endif // __VGACONSOLE_H