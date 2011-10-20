;This is the code to load my manually built GDT table into the memory.

global gdt_install

gdt_install:
	lgdt [gdtPointer]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:gdtFlush
gdtFlush:
	ret

align 32
gdtTable:	;The actual GDT table.
	dd 0x00000000	;Empty descriptor.
	dd 0x00000000

	dd 0x0000FFFF	;Code descriptor (ring 0). 0GB->4GB.
	dd 0x00CF9A00
	
	dd 0x0000FFFF	;Data descriptor (ring 0). 0GB->4GB.
	dd 0x00CF9200

	dd 0x0000FFFF	;Code descriptor (ring 3). 0GB->4GB.
	dd 0x00CFFA00
	
	dd 0x0000FFFF	;Data descriptor (ring 3). 0GB->4GB.
	dd 0x00CFF200

	dd 0x0000FFFF	;Empty TSS record. The base and limit need to be changed later. Base = everything that is zero, limit = everything that is F.
	dd 0x004F8900

align 32
gdtPointer:	;Pointer basically tells CPU how big table is.
	dw 47		;Size of table.
	poo equ gdtTable
	dd poo		;Location of table.
