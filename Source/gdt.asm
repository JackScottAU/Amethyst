;This is the code to load my manually built GDT table into the memory.

global gdtInstall

gdtInstall:
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
	
	dd 0xFFFF0000	;Code descriptor. 0GB->4GB.
	dd 0x009ACF00
	
	dd 0xFFFF0000	;Data descriptor. 0GB->4GB.
	dd 0x0092CF00

align 32
gdtPointer:	;Pointer basically tells CPU how big table is.
	dw 23		;Size of table.
	poo equ gdtTable
	dd poo		;Location of table.


