/**
 *  Amethyst Operating System - Time/Date and Timer Functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <portIO.h>
#include <Clock.h>
#include <memoryManager.h>
#include <interrupts.h>
#include <debug.h>

time_t clock_systemClock;

clock_timerRequest* clock_timerRequestsList = (clock_timerRequest*) 0x00;

int clock_daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

clock_timerRequest* clock_addOneShotRequest(time_t* requestedTime, void (*funcToCall)(void)) {
    clock_timerRequest* request = memoryManager_allocate(sizeof(clock_timerRequest));

    request->seconds = requestedTime->seconds;
    request->milliSeconds = requestedTime->milliSeconds;
    request->funcToCall = funcToCall;
    request->isRepeatTimer = 0;

    // Add to queue.
    // Do magic to try and find where to place this in the list.
    if (!clock_timerRequestsList) {
        request->next = (clock_timerRequest*) 0x00;
        clock_timerRequestsList = request;
    } else {
        // Add it to the front of the list. We'll figure out sorting it later.
        request->next = clock_timerRequestsList;
        clock_timerRequestsList = request;
    }

    return(request);
}

clock_timerRequest* clock_addRepeatRequest(uint64 secGap, uint16 milGap, void (*funcToCall)(void)) {
    clock_timerRequest* request = memoryManager_allocate(sizeof(clock_timerRequest));

    request->seconds = clock_systemClock.seconds + secGap;
    request->milliSeconds = clock_systemClock.milliSeconds + milGap;
    request->funcToCall = funcToCall;
    request->isRepeatTimer = 1;
    request->repeatSecondsToAdd = secGap;
    request->repeatMilliSecondsToAdd = milGap;

    // Add to queue.
    // Do magic to try and find where to place this in the list.
    if (!clock_timerRequestsList) {
        request->next = (clock_timerRequest*) 0x00;
        clock_timerRequestsList = request;
    } else {
        // Add it to the front of the list. We'll figure out sorting it later.
        request->next = clock_timerRequestsList;
        clock_timerRequestsList = request;
    }

    return(request);
}

void clock_deleteTimerRequest(clock_timerRequest* request) {
    clock_timerRequest* current = clock_timerRequestsList;
    clock_timerRequest* oldCurrent = 0;

    if (current == request) {     // Deals with head-of-list issues.
        clock_timerRequestsList = current->next;
        memoryManager_free(current);

        // So we don't continue on to the next section and try to delete things twice and make a huge mess of the entire
        // system.
        current = 0;
    }

    while (current) {
        // Perform check.
        if (current == request) {
            // Remove it from the list.
            clock_timerRequest*  temp = current;

            oldCurrent->next = current->next;
            current = oldCurrent->next;

            memoryManager_free(temp);
        } else {
            oldCurrent = current;
            current = current->next;
        }
    }
}

/**
 * Sets the divisor of the PIT channel 0 clock.
 * @param hertz Requested frequency.
 */
void clock_setHertz(unsigned int hertz) {
    int Divisor;
    Divisor = 1193180 / hertz;
    portIO_write8(0x43, 0x36);              // Set command byte.
    portIO_write8(0x40, Divisor & 0xFF);    // LSB
    portIO_write8(0x40, Divisor >> 8);      // MSB
}

/**
 * \brief Handles the clock-tick message produced by the programmable interrupt timer (PIT).
 */
void clock_handler_PIC(uint32 eventData) {
    clock_systemClock.milliSeconds++;

    if (clock_systemClock.milliSeconds%CLOCK_HERTZ == 0) {
        clock_systemClock.seconds++;
        clock_systemClock.milliSeconds = 0;
    }

    // There are a list of things that need to be notified on clockticks... visit them here.
    clock_timerRequest* current = clock_timerRequestsList;
    clock_timerRequest* oldCurrent = 0;
    while (current) {
        // Perform check.
        if ((current->seconds <= clock_systemClock.seconds) && (current->milliSeconds <= clock_systemClock.milliSeconds)) {
            // We have a shot!
            void (*foo)();
            foo = current->funcToCall;
            (*foo)();   // Call the function.
   //      vgaConsole_printf("Shot! %h.%h",current->seconds,current->milliSeconds);

            if (current->isRepeatTimer) {
                // Reset to give it another go.
                current->seconds = current->seconds + current->repeatSecondsToAdd;
                current->milliSeconds = current->milliSeconds + current->repeatMilliSecondsToAdd;

                if (current->milliSeconds >= 1000) {
                    current->seconds++;
                    current->milliSeconds = current->milliSeconds-1000;
                }

                oldCurrent = current;
                current = current->next;
            } else {
                // Remove it from the list.
                clock_timerRequest*  temp = current;

                oldCurrent->next = current->next;
                current = oldCurrent->next;

                memoryManager_free(temp);
            }
        } else {
            oldCurrent = current;
            current = current->next;
        }

        // vgaConsole_printf("list: %h.%h\n",current,oldCurrent);
    }
}

void clock_init() {
    clock_setHertz(CLOCK_HERTZ);

    clock_systemClock.seconds = 0;
    clock_systemClock.milliSeconds = 0;

    interrupts_addHandler(0x20, 0, (*clock_handler_PIC));

    // We now proceed to get an approx. fix on time from the RTC. This will
    // be recalculated once we have a better fix from a network time server.

    uint8 statusB = clock_getRTCRegister(0x0B);

    // Read seconds.
    if (statusB & 0x04) {
        clock_systemClock.seconds += clock_getRTCRegister(0x00);
    } else {
        clock_systemClock.seconds += clock_convertBCDtoNormal(clock_getRTCRegister(0x00));
    }

    // Read minutes.
    if (statusB & 0x04) {
        clock_systemClock.seconds += SECONDS_PER_MINUTE * clock_getRTCRegister(0x02);
    } else {
        clock_systemClock.seconds += SECONDS_PER_MINUTE * clock_convertBCDtoNormal(clock_getRTCRegister(0x02));
    }

    // Read hours.
    if (statusB & 0x02) {
        // 24-hour mode.
        if (statusB & 0x04) {
            clock_systemClock.seconds += SECONDS_PER_HOUR * (clock_getRTCRegister(0x04)+(CLOCK_UTC_OFFSET));
        } else {
            clock_systemClock.seconds += SECONDS_PER_HOUR *
                            (clock_convertBCDtoNormal(clock_getRTCRegister(0x04)) + (CLOCK_UTC_OFFSET));
        }
    } else {
        // 12-hour mode.
        int temp = clock_getRTCRegister(0x04);
        int ampm = temp & 0x80;

        if (ampm)   // Mask the 0x80 bit out.
            temp = temp & 0x7F;

        if (!(statusB & 0x04))
            temp = clock_convertBCDtoNormal(temp);

        if (temp == 12)
            temp = 0;

        if (ampm)
            temp = temp + 12;

        clock_systemClock.seconds += SECONDS_PER_HOUR * (temp+(CLOCK_UTC_OFFSET));
    }

    // Read days.
    if (statusB & 0x04) {
        clock_systemClock.seconds += SECONDS_PER_DAY * clock_getRTCRegister(0x07);
    } else {
        clock_systemClock.seconds += SECONDS_PER_DAY * clock_convertBCDtoNormal(clock_getRTCRegister(0x07));
    }

    // Read months.
    int month;
    int totaldays = 0;
    if (statusB & 0x04) {
        month = clock_getRTCRegister(0x07) - 1;     // Comes out of clock as 1-12. We need 0-11.
    } else {
        month = clock_convertBCDtoNormal(clock_getRTCRegister(0x07)) - 1;
    }

    // We now need to add up all the other days in the preceeding months.
    for (int i =0; i < month; i++) {
        totaldays += clock_daysPerMonth[month];
    }

    clock_systemClock.seconds += SECONDS_PER_DAY * totaldays;


    // Read years. This register gives us number of years since last century (2000) so we can just add straight on.
    // We really should check the century, but we don't.
    int year;
    if (statusB & 0x04) {
        year = clock_getRTCRegister(0x09);
    } else {
        year = clock_convertBCDtoNormal(clock_getRTCRegister(0x09));
    }
    clock_systemClock.seconds += SECONDS_PER_YEAR * year;

    // Now add in any leap days from feb.
    // The Gregorian leap year rules are that if a year is a multiple of 4 then it's a leap year,
    // unless it happens to be a new century and it can't be divided by 400. <-- from OSDev wiki.
    int numberOfLeapYearDays = 0;
    numberOfLeapYearDays = (year/4)+1;  // This works until 2100.
    clock_systemClock.seconds += SECONDS_PER_DAY * numberOfLeapYearDays;
}

void clock_shutdown() {
    // TODO(JackScottAU): Give back time to the RTC for next time.
}

uint32 clock_uptime() {
    return (clock_systemClock.seconds);
}

uint8 clock_convertBCDtoNormal(uint8 value) {
    value = ((value / 16) * 10) + (value & 0x0F);
    return(value);
}

uint8 clock_getRTCRegister(uint8 chosenRegister) {
    uint8 data = 0;

    interrupts_disableInterrupts();

    portIO_write8(0x70, chosenRegister);

    // DELAY HERE.

    data = portIO_read8(0x71);

    interrupts_enableInterrupts();

    return(data);
}
