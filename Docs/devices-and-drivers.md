A 'device' is a node in a tree that can either implement a class of device (video, sound, input, network, file, etc) or have child devices (such as be a PCI bus).

## Sample tree:

    x86 PC (platform driver)
     +- PCI Bus (bus driver)
     |   +- Video Card (bus driver, class driver for video)
     |   |   +- Monitor #1 (class driver for monitor)
     |   |   +- Monitor #2 (class driver for monitor)
     |   +- Sound Card (class driver for sound)
     |   +- SATA Interface
     |   |   +- Hard Drive (class driver for raw storage)
     +- Programmable Interrupt Timer (class driver for clock)
     +- PS/2 Controller (bus driver)
     |   +- PS/2 Keyboard (class driver for input)
     |   +- PS/2 Mouse (class driver for input)
     +- PCI Root Hub - i440FX Northbridge (0:0:0)
     |   +- PCI Bridge - PIIX3 Southbridge (0:1:0) // if slots have multiple functions, first one is a tree parent.
     |   |   +- ISA Controller (0:1:1)
     |   |   +- PCI Bridge (0:1:2) // i think this is the USB controller.
     |   |   +- ISA Controller (0:1:3)

## Driver Ideas

- Don't bother implementing ISA DMA - just use port I/O for everything non-PCI. Since we are targeting 686 and above, we can assume that most devices that need fastness will be on PCI.

## Resources

Every item in the tree can have resources attached to it. Resources are of type I/O, memory, or IRQ.

## Bus Devices & Drivers

### Platform Devices

Each platform has one root platform device, such as "x86-64 PC", and that root device is a bus driver which finds and initialises other bus and device drivers. That way SoC can be accomodated. On x86 it would do something like try to initialise ACPI, and if that fails, try to manually init the PCI bus.

The platform driver lives in the architecture-specific folder. All other devices live in the devices folder. A platform driver is simply a bus driver.

## Node Devices & Drivers

## Filter Devices

For example, turns scancodes into ascii keys.

## Device Classes
