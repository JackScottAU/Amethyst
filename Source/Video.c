/**
 * Text console driver used by the kernel at boot-up, before VESA (or other video) drivers are loaded.
 * Also includes a basic kernel printf()-style function.
 */

#include <portIO.h>
#include <Types.h>
#include <Console.h>

//These are defined here and not in .h because they are only relevant to implementation.
#define VGACONSOLE_HEIGHT	25	//Defines the height of the screen in characters.
#define VGACONSOLE_WIDTH	80	//Defines the width of the screen in characters.
#define VGACONSOLE_BASEADDRESS	0xB8000

//SYNOPSIS: Holds all needed information for Video driver. Packed as tightly as possible.	
	uint16* vgaConsole_videoMemory	= (uint16*) VGACONSOLE_BASEADDRESS;
	uint8 vgaConsole_cursorX	= 0;
	uint8 vgaConsole_cursorY	= 0;
	uint8 vgaConsole_colour		= VID_LIGHT_GREY;


void vgaConsole_updateCursor(void)
{
	// SYNOPSIS:	Moves Blinking Cursor to current X and Y positions by setting CRT Control Register indices 14 and 15.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, WriteByte()
	
	unsigned short int offset;
	
	offset = (vgaConsole_cursorY * 80) + vgaConsole_cursorX;
	
	writeByte(0x3D4, 14);
	writeByte(0x3D5, offset >> 8);
	writeByte(0x3D4, 15);
	writeByte(0x3D5, offset);
}

void vgaConsole_clearScreen(void)
{
	// SYNOPSIS:	X.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, memsetw(), Vid_UpdateCursor()
	
	unsigned short int Blank;
	unsigned short int i;
	
	//Create blank word (Space, Colour)
	Blank = 0x20 | (vgaConsole_colour << 8);

	//Copy byte to Video Memory
	for(i = 0; i < 25*80; i++)
	{
		*(vgaConsole_videoMemory + i) = Blank;
		
		//memsetw(VideoMemory + i * 80, Blank, 80);
	};

	//Update cursor
	vgaConsole_cursorX = 0;
	vgaConsole_cursorY = 0;
	vgaConsole_updateCursor();
}

void vgaConsole_setColour(unsigned char foreColour, unsigned char backColour)
{
	// SYNOPSIS:	Changes current colour by setting attribute byte in VideoInfo structure.
	// INPUT:	Foreground Colour (0-15), Background Colour (0-15)
	// OUTPUT:	None
	// REUIRES:	VideoInfo
	
	if((foreColour < 16) && (backColour < 16))
	{
		vgaConsole_colour = (backColour << 4) | (foreColour);
	} else {
		/* INVALID COLOUR ARRANGEMENT */
	};
}

void vgaConsole_putChar(unsigned char c)
{
	// SYNOPSIS:	Puts a single character on the screen, obeying all ASCII rules.
	// INPUT:	Single character
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, Vid_Scroll(), Vid_UpdateCursor()
	
	unsigned short int *Position;
	
	switch(c)
	{
		case 0x08:	//Backspace
			if(vgaConsole_cursorX != 0)
			{
				vgaConsole_cursorX --;
			};
			break;
		
		case 0x09:	//Tab
			vgaConsole_cursorX = (vgaConsole_cursorX + 8) & ~(8 - 1);
			break;
			
		case 0x0A:	//New Line
			vgaConsole_cursorX = 0;
			vgaConsole_cursorY ++;
			break;
			
		case 0x0D:	//Carriage Return
			vgaConsole_cursorX = 0;
			break;
		
		default:	//All other characters
			if(c >= 0x20)	//If it smaller than a space, then it is a control char, and should be ignored.
			{
				Position = (unsigned short *) vgaConsole_videoMemory + (vgaConsole_cursorY * 80 + vgaConsole_cursorX);
				*Position = c | (vgaConsole_colour << 8);
				vgaConsole_cursorX ++;
			};
			break;
	};
	
	//New Line if edge reached
	if(vgaConsole_cursorX >= 80)
	{
		vgaConsole_cursorX = 0;
		vgaConsole_cursorY ++;
	};
	
	//Scroll if needed, and update cursor.
	//Vid_Scroll();
	vgaConsole_updateCursor();
}

void vgaConsole_putString(const char *Text)
{
	// SYNOPSIS:	Uses PutChar to print a string to screen.
	// INPUT:	Text to output (Variable length)
	// OUTPUT:	None
	// REQUIRES:	PutChar(), strlen()
	
	int i;
	
	for(i=0;Text[i]!=0;i++)
	{
		vgaConsole_putChar(Text[i]);
	};
}
