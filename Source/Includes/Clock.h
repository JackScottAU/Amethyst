
#ifndef CLOCK_H
#define CLOCK_H

#include <Types.h>

	void clock_init(void);
	void clock_shutdown(void);
	void clock_handler_PIC(void);
	unsigned long Clock_Uptime(void);

	void clock_setHertz(unsigned int Hertz);

	#define CLOCK_HERTZ 1000

	typedef struct {
		uint64 seconds;
		uint16 milliSeconds;
	} time_t;
	
	//EPOCH: 2000-01-01 00:00:00.000

#endif