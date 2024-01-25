# Synergy OS Kernel

## Introduction

Synergy is a new operating system with the goal of bringing together all 
computers into a perfect cloud; sharing processing power and data storage, 
whilst still giving each user full control over their individual privacy and 
their data.

The Synergy platform will use a Java-like managed language and runtime to enable
cross-platform support, and to allow the power of all machines to be harnessed 
to their best advantage. A key goal of this language is to make support for 
threading commonplace amongst user-land programs, in order to harness the power 
of emerging multi-core machines.

The system is currently being developed for x86-based PCs, but additional 
platforms will be supported eventually. Support is planned not only for desktop 
and laptop systems, but also for tablets, smart phones, and other embedded 
devices.

## Minimum System Requirements

* Intel Pentium CPU (or later/equivalent)
* 8MiB RAM
* CD-ROM drive
* Keyboard
* VGA-compatible Video card and Monitor

## Compilation & Installation

As long as you have genisoimage and an appropriate GCC cross-compiler installed
(in other words, i586-elf-gcc is available), building the Synergy kernel should
be pretty easy:

    make all

To run Synergy, boot the resulting Synergy-OS.iso CD image on either a physical 
machine or an emulator (Oracle VirtualBox is recommended, but any should work).

## Further Information

The issue tracker as well as the primary mirror for the git repository are on
[GitHub](https://github.com/JackScottAU/Synergy-OS).

