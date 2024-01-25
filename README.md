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

## Minimum System Requirements

### x86-32 Architecture

* Intel 80586 CPU (or later/equivalent) - planning to change to 386 later
* BIOS Boot Services
* 8MiB RAM
* CD-ROM drive (planning to change this to Floppy later)
* Keyboard
* VGA-compatible Video card and Monitor

### x86-64 Architecture

* 64-bit CPU
* 64MB RAM
* UEFI Boot Services
* CD-ROM drive
* Keyboard and monitor

## Compilation & Installation

As long as you have genisoimage and an appropriate GCC cross-compiler installed
(in other words, i586-elf-gcc is available), building the Synergy kernel should
be pretty easy:

    make all

To run Synergy, boot the resulting Synergy-OS.iso CD image on either a physical 
machine or an emulator (Oracle VirtualBox is recommended, but any should work).

## Further Information

More documentation is in the documentation folder:

* [Devices and Drivers](documentation/devices-and-drivers.md)

## Contributing

The issue tracker as well as the primary mirror for the git repository are on
[GitHub](https://github.com/JackScottAU/Synergy-OS).
