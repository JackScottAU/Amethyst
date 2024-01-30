# Synergy OS Kernel

## Introduction

This is my operating system hobby project.

The goal, as much as there is one, is to produce an operating system vaguely 
similar to Windows 95 (if Windows 95 didn't inherit baggage from DOS and 3.1).
A feature set might include:

* Shell and Scripting Language
* Support for FAT32/ISO9660/EXT2 filesystems on IDE/SCSI/USB devices
* GUI Windowing System w. UI Toolkit
* TCP/IP Networking Stack
* GUI User-Mode Applications:
    * Text Editor
    * Terminal Emulator

The system is currently being developed for x86-based PCs, but additional 
platforms will be supported eventually. Eventual platforms will include:

* 32-bit PCs (Architecture 'x86-32') - primary focus at the moment
* 64-bit PCs (Architecture 'x86-64')
* Raspberry Pi 5 (Architecture 'rpi-5')

This repository contains the kernel and device drivers, and is written in C/C++ with scatterings 
of architecture-specific assembly. THe user-mode applications will be in another repository when the time comes.

## Supported Platforms

### x86-32 Architecture

The 'x86-32' architecture attempts to follow the [PC-97 design standard](https://www.tech-insider.org/windows/research/1997/0711.html) for x86-based IBM-compatible PCs.

* Intel Pentium CPU (or later/equivalent)
* BIOS Boot Services
* 16MiB RAM
* CD-ROM drive
* Keyboard
* PCI Bus
* VGA-compatible Video card and Monitor

Testing is done via Hyper-V, VirtualBox, QEMU, etc. Initial driver efforts will be focused on i440 northbridges and PIIX1/3/4 southbridges.

### Future Platforms

Future work may include support for the following platforms:

 *  **x86-64** - UEFI-based x64 PC
 *  **rpi-5** - Raspberry Pi 5

## Compilation & Installation

### Build System Requirements

The following packages are needed:

* build-essential
* genisoimage
* cpplint
* qemu-system-x86

You will also need cross-compilers for the supported platforms:

* i586-elf-gcc

### Development Cycle

Once the prerequisites are met, you can run the following command to compile
the operating system and launch it in the QEMU emulator:

    make x86_32

### Makefile Options

* **make clean** - Removes all intermediate and final build output. Necessary 
  when switching platforms.
* **make lint** - Runs a code linter and informs you of the thousands of issues.
* **make kernel-x86_32** - Builds the kernel for the x86_32 platform. Useful
  for testing compilation.
* **make image-x86_32** - Builds the final installation image for the x86_32 
  platform.
* **make qemu-x86_32** - Runs the built image in QEMU.

## Further Information

More documentation is in the documentation folder:

* [Devices and Drivers](Documentation/devices-and-drivers.md)

## Contributing

The issue tracker as well as the primary mirror for the git repository are on
[GitHub](https://github.com/JackScottAU/Synergy-OS).
