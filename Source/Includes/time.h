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
		sint64 seconds;
		uint16 partseconds; //65536ths of a second
	} time_t;


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_H */
