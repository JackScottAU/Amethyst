## Source Tree Layout

* Docs/ - contains documentation.
* Applications/ - Git submodules for user-mode applications.
* Resources/ - third-party code like GRUB etc, and other non-source files for the disk image.
* Source/ - Source code files to build the kernel.
    * arch/ - architecture-dependent stuff
	    * x86_32/ - files for x86
    * kernel/ - general kernel code not dependant on architecture.
	* drivers/ - code for drivers (all drivers except root platform device, even architecture-specific ones).
    * library/ - generic code that will eventually be part of the libc (linked lists, printf(), etc).

## Current Priorities

Milestone 1:
A user program loaded from disk runs in userspace, accepts input from the keyboard, and outputs to a text console.
Needs:
 - IDE driver
 - Filesystem drivers
 - VFS
 - Multitasking
 - Ring 3
 - Basic system calls

Milestone 2:
Multiple instances of that program can be run simultaneously in terminal emulators in a graphical gui

1. Serial communication. - done enough to work. Needs tidying up when we make full drivers.
2. Interactive shell.
4. Device tree.
3. ANSI Terminal Support for keyboard and VGA.
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

Amethyst will have a command/scripting language like Bash/Powershell.

## Incoherent Ramblings

I want it to be a platform for testing concepts

Ignore POSIX

User interface is important


VGA console is ansi terminal sequences compliant
Keyboard is ansi terminal as well?

Keyboard has two modes: ansi mode (with optional echoing) and scancode mode where it passes raw make/break codes.

Or migrate to serial port? Ansi in and out by default then.

Make an interface in C, one function to write a ansi char, another function to read an ansi char (blocking), then can implement a class that can be a terminal with read/write in ansi. Then can choose terminal on the fly. Maybe have grub options to do serial?

Need to revisit paging and see if that would be useful

Multitasking would be cool.

As would a shell... Which should talk to a terminal...

Use shell to ask questions and get info.
Use PowerShell like commands: Get-DeviceTree --Verbose

Then start building up a device tree.

We can get started on a shell quicker if we do serial.

Current Priorities:
1. Serial Port - done enough to move on
2. Interactive Shell - needs string functions! Tolower() and split() mostly
3. Link against libgcc
4. Device Tree
5. Higher half kernel. (Half of paging)
6. Generic linked list and tree structures 
More paging
7. Multitasking
8. Client apps
9. Device detector for IDE, floppy, etc
10. Driver for IDE, driver for file system fat32
11. Graphics driver

Bus devices (including platform device) know about all their possible children and can ask them to detect and init. Some (pci, USB) have registration so child devices can add themselves to the list?



Step 1: Platform Core Initialisation (CPU and Memory)
Step 2: Kernel Core Initialisation (Scheduler, etc)
Step 3: Platform Device Initialisation
Step 4: Open for user sessions. Initialise services.



User mode drivers
I/O request packets
Message passing
    Ports
    Fixed message size for simplicity, plus attached page of memory if needed
    Can share memory pages if needed
Microkernel 

## Config

Config object model (like browser's document object model). 
Base config set in header files or something, baked in to kernel.
Config loaded from disk in xml format (hard to parse, but fully capable of s-expressions so can handle any arbitrary config) into COM.

# Platforms

Each potential platform has a pair identifying it:
First part is CPU architecture, second part is the machine type (and built in assumptions about booting and root device)
    x86-pc
    x64-pc
    arm-pi5

# Time

Unix time isn't worth following, suffers from many issues with leap seconds and also the epoch not making any sense due to UTC not existing in 1970.

An epoch of 2000-01-01 would make sense?

How to store values? In ticks of 65536ths of a second. That way 64 bits can hold +/- 4 million years of ticks.

Could also have a high-precision variant that is a 256-bit number.

Big ticks are 1/2^16
Small ticks are 1/2^128

Class LowPrecisionTime = 64 bits: 48 bits for seconds and 16 bits for ticks - enough for modern cpu and system clock
Class HighPrecisionTime = 256 bits: 128 bits for seconds and 128 bits for ticks - provided to be useful for ultra-performance work

Can convert between the two by using sign extension etc.

128 bits for ticks (roughly 10^39) is more than enough to store at less than a quectosecond, and very near to planck time.
128 bits for seconds is enough to store the age of the universe, and probably the end of the universe.

256 bits of ticks that are 1/2^128th of a second long is enough to store the entire duration of the existence of the universe from start to end, in precision down to the quectosecond level. This is all that could possibly ever be needed.

                        /--------------------------------|--------------------------------\
High-Precision Time     |               128              |              128               |
                        \--------------------------------|--------------------------------/

                                            /------------|----\
Low Precision Time                          |     48     | 16 |
                                            \------------|----/

High-precision time exists only in userspace, the kernel doesn't worry about it?

## DateTime

A date/time is a combination of an instant, a calendar, and a timezone/location, as well as a fuzziness for how accurate the instant is (in number of dropped bits at the beginning). It allows human understanding of an instant.
