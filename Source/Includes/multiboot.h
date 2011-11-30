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
		uint32 memoryLower;
		uint32 memoryUpper;
		uint32 bootDevice;
		char* commandLine;
		uint32 modsCount;
		struct multiboot_moduleNode* modsAddr;
		uint32 syms1;	//28
		uint32 syms2;
		uint32 syms3;
		uint32 syms4;
		uint32 memoryMapLength; //44
		struct multiboot_memoryMapNode* memoryMapAddress; //48
		//More here.
	};
	
	struct multiboot_memoryMapNode {
		uint32 size;
		uint64 addr;
		uint64 len;
		#define MULTIBOOT_MEMORY_AVAILABLE	1
		#define MULTIBOOT_MEMORY_RESERVED	2
		uint32 type;
	} __attribute__((packed));
	
	struct multiboot_moduleNode {
		void* start;
		void* end;
		char* string;
		uint32 reserved;
	} __attribute__((packed));

#ifdef	__cplusplus
}
#endif

#endif	/* MULTIBOOT_H */

