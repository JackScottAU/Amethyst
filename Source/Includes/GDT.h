/**
 *  Amethyst Operating System - Functions interacting with the system GDT table.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_GDT_H_
#define INCLUDES_GDT_H_

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Installs a basic GDT table.
     */

    /// @brief Installs the basic GDT.
    void gdt_install();

    typedef struct {
        unsigned int limit_low              : 16;
        unsigned int base_low               : 24;

        /// @brief For system segments, the type of segment. For code/data segments, holds access bit, read/write bit, conforming bit, and executable bit.
        unsigned int access                 :  4;

        unsigned int code_data_segment      :  1;   // should be 1 for everything but TSS and LDT
        unsigned int DPL                    :  2;   // privilege level
        unsigned int present                :  1;
        unsigned int limit_high             :  4;
        unsigned int available              :  1;   // only used in software; has no effect on hardware
        unsigned int long_mode              :  1;
        unsigned int big                    :  1;   // 32-bit opcodes for code, uint32_t stack for data
        unsigned int gran                   :  1;   // 1 to use 4k page addressing, 0 for byte addressing
        unsigned int base_high              :  8;
    } __attribute__((packed)) gdt_entry;

    /// @brief Information about the Global Descriptor Table, loadable by LGDT.
    typedef struct {
        uint16 size;
        uint32 offset;
    } __attribute__((packed)) gdt_descriptor;

    extern gdt_descriptor gdt_pointer;

    extern gdt_entry gdt_table[6];

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_GDT_H_
