# Notes

## TODOs

Currently working on GUI events, ring 3, IDE.

### Micro Kernel (C only)

- Clean up debugging messages
- Clean up kernel file
- Tidy up current multitasking, GDT and interrupt code.
- Keep going with page management.
- Make the scheduler a bit smarter
- Refactor scheduler into thread.c (arch) and scheduler.c (generic)
- Seperate kernel clock mechanism from PIT.
- Rename "portIO" to "hal"

### Drivers

- Refactor device drivers into a nice C++ class sort of thing.
- Sort out the mess that is the PCI IDE driver (turn things into functions and DEFINEs)
- Split the IDE driver into three parts: PCI detection for two channels, then management of a channel and it's two drives, and then disk block access
- Make the text console work with colours when scrolling (and faster)
- Fix detection in ATA driver.
- Rename QEMU VGA driver to something more generic and make the files nicer.
- Refactor "vga_framebuffer" into canvas
- Add 1280x720 mode to ATI driver for widescreens

#### Driver Priorities

While we work on core microkernel stuff, we should also be working on drivers:

1. QEMU/Bochs Display Adapter (enough to set a mode and get a framebuffer)
2. IDE
3. Mouse

### User Space

- Turn canvas into a class
- Finish the windowing toolkit
- Fix the memory leak in stream_readLine() caused by multitasking.
- Make the shell better
- Improve performance of GUI by marking widgets as dirty on event processing, they clean themselves when Redraw() is called. Then only redraw dirty things on mouse move. Something like that.

## Source Tree Layout

- Docs/ - contains documentation.
- Applications/ - Git submodules for user-mode applications.
- Resources/ - third-party code like GRUB etc, and other non-source files for the disk image.
- Source/ - Source code files to build the kernel.
  - arch/ - architecture-dependent stuff
    - x86_32/ - files for x86
  - kernel/ - general kernel code not dependant on architecture.
  - drivers/ - code for drivers (all drivers except root platform device, even architecture-specific ones).
  - library/ - generic code that will eventually be part of the libc (linked lists, printf(), etc).

## Debug in StandardIO

Add a logging channel to StandardIO. Everything that uses StandardIO can be given a default debugging channel and can set it's own debugging channels.

And the kernel pre-StandardIO can use the debug() method directly, which will be wired directly to the qemu port.

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

## Ring 3

For ring 3:

Compile a simple program mapped to 0x000 instead of higher memory.

Load that via kernel module.

TSS esp0 can be set to highest value of kernel stack page. Do this when creating task.

Jump to entry point of our elf module

Our task switching code will need to save and load the tss esp stack top to the thread control block.

Have an interrupt to call

## VFS

file://CD0/

HD1 = hard disk logical partition 1
HD2 = hd1, p

FD1 = A:
FD2 = B:

NET? no these are something other than file://

## Startup Process

1. Memory Management:
        GDT
        Paging
2. Interrupts
3. Multitasking
4. Basic Devices
5. Everything Else

## Multitasking

Thread.h contains stuff for creating, destroying and switching threads, and is processor-specific.

Scheduler.h contains the main scheduler, and can have threads added and removed, and is processor agnostic.

## Video Cards and Drivers

### General Principles

-  Card-specific video drivers should support double buffering, 2D blitting, and a hardware cursor. Where not supported
   in hardware, these functions should be implemented in software.
-  Card-specific video drivers must not use BIOS-level functionality (such as EFI or VBE). Raw register programming is 
   preferred even when development time would be much slower.
-  The names of video drivers should be `gfx-{vendor}-{model}`. Where the targetted device is a virtual-only device 
   (such as the Bochs display adapter), or a common standard (such as VGA), the vendor should be `virtual`.
-  The kernel does not require a video output to function, so no support for video is required from our bootloader.
-  The root platform device for each platform is responsible for prioritising display drivers correctly. For example,
   the ATI Rage 128 could be supported by either the `gfx-ati-r128` or the `gfx-virtual-vga` drivers, logically the
   device-specific driver should be prioritised. For any platform supporting PCI, this simply means matching drivers 
   based first on their vendor and device IDs before matching on class and subclass.

### Colour Depths

Internally the system will use either ARGB32 or ARGB64 for pixel data. 

It may be quantised for output within the video driver.

### Driver Development Goals and Timeline

Aim to develop full-featured drivers for the following cards:

0. Multiboot framebuffer (`gfx-virtual-multiboot`)
1. QEMU/Bochs Display Adapter (`gfx-virtual-bochs`) - Note this device only provides a raw framebuffer.
2. Cirrus Logic 5400 Series (`gfx-cirrus-5400`)
22. Virtio GPU (`gfx-virtual-virtio`)
3. ATI/AMD Rage 128 (`gfx-ati-r128`)
4. Intel Graphics (`gfx-intel-xxx`) - Will require multiple drivers for the entire range I suspect?
5. VGA (`gfx-virtual-vga`) - the ultimate fallback.

The reasons for choosing these devices are:

-  Each device supports something better than 800x600x16.
-  Each device is testable either virtually or physically (or ideally both).
-  Each device can provide double buffering which is a huge improvement.

### Cirrus Logic 5400 Series

Have not even started this driver yet - probably needs to take precedence over the ATI card as I think we could actually do something with it.

Device supports:
-  Double buffering
-  Blitting
-  Hardware cursor

???

One tricky bit is that this device uses additional resources, including the VGA area at 0xA0000 and 0xB8000 etc. The blit engine is kept at 0xB8000 etc. in revision A.

https://theretroweb.com/chip/documentation/cl-gd5446technical-6456556f945a1573066314.pdf

### ATI Rage 128 (`gfx-ati-r128`)

There isn't really a huge benefit to writing this driver: the QEMU emulation of it is very bad and doesn't support mouse pointers or 2D acceleration. The only way to make this worthwhile is to actually work on improving the QEMU driver (which is currently orphaned).

Although it does support better resolutions than the Cirrus card, which would be the main claim to fame.

Conceptually similar to the early Radeons. R100 and R200 series would feel vaguely familiar.

R300 onwards is basically an entirely different GPU architecture.

#### PLL Clock

reference clock is the crystal on the board. Mine says "29.4-0210", I suspect this is a 29.5MHz crystal like the linux driver says it could be. 29.5 times 4 is 118 so this makes perfect sense.

Can get reference clock info from the BIOS on the board.

29.5 times 11 divide 5 is 64.9 which is very close to the number we need. Equals refresh rate of 59.9Hz which is something the monitor can do?

We need to set memory parameters before we can display a picture.
Need to dump PLL registers via serial so we can confirm PLL settings and get reference clock.
Then we can rebuild PLL settings from that, for the display mode we choose.
And based on the pixel clock, we can determine the memory clock? Getting reference clock is the key.

#### Programming
- Get hardware parameters from BIOS.
- Compute mode parameters and PLL parameters.
- Disable VGA CCComptability, Display output, etcc.
- Set VCLK somehow.
- Set CRTC.
- Enable things again.

## Executables

Use 32-bit elf binaries.

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

## Makefile Targets

- make ARCH-cd-qemu < default
- make ARCH-hd-qemu
- make ARCH-cd
- make ARCH-hd
- make ARCH
- make clean
- make lint

## Logging and Debugging

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

## Plan

The kernel has a function, debug() which takes a log level, a format string, and an arguments list. It outputs a formatted string to debug_loggingDevice, which is a function that takes a single character and puts it to the debug device. This debug device for boot startup is set to QEMU's debug device (port E9).

There is also a function, debug_setLoggingDevice() which sets the value of the debug_loggingDevice function, so that once more services are available at runtime a better logging store can be used.

Similar with debug_loggingLevel and debug_setLoggingLevel().

## Time

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

### DateTime

A date/time is a combination of an instant, a calendar, and a timezone/location, as well as a fuzziness for how accurate the instant is (in number of dropped bits at the beginning). It allows human understanding of an instant.
