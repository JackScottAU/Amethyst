;This is the code to load my manually built GDT table into the memory.

global gdt_install
global gdt_pointer
global gdt_table

gdt_install:	;Installs a new GDT and reloads segment registers.
	lgdt [gdt_pointer]
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
gdt_table:	;The actual GDT table.
	dd 0x00000000	;Empty descriptor.
	dd 0x00000000

	dd 0x0000FFFF	;Code descriptor (ring 0). 0GB->4GB.
	dd 0x00CF9A00
	
	dd 0x0000FFFF	;Data descriptor (ring 0). 0GB->4GB.
	dd 0x00CF9200

align 32
gdt_pointer:	;Pointer basically tells CPU how big table is.
	dw 23				;Size of table (in bytes), minus 1.
	foo equ gdt_table		;Yay for badly named variables!
	dd foo				;Location of table.
