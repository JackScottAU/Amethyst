/*
	Clock.c - Time/Date and Timer Functions.
 * 
 *	Basically it uses the RTC to load initial data, and then the PIT (or HP-clock) to update that.
*/

#include <portIO.h>
#include <Clock.h>
#include <vgaConsole.h>

//Holds uptime (Secs*CLOCK_HERTZ)
unsigned long Clock_Ticks;

time_t clock_systemClock;

void clock_setHertz(unsigned int Hertz)
{
	/*
	SYNOPSIS:	Sets the divisor of the channel 0 clock.
	INPUT:		Requested frequency.
	OUTPUT:		None.
	NOTES:		None.
	*/
	
	int Divisor;
	Divisor = 1193180 / Hertz;
	writeByte(0x43, 0x36);			//Set command byte.
	writeByte(0x40, Divisor & 0xFF);	//LSB
	writeByte(0x40, Divisor >> 8);		//MSB
}

/**
 * \brief Handles the clock-tick message produced by the programmable interrupt timer (PIT).
 */
void clock_handler_PIC()
{
	clock_systemClock.milliSeconds++;
	
	if(clock_systemClock.milliSeconds%CLOCK_HERTZ==0)
	{
		clock_systemClock.seconds++;
		clock_systemClock.milliSeconds = 0;
		
		vgaConsole_printf("Clock: %h\n",clock_systemClock.seconds);
	}
	
	//There are a list of things that need to be notified on clockticks... visit them here.
}

unsigned long Clock_Uptime()
{
	return (Clock_Ticks / CLOCK_HERTZ);
}

void clock_init()
{
	clock_setHertz(CLOCK_HERTZ);
	
	clock_systemClock.seconds = 0;
	clock_systemClock.milliSeconds = 0;
	
	//Get time from RTC here.
}

void clock_shutdown()
{
	//Give back time to the RTC for next time.
}
