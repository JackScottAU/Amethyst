# Platforms

One of Amethyst's goals is to run on as many different computer hardware platforms as possible.

Want to support both x86 and x64, arm in various forms, plus risc-v once it gets cheap, plus powerpc and 68k for old school kool.

These all have a corresponding #define, and when that define is set via makefile it compiles the kernel for that architecture.

All archs are a seven-letter code. Last two is bits.

| Priority | Platform Code | Description                                                                               |
| -------- | ------------- | ----------------------------------------------------------------------------------------- |
| Tier 1   | `Intel32`     | 32-bit x86 BIOS PC (PC-97)                                                                |
| Tier 1   | `Intel64`     | 64-bit x86 UEFI PC                                                                        |
| Tier 1   | `ArmPi32`     | 1st and 2nd Generation Raspberry Pi Boards                                                |
| Tier 2   | `ArmPi64`     | 3rd and Subsequent Generation Raspberry Pi Boards                                         |
| Tier 2   | `ArmPc64`     | Arm PC Base System Architecture 1.0                                                       |
| Tier 2   | `Power64`     | 5th Generation PowerPC-based Macintoshes                                                  |
| Tier 3   | `Power32`     | 1st to 4th Generation PowerPC-based Macintoshes                                           |
| Tier 3   | `Motor32`     | Motorola 68000-based Macintoshes                                                          |

Other Potential Platforms:

- Loong64
- Riscv64 (BRS defines the system) <https://lists.riscv.org/g/tech-brs/attachment/352/0/riscv-brs-spec.pdf>
- Itanium?!
- Sparc32
- Sparc64
- PDP-11? (16-bit)
- IBM Series/1 (16-bit)
- IBM System/36 (16-bit)
- VAX
- MIPS stuff

## Intel32

Follows the PC-97 Standard. A 586 machine with a PCI bus.

CPU choice: 586 as that supports CPUID and multi-processor and FPU. 686 adds PAE. 686 *might* add MMX, but not necessarily. 486 doesn't have CPUID.

### Booting

Booting is always done via multiboot. This gives us several options:

- Using GRUB on hard disk.
- Using GRUB on CD-ROM.
- Loading via iPXE either using CD-ROM or Network card ROM image.

## Intel64

uefi pc (noting a gap in systems that are 64-bit but not uefi, this is a hole we can afford to lose, it's about 5 years from 2010 to 2015)
