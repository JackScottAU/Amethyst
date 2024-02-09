.global loader                          # making entry point visible to linker

# setting up the Multiboot header - see GRUB docs for details
.set ALIGN,    	1<<0                     # align loaded modules on page boundaries
.set MEMINFO,  	1<<1                     # provide memory map
.set VIDEO,		1<<2					 # set graphics
.set FLAGS,    	ALIGN | MEMINFO		  	# this is the Multiboot 'flag' field
.set MAGIC,    	0x1BADB002               # 'magic number' lets bootloader find the header
.set CHECKSUM, 	-(MAGIC + FLAGS)         # checksum required

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0	#graphics modules
.long 1024
.long 768
.long 32

# reserve initial kernel stack space
.set STACKSIZE, 0x4000                  # that is, 16k.
.comm stack, STACKSIZE, 32              # reserve 16k stack on a quadword boundary

#Our Multiboot loader will start execution at this point.
loader:
	#Set up the kernel's stack.
	mov $(stack + STACKSIZE), %esp		#Move the address of the top of the stack into ESP.

	#Start the main kernel.
	push %ebx				#Push the Multiboot data structure.
	push %eax				#Push the Multiboot magic number.
	call kernel_initialise			#Call the main kernel initialisation function.
	
hang:
	#The kernel should not reach this point. If it does, hang the machine.
	cli					#Disable interrupts.
	hlt					#Halt CPU.
	jmp hang				#We *definitely* shouldn't get here, but if we do, go back to trying to hang again.
