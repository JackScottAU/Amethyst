/*
	Clock.c - Time/Date and Timer Functions.
 * 
 *	Basically it uses the RTC to load initial data, and then the PIT (or HP-clock) to update that.
*/

#include <portIO.h>
#include <Clock.h>
#include <vgaConsole.h>
#include <interrupts.h>

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
	return (clock_systemClock.seconds);
}

void clock_init()
{
	clock_setHertz(CLOCK_HERTZ);
	
	clock_systemClock.seconds = 0;
	clock_systemClock.milliSeconds = 0;
	
	uint8 statusB = clock_getRTCRegister(0x0B);
	
	//Read seconds.
	if(statusB & 0x04)
	{
		clock_systemClock.seconds += clock_getRTCRegister(0x00);
	} else {
		clock_systemClock.seconds += clock_convertBCDtoNormal(clock_getRTCRegister(0x00));
	}
	
	//Read minutes.
	if(statusB & 0x04)
	{
		clock_systemClock.seconds += SECONDS_PER_MINUTE * clock_getRTCRegister(0x02);
	} else {
		clock_systemClock.seconds += SECONDS_PER_MINUTE * clock_convertBCDtoNormal(clock_getRTCRegister(0x02));
	}
}

void clock_shutdown()
{
	//Give back time to the RTC for next time.
}

uint8 clock_convertBCDtoNormal(uint8 value)
{
	value = ((value / 16) * 10) + (value & 0x0F);
	return(value);
}

uint8 clock_getRTCRegister(uint8 chosenRegister)
{
	uint8 data = 0;
	
	interrupts_disableInterrupts();
	
	writeByte(0x70,chosenRegister);
	
	//DELAY HERE.
	
	data = readByte(0x71);
	
	interrupts_enableInterrupts();
	
	return(data);
}
