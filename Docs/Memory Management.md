# Memory Management

## Physical Memory Layout

The first four megabytes (one page table) are always mapped into kernel space. 

| Start      | End        | Size    | Usage                                       |
| ---------- | ---------- | ------- | -----
| 0x00000000 | 0x0007FFFF | 512 KiB | Kernel Essential Data Structures (IDT, GDT, TSS, etc)
| 0x00080000 | 0x000FFFFF | 512 KiB | Hardware Reserved (BIOS ROM, VGA ROM, VGA RAM, etc)
| 0x00100000 | 0x003FFFFF | 3 MiB   | Kernel Code and Modules, loaded by the Multiboot bootloader

After that, the kernel retrieves the memory map from the bootloader (GRUB) and also from the PCI configuration space to find out what is usable memory and what is reserved.

## Virtual Memory Layout

0GB to 3GB is user space.
3GB to 4GB is kernel space (and is the same in every process).

### Boot Page Directory

The first page directory loaded during boot maps 0MB to 4MB physical to both:
	0MB to 4MB virtual
	3GB to 3.004GB virtual
