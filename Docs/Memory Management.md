# Memory Management

## Physical Memory Layout

The first four megabytes (one page table) are always mapped 1:1 into kernel space. 

| Start (P)  | End (P)    | Size     | Usage                                      									   |
| ---------- | ---------- | -------- | ------------------------------------------------------------------------------- |
| 0x00000000 | 0x0007FFFF |  512 KiB | Kernel Essential Data Structures (IDT, GDT, TSS, etc)
| 0x00080000 | 0x000FFFFF |  512 KiB | Hardware Reserved (EBDA, BIOS ROM, VGA ROM, VGA RAM, etc)
| 0x00100000 | 0x003FFFFF |    3 MiB | Kernel Code and Modules, loaded by the Multiboot bootloader

After that, the kernel retrieves the memory map from the bootloader (GRUB) and also from the PCI configuration space to find out what is usable memory and what is reserved.

### Kernel Data Structures

| Start (P)  | End (P)    | Size     | Usage                                      									   |
| ---------- | ---------- | -------- | ------------------------------------------------------------------------------- |
| 0x00002000 | 0x00002071 |   72   B | Task State Segment Structure													   |
| 0x00060000 | 0x0007FFFF |  128 KiB | Physical Memory Allocation Bitmap                                               |

We also want to put the IDT, GDT, etc in here

### Physical Memory Allocation Bitmap

128K is 1024x1024 / 8.

All zeroes = all allocated.

All ones = all free.

## Virtual Memory Layout

0GB to 3GB is user space.
3GB to 4GB is kernel space (and is the same in every process).

| Start (V)  | End (V)    | Size     | Usage                                      									   |
| ---------- | ---------- | -------- | ------------------------------------------------------------------------------- |
| 0x00000000 | 

### Kernel Space

Kernel space is 256MiB from each address space on x86. 0xF0000000 upwards.

-	V0xC0000000 upwards = kernel code and modules and heap

-

Remember each kernel process has it's own space. So we need lots of space for kernel stacks (if we reserved 4MB, that's 1024 threads per process which should be enough).

| Start (V)  | End (V)    | Size     | Usage                                                                           |
| ---------- | ---------- | -------- | ------------------------------------------------------------------------------- |
| 0xF0000000 | 0xF03FFFFF |    4 MiB | Kernel Code + Kernel Core Data Structures                                       |

| 0xF1000000 | 0xFEFFFFFF |  224 MiB | Kernel-Mode Heap Space                                                          |

| 0xFF800000 | 0xFFBFFFFF |    4 MiB | Kernel Stacks (4KiB per Thread)                                                 | < this is the only part of the kernel directory which changes per process
| 0xFFC00000 | 0xFFFFFFFF |    4 MiB | Self-Reference to Page Directory                                                |

### Boot Page Directory

The first page directory loaded during boot maps 0MB to 4MB physical to both:
	0MiB to 4MiB virtual
	3GiB to 3.004GiB virtual

As soon as paging is set up it then removes the identity mapping.