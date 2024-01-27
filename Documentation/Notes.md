## Source Tree Layout

* Documentation/ - contains docs
* Modules/ - Source code files for userland code.
* Resources/ - third-party code like GRUB etc.
* Source/ - Source code files to build the kernel.
    * arch/ - architecture-dependent stuff
	    * x86_32/ - files for x86
    * kernel/ - general kernel code not dependant on architecture.
	* drivers/ - code for drivers (all drivers except root platform device, even architecture-specific ones).

## Current Priorities

1. Serial communication. - done enough to work. Needs tidying up when we make full drivers.
2. Interactive shell.
4. Device tree.
3. ANSI Terminal Support for keyboard and VGA.
4. Higher half kernel.
5. Expanding string and linked list/tree support functions.
6. Multitasking
7. Drivers for storage

## Other Notes

Single address space - everything runs as a bytecode on top of kernel mode.
So just need a simple GDT. Physical memory manager and virtual memory manager need to be very good and very fast.

Physical Memory Manager --> Virtual Memory Manager --> Object Memory Manager (GC) --> Runtime Interpreter <-- GUI Shell <-- Video/Console/etc Drivers
				^					^			^
			Paging Memory Manager			Class/Object Loader & Saver	^
				^					^		^	^
			Disk/FS Drivers		----->		VFS Drivers		Network Drivers
			
			
File system:
	Stores three things: code objects, code classes, data objects
	Data objects are kept in a heirachy by user, then however they want
	Code objects are kept in a DHT.
	Code classes are kept in a global tree.
	Goals:
		Distributed, but a copy of user's data is always kept on their machine.
		User's data locked by OpenPGP or somesuch.
		Full revision control over data and code objects, if desired.
		Something ZFS-like.

--------------------------------------------------------------------------------

One of the biggest decisions is what should go in kernel space, and what should go in user space:

Reasons for userspace:
 - Not in the kernel tree, which makes it easier to write new kernels.
 - Copied across to new nodes as they need them.

Reasons for kernelspace:
 - Needed during boot?
 - Speed.

Things to go in kernel space:
 - Network drivers
 - GUI drivers
 - Other drivers

Other option: use signed userspace drivers.
Everything possible goes in userspace.
When porting the kernel, only the support kernel and the runtime need to be ported.
As long as the definition of the runtime objects and the language stay the same, no code need be ported.

synergy.driver.* tree to be used for drivers.
synergy.library.* to be used for supprort libraries.
synergy.gui.* to be used for the gui.
network.* to be used for public network interface.
graphics.* to etc...
file.*
input.*
sound.*
library.* for stdlib-style stuff.

Three different languages:

  * Scripting Language - plugs in to object model used by operating system
  * Programming Language - for developing compiled/jit programs in
  * CLI language - for one line interactions