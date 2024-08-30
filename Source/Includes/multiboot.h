/**
 *  Amethyst Operating System - Multiboot v1 Information Structure.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_MULTIBOOT_H_
#define INCLUDES_MULTIBOOT_H_

#include <Types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MULTIBOOT_MAGIC_NUMBER  0x2BADB002

typedef enum : uint8 {
    IndexedColour = 0,
    DirectColour = 1,
    EGA = 2,
} multiboot_framebufferType;

struct multiboot_info {
    uint32 flags;
    uint32 memoryLower;
    uint32 memoryUpper;
    uint32 bootDevice;
    char* commandLine;
    uint32 modsCount;
    struct multiboot_moduleNode* modsAddr;
    uint32 syms1;
    uint32 syms2;
    uint32 syms3;
    uint32 syms4;
    uint32 memoryMapLength;
    struct multiboot_memoryMapNode* memoryMapAddress;

    uint32 drives_length;
    uint32 drives_addr;

    uint32 config_table;

    uint32 boot_loader_name;

    uint32 apm_table;

    uint32 vbe_control_info;
    uint32 vbe_mode_info;
    uint16 vbe_mode;
    uint16 vbe_interface_seg;
    uint16 vbe_interface_off;
    uint16 vbe_interface_len;

    void* framebuffer_addr;
    uint32 framebuffer_addr_upper;
    uint32 framebuffer_pitch;
    uint32 framebuffer_width;
    uint32 framebuffer_height;
    uint8 framebuffer_bpp;

    multiboot_framebufferType framebuffer_type;
    //uint8 framebuffer_type;     // 0 = indexed colour, 1 = direct colour, 2 = ega text

    uint8 framebuffer_red_field_position;
    uint8 framebuffer_red_mask_size;
    uint8 framebuffer_green_field_position;
    uint8 framebuffer_green_mask_size;
    uint8 framebuffer_blue_field_position;
    uint8 framebuffer_blue_mask_size;
};

/// @brief Types of memory, provided by GRUB.
typedef enum : uint32 {
    Available = 1,
    Reserved = 2,
    ACPI = 3,

    /// @brief reserved memory which needs to be preserved on hibernation
    Hibernation = 4,

    /// @brief memory which is occupied by defective RAM modules
    Defective = 5,
} multiboot_memoryType;

struct multiboot_memoryMapNode {
    /// @brief Size of this memory map node entry. Nothing to do with the actual memory itself.
    uint32 size;

    /// @brief Start address of the memory.
    uint64 addr;

    /// @brief Length of the memory.
    uint64 len;

    /// @brief Type of the memory.
    multiboot_memoryType type;
} __attribute__((packed));

struct multiboot_moduleNode {
    void* start;
    void* end;
    char* fileName;
    uint32 reserved;
} __attribute__((packed));

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_MULTIBOOT_H_

