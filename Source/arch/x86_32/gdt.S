#This is the code to load my manually built GDT table into the memory.

.global gdt_install
.global gdt_table
.global gdt_pointer

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
	.long 0x00000000 		#Empty descriptor.
	.long 0x00000000

	.long 0x0000FFFF 		#Code descriptor (ring 0). 0GB->4GB.
	.long 0x00CF9A00

	.long 0x0000FFFF 		#Data descriptor (ring 0). 0GB->4GB.
	.long 0x00CF9200

	.long 0x0000FFFF 		#Code descriptor (ring 3). 0GB->4GB.
	.long 0x00CFFA00

	.long 0x0000FFFF 		#Data descriptor (ring 3). 0GB->4GB.
	.long 0x00CFF200

	.long 0x00000000 		#Task State Segment - We Fix this in code later.
	.long 0x00000000

# Pointer we give the CPU to tell it where the GDT table is and how big it is.
.align 32
gdt_pointer:
	.word 47				# Size of GDT table (in bytes), minus 1.
	.long gdt_table			# The location of the GDT table.
