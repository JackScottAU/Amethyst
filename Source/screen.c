//screen.c - Simple Console Output

//Copyright (c) 2006-2009, Jack Scott (jack@jackscott.org)
//Released under the 3-clause BSD License (see License)

unsigned short* videoMemory = (unsigned short*) 0xB8000;
unsigned char textColour = 0x07;
unsigned char xpos = 0;
unsigned char ypos = 0;


void clearScreen()
{
	unsigned short fill = (textColour<<8)+0x20;
	for(int i=0;i<(80*25);i++)
		videoMemory[i]=fill;
}

void printChar(char character, unsigned char x, unsigned char y, unsigned char colour)
{
  unsigned short fill = (colour<<8)+character;
  videoMemory[(y*80)+x] = fill;
}

void printString(char* string, void* extraVal)
{
  int i=0;
  while (string[i]!='\0')
    {
      switch(string[i])
	{
	case '\n':
	  xpos=0;ypos++;
	  break;
	default:
	  printChar(string[i],xpos++,ypos,textColour);
	  break;
	}
      i++;
    }
}
