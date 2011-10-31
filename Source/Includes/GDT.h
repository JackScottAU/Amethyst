/*
	GDT.h - Global Descriptor Table Header.
	Part of the Jane Microkernel.
	
	Author:		Jack Scott (yayyak@yayyak.net).
	Last Modified:	2006-12-27.
	License:	3-clause BSD License.
	Notes:		None.
*/

#ifndef GDT_H
#define GDT_H

	void gdt_install();
	void* gdt_table;
	void* gdt_pointer;

#endif /* GDT_H */
