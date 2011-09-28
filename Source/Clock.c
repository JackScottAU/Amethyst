/*
	Clock.c - Time/Date and Timer Functions.
	Part of the Jane Microkernel.
	
	Maintainer:	Jack Scott (yayyak@yayyak.net).
	Last Modified:	2007-01-05.
	License:	3-clause BSD License.
	Notes:		None.
*/

#include <port.h>
#include <Clock.h>

//Holds uptime (Secs*CLOCK_HERTZ)
unsigned long Clock_Ticks;


void Clock_SetHertz(unsigned int Hertz)
{
	/*
	SYNOPSIS:	Sets the divisor of the channel 0 clock.
	INPUT:		Requested frequency.
	OUTPUT:		None.
	NOTES:		None.
	*/
	
	int Divisor;
	Divisor = 1193180 / Hertz;
	writeIO(0x43, 0x36);			//Set command byte.
	writeIO(0x40, Divisor & 0xFF);	//LSB
	writeIO(0x40, Divisor >> 8);		//MSB
}



void Clock_Handler()
{
	Clock_Ticks++;
}

unsigned long Clock_Uptime()
{
	return (Clock_Ticks / CLOCK_HERTZ);
}

void Clock_Init()
{
	Clock_SetHertz(CLOCK_HERTZ);
	Clock_Ticks = 0;
}
