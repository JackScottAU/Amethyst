// Jack's Video Driver.
// Yayyak@gmail.com, December 2005


#include <portIO.h>

	// SYNOPSIS:	Holds all needed information for Video driver. Packed as tightly as possible.
	
	unsigned char *VideoMemory	= (unsigned char *) 0xB8000;
	unsigned char CursorX		= 0;
	unsigned char CursorY		= 0;
	unsigned char Colour		= 0x07;


/*
void Vid_Scroll() //UNFINISHED
{
	// SYNOPSIS:	Moves everything on screen up one line.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	memcpy(), memsetw(), VideoInfo
	
	unsigned short int Blank;
	unsigned short int Lines;
	
	//Create blank word (Space, Colour)
	Blank = 0x20 | (Colour << 8);
	
	// Row 25 is the end, this means we need to scroll up *
	if(CursorY >= 25)
	{
		//How many lines to move back.
		Lines = CursorY - 24;
		
		//Copy the screen memory back one line.
		memcpy(VideoMemory, VideoMemory+(Lines*80), (25-Lines)*160);
		
		//Set the last line to blank.
		memsetw(VideoMemory+((25-Lines)*80), Blank, 80);
		
		//Set the cursor back one line.
		CursorY = 24;
	};
}
*/

void Vid_UpdateCursor()
{
	// SYNOPSIS:	Moves Blinking Cursor to current X and Y positions by setting CRT Control Register indices 14 and 15.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, WriteByte()
	
	unsigned short int Offset;
	
	Offset = (CursorY * 80) + CursorX;
	
	writeByte(0x3D4, 14);
	writeByte(0x3D5, Offset >> 8);
	writeByte(0x3D4, 15);
	writeByte(0x3D5, Offset);
}
	
void Vid_ClearScreen()
{
	// SYNOPSIS:	X.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	VideoInfo, memsetw(), Vid_UpdateCursor()
	
	unsigned short int Blank;
	unsigned short int i;

	//Create blank word (Space, Colour)
	Blank = 0x20 | (Colour << 8);

	//Copy byte to Video Memory
	for(i = 0; i < 25*80; i++)
	{
		*(VideoMemory + i) = Blank;
		
		//memsetw(VideoMemory + i * 80, Blank, 80);
	};

	//Update cursor
	CursorX = 0;
	CursorY = 0;
	Vid_UpdateCursor();
}

void Vid_SetColour(unsigned char ForeColour, unsigned char BackColour)
{
	// SYNOPSIS:	Changes current colour by setting attribute byte in VideoInfo structure.
	// INPUT:	Foreground Colour (0-15), Background Colour (0-15)
	// OUTPUT:	None
	// REUIRES:	VideoInfo
	
	if((ForeColour < 16) && (BackColour < 16))
	{
		Colour = (BackColour << 4) | (ForeColour);
	} else {
		/* INVALID COLOUR ARRANGEMENT */
	};
}

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
			if(CursorX != 0)
			{
				CursorX --;
			};
			break;
		
		case 0x09:	//Tab
			CursorX = (CursorX + 8) & ~(8 - 1);
			break;
			
		case 0x0A:	//New Line
			CursorX = 0;
			CursorY ++;
			break;
			
		case 0x0D:	//Carriage Return
			CursorX = 0;
			break;
		
		default:	//All other characters
			if(c >= 0x20)	//If it smaller than a space, then it is a control char, and should be ignored.
			{
				Position = (unsigned short *) VideoMemory + (CursorY * 80 + CursorX);
				*Position = c | (Colour << 8);
				CursorX ++;
			};
			break;
	};
	
	//New Line if edge reached
	if(CursorX >= 80)
	{
		CursorX = 0;
		CursorY ++;
	};
	
	//Scroll if needed, and update cursor.
	//Vid_Scroll();
	Vid_UpdateCursor();
}

void PutString(char *Text)
{
	// SYNOPSIS:	Uses PutChar to print a string to screen.
	// INPUT:	Text to output (Variable length)
	// OUTPUT:	None
	// REQUIRES:	PutChar(), strlen()
	
	int i;
	
	for(i=0;Text[i]!=0;i++)
	{
		Vid_PutChar(Text[i]);
	};
}
