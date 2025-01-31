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

 - Tidy up current multitasking, GDT and interrupt code.
 - Write a page frame allocator

## Milestones (move this to Github)

Milestone 1:
A user program loaded from disk runs in userspace, accepts input from the keyboard, and outputs to a text console.
Needs:
 - IDE driver
 - Filesystem drivers
 - VFS
 - Multitasking
 - Ring 3
 - Basic system calls
 - Executable loading from file

Milestone 2:
Multiple instances of that program can be run simultaneously in terminal emulators in a graphical gui

1. Serial communication. - done enough to work. Needs tidying up when we make full drivers.
2. Interactive shell.
4. Device tree.
3. ANSI Terminal Support for keyboard and VGA.
5. Expanding string and linked list/tree support functions.
6. Multitasking
7. Drivers for storage

## VFS

file://CD0/

HD1 = hard disk logical partition 1
HD2 = hd1, p

FD1 = A:
FD2 = B:

NET? no these are something other than file://

## Startup Process

1.  Memory Management:
        GDT
        Paging
2.  Interrupts
3.  Multitasking
4.  Basic Devices
5.  Everything Else

## Multitasking

Thread.h contains stuff for creating, destroying and switching threads, and is processor-specific.

Scheduler.h contains the main scheduler, and can have threads added and removed, and is processor agnostic.

## Booting - x86_32

Booting is always done via multiboot. This gives us several options:
 *  Using GRUB on hard disk.
 *  Using GRUB on CD-ROM.
 *  Loading via iPXE either using CD-ROM or Network card ROM image.

## Video Cards

We aren't going to use EFI or VBE to set video modes.

We'll stick with EGA text mode for now. Later on, steal ideas from the Linux kernel in order to create 2D framebuffer drivers. Start with VGA, then work up from there.

This means we can avoid needs virtual 8086 mode and can stick entirely to protected mode.

## Executables

Use 32-bit elf binaries.

## Multitasking

Use kernel threading in 1:N model (kernel knows about all threads).

Session:    Console, Security
Process:	Code / Data / Heap (Virtual Address Space - CR3), File Pointers, working directory etc
Thread:		Registers / Stack (Stack Top)
Fibres:     User-mode co-operative thread. Invisible to the kernel, but implemented in the standard library.

## Shell Language

Amethyst will have a command/scripting language like Bash/Powershell.

Call the shell language Gem, allow scripts with extension .gem?

Or oyster (oyster shell) - file extension .oys(ter)

Or do we do something like lisp?

syntax

then semantics


syntax = like tcl or powershell?

basically, everything is an expression.
expressions are in prefix notation

take ideas from TCL

everything is either a literal, a variable or a function, there are no operators (or operators are disguised functions)

a variable can contain either a literal or a function call

a literal can be either a string or a number (in one of many formats)

everything is func(arg, arg, arg)


    if(arg, do(expr1, expr2, expr3))

    assign(varname, "x") <- assigns the expression to the variable without executing it yet, allows defining functions

    import("filename") reads in a script file and executes it

without the (), something is a variable. The () - at the top level makes it go.

idea: have both commas and semicolons be usable, so you can do this to create blocks (and allow ending ; or , before closing brackets):

    if(arg, do(
        expr1;
        expr2; 
        expr3;
    ))

have namespaces:

    thread:new(arg1, arg2)

"statements":

    if(cond, block)
    do(block, ...)
    assign(var, block)
    import(file)
    while(cond, block)

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
Also can be passed in on multiboot command line.

# Platforms

Each potential platform has a pair identifying it:
First part is CPU architecture, second part is the machine type (and built in assumptions about booting and root device)

| Platform Name | CPU Architecture  | Machine Type                                              |
| ------------- | ----------------- | --------------------------------------------------------- |
| X86_BIOS      | x86 32-bit        | IBM Compatible PC BIOS - Booting with Multiboot v1        |
| X64_UEFI      | x86 64-bit        | IBM PC - UEFI Booting w. ACPI                             |
| ARM_RPI5      | Arm 64-bit        | Raspberry Pi 5                                            |


A = arm, I = intel, S = sparc, R = riscv, P = powerpc, M = 68k, W = WDC 6502?

These all have a corresponding #define, and when that define is set via makefile it compiles the kernel for that architecture.

# Logging and Debugging

There are several debugging options available to us:
 - QEMU Debug Device (0xe9)
 - Serial Port
 - EGA text console
 - File logging
 - GDB via QEMU

What we do is:
 - Have a debug logger that exports to port E9 until we have everything set up, and can then log to file

What we currently do is:
 - Send ANSI to serial

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
