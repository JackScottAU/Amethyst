/**
 *  Amethyst Operating System - Multiboot v1 Information Structure.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef MULTIBOOT_H
#define	MULTIBOOT_H

#include <Types.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define MULTIBOOT_MAGIC_NUMBER	0x2BADB002

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

		uint32 drives_length;
		uint32 drives_addr;

		uint32 config_table;

		uint32 boot_loader_name;

		uint32 apm_table;

		uint32 vbe_control_info;
		uint32 vbe_mode_info;
		uint16 vbe_mode;
		uint16 vbe_interface_seg;
		uint16 vbe_interface_off;
		uint16 vbe_interface_len;

		// offset 88
		void* framebuffer_addr;
		uint32 framebuffer_addr_upper;
		uint32 framebuffer_pitch;
		uint32 framebuffer_width;
		uint32 framebuffer_height;
		uint8 framebuffer_bpp;

		uint8 framebuffer_type; // 0 = indexed colour, 1 = direct colour, 2 = ega text

		uint8 framebuffer_red_field_position;
		uint8 framebuffer_red_mask_size;
		uint8 framebuffer_green_field_position;
		uint8 framebuffer_green_mask_size;
		uint8 framebuffer_blue_field_position;
		uint8 framebuffer_blue_mask_size;
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

