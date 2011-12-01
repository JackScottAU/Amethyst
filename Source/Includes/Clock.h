
#ifndef CLOCK_H
#define CLOCK_H

#include <Types.h>

	typedef struct {
		uint64 seconds;
		uint16 milliSeconds;
	} time_t;
	
	typedef struct clock_timerRequest_s {
		uint64 seconds;
		uint16 milliSeconds;
		void (* funcToCall)(void);
		uint32 isRepeatTimer;
		uint64 repeatSecondsToAdd;
		uint16 repeatMilliSecondsToAdd;
		struct clock_timerRequest_s* next;
	} clock_timerRequest;
	
	void clock_init(void);
	void clock_shutdown(void);
	void clock_handler_PIC(uint32 arbitraryNumber);
	unsigned long clock_uptime(void);

	void clock_setHertz(unsigned int Hertz);
	
	uint8 clock_getRTCRegister(uint8 chosenRegister);
	void clock_updateTimeFromRTC(time_t* clock);
	uint8 clock_convertBCDtoNormal(uint8 value);
	
	clock_timerRequest* clock_addOneShotRequest(time_t* requestedTime, void (*funcToCall)(void));
	clock_timerRequest* clock_addRepeatRequest(uint64 secGap, uint16 milGap, void (*funcToCall)(void));
	void clock_deleteTimerRequest(clock_timerRequest* request);

	#define CLOCK_HERTZ 1000

	#define SECONDS_PER_MINUTE 60
	#define SECONDS_PER_HOUR 3600
	#define SECONDS_PER_DAY 86400
	#define SECONDS_PER_YEAR 31556926 //According to Google. We ignore leap seconds in this approximate fix.

	//If the RTC is in local time, this is the number that needs to be added in order to get UTC.
	//Example: I am in UTC+11, so my value is -11 (11-11=0)
	#define CLOCK_UTC_OFFSET 0
	
	//EPOCH: 2000-01-01 00:00:00.000

#endif