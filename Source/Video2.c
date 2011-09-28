// Jack's Video Driver.
// Yayyak@gmail.com, December 2005

#include <system.h>
#include "Video.h"

struct {
	// SYNOPSIS:	Holds all needed information for Video driver. Packed as tightly as possible.
	
	unsigned char *VideoMemory	= (unsigned char *) VIDEO_MEM;
	unsigned char CursorX		= 0;
	unsigned char CursorY		= 0;
	unsigned char Colour		= 0;
} VideoInfo;


void Vid_Scroll() //UNFINISHED
{
	// SYNOPSIS:	Moves everything on screen up one line.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	memcpy(), memsetw(), VideoInfo
	
	unsigned short int Blank;
	unsigned short int Lines;
	
	//Create blank word (Space, Colour)
	Blank = 0x20 | (VideoInfo.Colour << 8);
	
	/* Row 25 is the end, this means we need to scroll up */
	if(VideoInfo.CursorY >= 25)
	{
		//How many lines to move back.
		Lines = VideoInfo.CursorY - 24;
		
		//Copy the screen memory back one line.
		memcpy(VideoInfo.VideoMemory, VideoInfo.VideoMemory+(Lines*80), (25-Lines)*160);
		
		//Set the last line to blank.
		memsetw(VideoInfo.VideoMemory+((25-Lines)*80), Blank, 80);
		
		//Set the cursor back one line.
		VideoInfo.CursorY = 24;
	};
};

void Vid_ClearScreen()
{
	// SYNOPSIS:	Moves Blinking Cursor to current X and Y positions by setting CRT Control Register indices 14 and 15.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, memsetw(), Vid_UpdateCursor()
	
	unsigned short int Blank;
	unsigned short int i;

	//Create blank word (Space, Colour)
	Blank = 0x20 | (VideoInfo.Colour << 8);

	//Copy byte to Video Memory
	for(i = 0; i < 25; i++)
	{
		memsetw(VideoInfo.VideoMemory + i * 80, blank, 80);
	};

	//Update cursor
	VideoInfo.CursorX = 0;
	VideoInfo.CursorY = 0;
	Vid_UpdateCursor();
};

void Vid_UpdateCursor()
{
	// SYNOPSIS:	Moves Blinking Cursor to current X and Y positions by setting CRT Control Register indices 14 and 15.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, outportb()
	
	unsigned short int Offset;
	
	Offset = (VideoInfo.CursorY * 80) + VideoInfo.CursorX;
	
	outportb(0x3D4, 14);
	outportb(0x3D5, Offset >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, Offset);
};

void Vid_SetColour(unsigned char ForeColour, unsigned char BackColour)
{
	// SYNOPSIS:	Changes current colour by setting attribute byte in VideoInfo structure.
	// INPUT:	Foreground Colour (0-15), Background Colour (0-15)
	// OUTPUT:	None
	// REUIRES:	VideoInfo
	
	if((ForeColour < 16) && (BackColour < 16))
	{
		VideoInfo.Colour = (BackColour << 4) | (ForeColour);
	} else {
		/* INVALID COLOUR ARRANGEMENT */
	};
};

void Vid_PutChar(unsigned char c)
{
	// SYNOPSIS:	Puts a single character on the screen, obeying all ASCII rules.
	// INPUT:	Single character
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, Vid_Scroll(), Vid_UpdateCursor()
	
	unsigned short int *Position;
	
	switch(c)
	{
		case 0x08:	//Backspace
			if(VideoInfo.CursorX != 0)
			{
				VideoInfo.CursorX --;
			};
			break;
		
		case 0x09:	//Tab
			VideoInfo.CursorX = (VideoInfo.CursorX + 8) & ~(8 - 1);
			break;
			
		case 0x0A:	//New Line
			VideoInfo.CursorX = 0;
			VideoInfo.CursorY ++;
			break;
			
		case 0x0D:	//Carriage Return
			VideoInfo.CursorX = 0;
			break;
		
		default:	//All other characters
			if(c >= 0x20)	//If it smaller than a space, then it is a control char, and should be ignored.
			{
				Position = VideoInfo.VideoMemory + (VideoInfo.CursorY * 80 + VideoInfo.CursorX);
				*Position = c | (VideoInfo.Colour << 8);
				VideoInfo.CursorX ++;
			};
			break;
	};
	
	//New Line if edge reached
	if(VideoInfo.CursorX >= 80)
	{
		VideoInfo.CursorX = 0;
		VideoInfo.CursorY ++;
	};
	
	//Scroll if needed, and update cursor.
	Vid_Scroll();
	Vid_UpdateCursor();
};
