# Synergy OS Kernel

## Introduction

This is my operating system hobby project.

The system is currently being developed for x86-based PCs, but additional 
platforms will be supported eventually. Eventual platforms will include:

* 32-bit PCs (Architecture 'x86-32') - primary focus at the moment
* 64-bit PCs (Architecture 'x86-64')
* Raspberry Pi 5 (Architecture 'rpi-5')

This repository contains the kernel, and is written in C/C++ with scatterings 
of architecture-specific assembly.

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

Testing is done via Hyper-V, VirtualBox, QEMU, etc.

### Future Platforms

Future work may include support for the following platforms:

 *  **x86-64** - UEFI-based x64 PC
 *  **rpi-5** - Raspberry Pi 5

## Compilation & Installation

As long as you have genisoimage and an appropriate GCC cross-compiler installed
(in other words, i586-elf-gcc is available), building the Synergy kernel should
be pretty easy:

    make all

To run Synergy, boot the resulting Synergy-OS.iso CD image on either a physical 
machine or an emulator (Oracle VirtualBox is recommended, but any should work).

## Further Information

More documentation is in the documentation folder:

* [Devices and Drivers](Documentation/devices-and-drivers.md)

## Contributing

The issue tracker as well as the primary mirror for the git repository are on
[GitHub](https://github.com/JackScottAU/Synergy-OS).
