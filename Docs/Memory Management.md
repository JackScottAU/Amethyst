# Memory Management

## Physical Memory Layout

The first four megabytes (one page table) are always mapped into kernel space. 

## Virtual Memory Layout

0GB to 3GB is user space.
3GB to 4GB is kernel space (and is the same in every process).

### Boot Page Directory

The first page directory loaded during boot maps 0MB to 4MB physical to both:
	0MB to 4MB virtual
	3GB to 3.004GB virtual




## Old Notes

IGNORE EVERYTHING BELOW - IT'S OUT OF DATE.

Amethyst uses amazingly simple memory management. Everything uses physical 
addresses, and paging is not enabled at all, due to the single-address-space 
requirements.

## Memory Manager Initialisation

1.	The kernel is given a list of free memory regions and spaces used by the 
	kernel and modules by the Multiboot-compliant loader (such as GRUB).
2.	All free memory regions (free spaces - module spaces - kernel space) are
	added to the *free* list.

## Memory Manager Operation

### Memory Allocation

To allocate a block of memory, the kernel calls allocate() and the amount needed
is taken from the front of the free list, a header is added to the region with
the amount that was allocated and away we go.

### Memory Deallocation

free() takes the pointer to the region, finds the header, reads how much was 
allocated, and puts that back on the free list.

### Joining Free Memory Regions

When the CPU is quiet, or when there are no free memory blocks of the required 
size, the kernel will go through and join together free memory blocks to make
bigger ones.

