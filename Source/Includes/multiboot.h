/* 
 * File:   multiboot.h
 * Author: Jack Scott
 *
 * Created on 25 October 2011, 9:51 AM
 */

#ifndef MULTIBOOT_H
#define	MULTIBOOT_H

#include <Types.h>

#ifdef	__cplusplus
extern "C" {
#endif

	struct multiboot_info {
		uint32 flags;
		uint32 memLower;
		uint32 memUpper;
		uint32 bootDevice;
		char* commandLine;
		uint32 modsCount;
		uint32 modsAddr;
		uint32 syms1;	//28
		uint32 syms2;
		uint32 syms3;
		uint32 syms4;
		uint32 mmapLength; //44
		uint32 mmapAddr; //48
		//More here.
	};


#ifdef	__cplusplus
}
#endif

#endif	/* MULTIBOOT_H */

