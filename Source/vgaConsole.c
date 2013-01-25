/**
 * Text console driver used by the kernel at boot-up, before VESA (or other video) drivers are loaded.
 * Also includes a basic kernel printf()-style function.
 * Unlike the rest of the system, this operates in ASCII instead of UTF-8.
 * It's also designed to rely on as few other system components as possible. Basically, just the port IO functions.
 */

#include <portIO.h>
#include <Types.h>
#include <vgaConsole.h>
#include <stdarg.h>

//These are defined here and not in .h because they are only relevant to implementation.
#define VGACONSOLE_HEIGHT	25	//Defines the height of the screen in characters.
#define VGACONSOLE_WIDTH	80	//Defines the width of the screen in characters.

/** Holds the base address of the text area for the VGA controller. */
uint16* vgaConsole_videoMemory	= (uint16*) 0xB8000;

/** Holds the horizontal offset in characters from the left of the screen. */
uint8 vgaConsole_cursorX	= 0;

/** Holds the vertical offset in lines from the top of the screen. */
uint8 vgaConsole_cursorY	= 0;

/** Holds the current value of the VGA attribute byte (which controls the colour of the displayed text). */
uint8 vgaConsole_colour		= VGACONSOLE_LIGHT_GREY;


/**
 * Clears the screen.
 */
void vgaConsole_clearScreen(void)
{
	//While this isn't a particularly gruelling calculation, we may as well only do it once.
	//LSB is a space character, MSB is the currently selected colour.
	uint16 blank = 0x20 | (vgaConsole_colour << 8);

	//Copy this repeatedly to video memory, to cover the whole screen.
	for(int i = 0; i < VGACONSOLE_HEIGHT * VGACONSOLE_WIDTH; i++)
		vgaConsole_videoMemory[i] = blank;

	//Update the location of the cursor on screen.
	vgaConsole_cursorX = 0;
	vgaConsole_cursorY = 0;
	vgaConsole_updateCursor();
}

void vgaConsole_printf(const char* formatString, ...)
{
	//TODO.
	//%d = decimal integer
	//%h = hex integer
	
	va_list args;
	int i = 0;
	int arg;
	
	va_start(args,formatString);
	
	while(formatString[i])
	{
		if(formatString[i]=='%')
		{
			i++;
			arg = va_arg(args,int);
			
			if(formatString[i]=='s')
			{
				//Print a status in a pretty way.
				if(arg)
				{
					//true,ok.
					uint8 storedColour = vgaConsole_colour;
					
					vgaConsole_setColour(VGACONSOLE_WHITE,VGACONSOLE_BLACK);
					vgaConsole_putString("[ ");
					vgaConsole_setColour(VGACONSOLE_GREEN,VGACONSOLE_BLACK);
					vgaConsole_putString("PASS");
					vgaConsole_setColour(VGACONSOLE_WHITE,VGACONSOLE_BLACK);
					vgaConsole_putString(" ]");
					
					vgaConsole_colour = storedColour;
				} else {
					//false,fail.
					uint8 storedColour = vgaConsole_colour;
					
					vgaConsole_setColour(VGACONSOLE_WHITE,VGACONSOLE_BLACK);
					vgaConsole_putString("[ ");
					vgaConsole_setColour(VGACONSOLE_RED,VGACONSOLE_BLACK);
					vgaConsole_putString("FAIL");
					vgaConsole_setColour(VGACONSOLE_WHITE,VGACONSOLE_BLACK);
					vgaConsole_putString(" ]");
					
					vgaConsole_colour = storedColour;
				}
			}
			if(formatString[i]=='d')
			{
				vgaConsole_putDecimal(arg);
			}
			if(formatString[i]=='h')
			{
				vgaConsole_putHexadecimal(arg, 0);
			}
			if(formatString[i]=='H')
			{
				vgaConsole_putHexadecimal(arg, 1);
			}
			if(formatString[i]=='%')
			{
				vgaConsole_putChar('%');
			}
		} else {
			vgaConsole_putChar(formatString[i]);
		}
		
		i++;
	}
	
	vgaConsole_updateCursor();
	
	va_end(args);
}

/**
 * Puts a single character on the screen, obeying all ASCII rules.
 * @param c A single character.
 */
void vgaConsole_putChar(unsigned char c)
{
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
			if(c >= 0x20 && c <= 0x7E)	//If it smaller than a space, then it is a control char, and should be ignored. Or it's a delete (7E) or a UTF char.
			{
				vgaConsole_videoMemory[(vgaConsole_cursorY*VGACONSOLE_WIDTH)+vgaConsole_cursorX] = c | (vgaConsole_colour << 8);
				vgaConsole_cursorX++;
			};
			break;
	};
	
	//New Line if edge reached
	if(vgaConsole_cursorX >= VGACONSOLE_WIDTH)
	{
		vgaConsole_cursorX = 0;
		vgaConsole_cursorY++;
	};
	
	//Scroll if needed.
	if(vgaConsole_cursorY >= VGACONSOLE_HEIGHT)
	{
		vgaConsole_scroll();
	};
}

void vgaConsole_putHexadecimalInternal(uint32 arg);
void vgaConsole_putHexadecimalInternal(uint32 arg)
{
	if(arg/16 >= 1)
		vgaConsole_putHexadecimalInternal(arg/16);
	
	if((arg%16)<10)
	{
		vgaConsole_putChar('0'+(arg%16));
	} else {
		vgaConsole_putChar('A'+((arg%16)-10));
	}
	
	vgaConsole_updateCursor();
}

void vgaConsole_putHexadecimal(uint32 arg, uint8 leadingZeroes)
{
	vgaConsole_putString("0x");
	
	if(leadingZeroes)
	{
		int j;

		for(int i=28;i>=0;i-=4)
		{
			j = (arg & (0xF<<i))>>i;

			if(j<10)
			{
				vgaConsole_putChar('0'+j);
			} else {
				vgaConsole_putChar('A'+(j-10));
			}
		}
	} else {
		vgaConsole_putHexadecimalInternal(arg);
	}
	
	vgaConsole_updateCursor();
}

void vgaConsole_putDecimal(uint32 arg)
{
	if(arg/10 >= 1)
		vgaConsole_putDecimal(arg/10);
	
	vgaConsole_putChar((arg%10)+'0');
	
	vgaConsole_updateCursor();
}

/**
 * Uses PutChar to print a string to screen.
 * @param Text Text to output (Variable length)
 */
void vgaConsole_putString(const char *Text)
{	
	int i;
	
	for(i=0;Text[i]!=0;i++)
	{
		vgaConsole_putChar(Text[i]);
	};
	
	vgaConsole_updateCursor();
}

void vgaConsole_scroll(void)
{
	for(uint16 i = 80;i<(80*25);i++)
	{
		vgaConsole_videoMemory[i-80] = vgaConsole_videoMemory[i];
	}
	
	for(uint16 i = 80*25;i<(80*26);i++)
	{
		vgaConsole_videoMemory[i-80] = 0x0000;
	}
	
	vgaConsole_cursorY--;
	vgaConsole_updateCursor();
}

/**
 * Changes current colour by setting attribute byte in VideoInfo structure.
 * @param foreColour Foreground Colour (0-15)
 * @param backColour Background Colour (0-15)
 */
void vgaConsole_setColour(unsigned char foreColour, unsigned char backColour)
{
	if((foreColour < 16) && (backColour < 16))
	{
		vgaConsole_colour = (backColour << 4) | (foreColour);
	} else {
		/* INVALID COLOUR ARRANGEMENT */
	};
}

/**
 * Moves the VGA BIOS cursor to the current X and Y positions.
 */
void vgaConsole_updateCursor(void)
{
	//The offset, in characters, from the top-left of the screen.
	unsigned short int offset;
	
	//Compute the offset from the current X and Y positions.
	offset = (vgaConsole_cursorY * 80) + vgaConsole_cursorX;
	
	//Write the values to the CRT control register indices 14 and 15.
	writeByte(0x3D4, 14);
	writeByte(0x3D5, offset >> 8);   //High byte.
	writeByte(0x3D4, 15);
	writeByte(0x3D5, offset & 0x08); //Low byte.
}
