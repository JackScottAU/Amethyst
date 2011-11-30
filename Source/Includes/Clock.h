
#ifndef CLOCK_H
#define CLOCK_H

#include <Types.h>

	typedef struct {
		uint64 seconds;
		uint16 milliSeconds;
	} time_t;
	
	void clock_init(void);
	void clock_shutdown(void);
	void clock_handler_PIC(void);
	unsigned long Clock_Uptime(void);

	void clock_setHertz(unsigned int Hertz);
	
	uint8 clock_getRTCRegister(uint8 chosenRegister);
	void clock_updateTimeFromRTC(time_t* clock);
	uint8 clock_convertBCDtoNormal(uint8 value);

	#define CLOCK_HERTZ 1000

	#define SECONDS_PER_MINUTE 60
	#define SECONDS_PER_HOUR
	#define SECONDS_PER_DAY
	#define DAYS_IN_JANUARY
	#define SECONDS_PER_YEAR
	
	//EPOCH: 2000-01-01 00:00:00.000

#endif