#This is the code to load my manually built GDT table into the memory.

.global gdt_install
.global gdt_pointer
.global gdt_table

gdt_install:     #Installs a new GDT and reloads segment registers.
	lgdt gdt_pointer
        movw $0x10,%ax
        movw %ax,%ds
        movw %ax,%es
        movw %ax,%fs
        movw %ax,%gs
        movw %ax,%ss
        ljmp $0x08, $gdtFlush
gdtFlush:
        ret

.align 32
gdt_table:       #The actual GDT table.
        .long 0x00000000 #Empty descriptor.
        .long 0x00000000

        .long 0x0000FFFF #Code descriptor (ring 0). 0GB->4GB.
        .long 0x00CF9A00

        .long 0x0000FFFF #Data descriptor (ring 0). 0GB->4GB.
        .long 0x00CF9200

.align 32
gdt_pointer:     #Pointer basically tells CPU how big table is.
        .word 23                        #Size of table (in bytes), minus 1.
        .equ foo, gdt_table             #Yay for badly named variables!
	.long foo
