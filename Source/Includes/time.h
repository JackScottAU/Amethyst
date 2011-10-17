/* 
 * File:   time.h
 * Author: Jack Scott
 *
 * Created on 17 October 2011, 5:57 PM
 */

#ifndef TIME_H
#define	TIME_H

#include <Types.h>

#ifdef	__cplusplus
extern "C" {
#endif

	typedef struct {
		sint64 seconds:54;
		uint16 milliseconds:10; //Values 1000 to 1023 are invalid.
	} time_t;


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */

