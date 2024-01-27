/* 
 * File:   string.h
 * Author: Jack Scott
 */

#ifndef STRING_H
#define	STRING_H

#ifdef	__cplusplus
extern "C" {
#endif
	
	void string_copy(char *dest, const char* src);

	/**
	 * Formats a string.
	 * @param destination Destination for the string output. Must have sufficient size. Memory allocation is the responsibility of the caller.
	 * @param formatString TODO.
	*/
	//void string_format(char* destination, const char* formatString, ...);

#ifdef	__cplusplus
}
#endif

#endif	/* STRING_H */

