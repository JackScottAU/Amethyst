// Jack's Video Driver.
// Yayyak@gmail.com, December 2005

#include <system.h>
#include "Video.h"
#include "Keyboard.h"
//#include "Bell.h"
//#include "Serial.h"

struct ConsoleInfoStruct{
	//SYNOPSIS:	Contains all the information needed for the proper running of the Console.
	unsigned char Console = 0x00;
} ConsoleInfo;





void PutString(unsigned char *Text)
{
	// SYNOPSIS:	Uses PutChar to print a string to screen.
	// INPUT:	Text to output (Variable length)
	// OUTPUT:	None
	// REQUIRES:	PutChar(), strlen()
	
	int i;
	
	for (i = 0; i < strlen(Text); i++)
	{
		PutChar(Text[i]);
	};
};

void PutChar(unsigned char C) //UNFINISHED
{
	// SYNOPSIS:	Performs first round of control character sorting, then chooses and uses routines to output data.
	// INPUT:	Character to Output
	// OUTPUT:	None
	// REQUIRES:	
	
	switch(C)
	{
		case 0x07:	//Bell
			//RingBell();
			break;
		
		default:	//All other characters
			switch(ConsoleInfo.Console & 0x0F)
			{	//Choose a nice output device...
				case CONSOLE_VIDEO:
					Vid_PutChar(C);	//And use it!
					break;
			};
			break;
	};
};
