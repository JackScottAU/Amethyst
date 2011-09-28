// Jack's Keyboard Driver.
// Yayyak@gmail.com, December 2005

#include <system.h>
#include "Keyboard.h"
#include "KeyMap.h"

struct KeyboardInfoStruct{
	// SYNOPSIS:	Holds all needed information for Keyboard driver. Could be packed a lot better.
	
	unsigned char Buffer[256]	= 0x00;
	unsigned char Position		= 0;
	unsigned char ShiftHeld	= 0;
	unsigned char ControlHeld	= 0;
	unsigned char AltHeld		= 0;
	unsigned char CapsHeld		= 0;
	unsigned char ReleaseKey	= 0;
} KeyboardInfo;


void Key_Setup()
{
	// SYNOPSIS:	Sets up Keyboard IRQ handler to fire on key hit, and selects keyboard scancode.
	// INPUT:	None
	// OUTPUT:	None
	// REQUIRES:	irq_install_handler(), Key_Handler(), inportb(), outportb()
	
	unsigned char i;
	
	irq_install_handler(1, Key_Handler);
	
	while(i=0)		//Wait for keyboard buffer to be empty
		if((inportb(KEYBOARD_STATUS) << 7) == 0x80)
			i=1;
	
	outportb(KEYBOARD_COMMAND,0xF0);	//Select Keyboard Scancode 3
	outportb(KEYBOARD_COMMAND,0x03);
	
};

void Key_Handler(struct regs *r)
{
	// SYNOPSIS:	Keyboard Interrupt Handler. Gets scancode from keyboard, converts, and puts in buffer.
	// INPUT:	Register values from ASM code
	// OUTPUT:	None
	// REQUIRES:	inportb(), KeyboardInfo, Keyboard_US
	
	unsigned char Scancode;
	unsigned char Character;
	
	//Get Scancode from Keyboard.
	Scancode = inportb(KEYBOARD_DATA);
	
	if (Scancode & 0xF0)
	{
		//We have a break code. Read another byte, and find out what key was released.
		Scancode = inportb(KEYBOARD_DATA);
		switch(Scancode)
		{
			case 0x11:
			case 0x58:
				//Control Has been released.
				KeyboardInfo.ControlHeld = 0;
				break;
			
			case 0x12:
			case 0x59:
				//Shift has been released.
				KeyboardInfo.ShiftHeld = 0;
				break;
			
			case 0x14:
				//Capslock has been released.
				KeyboardInfo.CapsHeld = 0;
				break;
			
			case 0x19:
			case 0x39:
				//Alt has been released.
				KeyboardInfo.AltHeld = 0;
				break;
		};
	}
	else
	{
		//We have a make code. Find out what key was pressed.
		switch(Scancode)
		{
			case 0x11:
			case 0x58:
				//Control Has been pressed.
				KeyboardInfo.ControlHeld = 1;
				break;
			
			case 0x12:
			case 0x59:
				//Shift has been pressed.
				KeyboardInfo.ShiftHeld = 1;
				break;
			
			case 0x14:
				//Capslock has been pressed.
				KeyboardInfo.CapsHeld = 1;
				break;
			
			case 0x19:
			case 0x39:
				//Alt has been pressed.
				KeyboardInfo.AltHeld = 1;
				break;
				
			default:
				if(KeyboardInfo.ShiftHeld == 1)
				{
					Character = KeyMap_US_3_U[Scancode];
				} else {
					Character = KeyMap_US_3_L[Scancode];
				};
				AddToBuffer(Character);
		};
	};
};

void Key_FlushBuffer()
{
	// SYNOPSIS:	Flushes the internal buffer.
	// INPUT:	
	// OUTPUT:	
	// REQUIRES:	KeyboardInfo
	
	unsigned char i;
	
	for(i=0, i<=255, i=i+1)
	{
		KeyboardInfo.Buffer[i] = 0x00;
	};
};

void Key_AddToBuffer(unsigned char Character) //UNFINISHED
{
	KeyboardInfo.Buffer[KeyboardInfo.Position] = Character;
	KeyboardInfo.Position ++;
};

unsigned char Key_TakeFromBuffer() //UNFINISHED
{
	unsigned char Character;
	
	KeyboardInfo.Position --;
	Character = KeyboardInfo.Buffer[KeyboardInfo.Position];
	KeyboardInfo.Buffer[KeyboardInfo.Position] = 0x00;
	
	return Character;
};