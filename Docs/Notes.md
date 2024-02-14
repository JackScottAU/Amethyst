## TODO

Immediate TODO list:

1.  Keyboard driver:
    3.  stream_readLine() - still needs to handle backspace at least.
    4.  Finish PS/2 Controller initialisation routine (device detection and child init, device tree building, etc)
2.  Expand string and data structures functionality and put it in the library/ folder:
    1.  Linked list
    2.  string_toLower() etc
    3.  string_format() (sprintf)
3.  Fix PCI device bus enumeration so it's nice and pretty.
4.  Enable paging.

## Source Tree Layout

* Docs/ - contains documentation.
* Applications/ - Git submodules for user-mode applications.
* Resources/ - third-party code like GRUB etc.
* Source/ - Source code files to build the kernel.
    * arch/ - architecture-dependent stuff
	    * x86_32/ - files for x86
    * kernel/ - general kernel code not dependant on architecture.
	* drivers/ - code for drivers (all drivers except root platform device, even architecture-specific ones).
    * library/ - generic code that will eventually be part of the libc (linked lists, printf(), etc).

## Current Priorities

Milestone 1:
A user program loaded from disk runs in userspace, accepts input from the keyboard, and outputs to a text console.

Milestone 2:
Multiple instances of that program can be run simultaneously in terminal emulators in a graphical gui

1. Serial communication. - done enough to work. Needs tidying up when we make full drivers.
2. Interactive shell.
4. Device tree.
3. ANSI Terminal Support for keyboard and VGA.
4. Higher half kernel.
5. Expanding string and linked list/tree support functions.
6. Multitasking
7. Drivers for storage

## Booting - x86_32

Booting is always done via multiboot. This gives us several options:
 *  Using GRUB on hard disk.
 *  Using GRUB on CD-ROM.
 *  Loading via iPXE either using CD-ROM or Network card ROM image.

## Executables

Use 32-bit elf binaries.

## Multitasking

Use kernel threading in 1:N model (kernel knows about all threads).

Process:	Code / Data / Heap
Thread:		Registers / Stack

## Shell Language

Synergy will have a command/scripting language like Bash/Powershell.
