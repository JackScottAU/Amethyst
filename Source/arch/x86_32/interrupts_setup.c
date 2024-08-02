/**
 *  Amethyst Operating System - Interrupts setup.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <portIO.h>
#include <Types.h>
#include <interrupts.h>


struct IDT_Entry_S {
    uint16 Address_Low;
    uint16 Selector;        // GDT selector (which code segment)
    uint8 Empty;
    uint8 Flags;
    uint16 Address_High;
} __attribute__((packed));

struct IDT_Pointer_S {
    uint16 Limit;
    uint32 Base;
} __attribute__((packed));


struct IDT_Entry_S IDT_Table[0x40];
struct IDT_Pointer_S IDT_Pointer;

void interrupts_disableInterrupts(void) {
    __asm__ volatile("cli");
}

void interrupts_enableInterrupts(void) {
    __asm__ volatile("sti");
}

void interrupts_initialise(void) {
    interrupts_installEmptyIDT();
    interrupts_installISRs();        //} These two could be done in any order.
    interrupts_remap8259s(0x20);        //}

    interrupts_enableInterrupts();
}

void interrupts_installEmptyIDT() {
    /*
    SYNOPSIS:    Clears the IDT, and installs it ready to use.
    INPUTS:        None.
    OUTPUT:        None.
    NOTES:        None.
    */

    unsigned char *Temp;
    unsigned int Count = 0;

    // Create the pointer.
    IDT_Pointer.Limit = (sizeof (struct IDT_Entry_S) * 0x40) - 1;
    IDT_Pointer.Base = (unsigned int) &IDT_Table;

    // Empty the IDT. THis is a hack of a code.
    Temp = (void*) &IDT_Table;
    while (Count < (sizeof(struct IDT_Entry_S) * 0x40)) {
        *Temp = 0x00;
        Temp++;
        Count++;
    }

    // Do the install.
    __asm__ __volatile__("lidt %0"::"m"(IDT_Pointer));
}

void interrupts_installISRs() {
    interrupts_setGate(0x00, (unsigned)interrupts_ISR_00, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x01, (unsigned)interrupts_ISR_01, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x02, (unsigned)interrupts_ISR_02, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x03, (unsigned)interrupts_ISR_03, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x04, (unsigned)interrupts_ISR_04, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x05, (unsigned)interrupts_ISR_05, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x06, (unsigned)interrupts_ISR_06, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x07, (unsigned)interrupts_ISR_07, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x08, (unsigned)interrupts_ISR_08, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x09, (unsigned)interrupts_ISR_09, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x0A, (unsigned)interrupts_ISR_0A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x0B, (unsigned)interrupts_ISR_0B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x0C, (unsigned)interrupts_ISR_0C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x0D, (unsigned)interrupts_ISR_0D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x0E, (unsigned)interrupts_ISR_0E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x0F, (unsigned)interrupts_ISR_0F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x10, (unsigned)interrupts_ISR_10, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x11, (unsigned)interrupts_ISR_11, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x12, (unsigned)interrupts_ISR_12, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x13, (unsigned)interrupts_ISR_13, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x14, (unsigned)interrupts_ISR_14, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x15, (unsigned)interrupts_ISR_15, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x16, (unsigned)interrupts_ISR_16, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x17, (unsigned)interrupts_ISR_17, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x18, (unsigned)interrupts_ISR_18, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x19, (unsigned)interrupts_ISR_19, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x1A, (unsigned)interrupts_ISR_1A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x1B, (unsigned)interrupts_ISR_1B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x1C, (unsigned)interrupts_ISR_1C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x1D, (unsigned)interrupts_ISR_1D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x1E, (unsigned)interrupts_ISR_1E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x1F, (unsigned)interrupts_ISR_1F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x20, (unsigned)interrupts_ISR_20, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x21, (unsigned)interrupts_ISR_21, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x22, (unsigned)interrupts_ISR_22, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x23, (unsigned)interrupts_ISR_23, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x24, (unsigned)interrupts_ISR_24, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x25, (unsigned)interrupts_ISR_25, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x26, (unsigned)interrupts_ISR_26, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x27, (unsigned)interrupts_ISR_27, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x28, (unsigned)interrupts_ISR_28, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x29, (unsigned)interrupts_ISR_29, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x2A, (unsigned)interrupts_ISR_2A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x2B, (unsigned)interrupts_ISR_2B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x2C, (unsigned)interrupts_ISR_2C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x2D, (unsigned)interrupts_ISR_2D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x2E, (unsigned)interrupts_ISR_2E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x2F, (unsigned)interrupts_ISR_2F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x30, (unsigned)interrupts_ISR_30, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x31, (unsigned)interrupts_ISR_31, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x32, (unsigned)interrupts_ISR_32, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x33, (unsigned)interrupts_ISR_33, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x34, (unsigned)interrupts_ISR_34, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x35, (unsigned)interrupts_ISR_35, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x36, (unsigned)interrupts_ISR_36, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x37, (unsigned)interrupts_ISR_37, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x38, (unsigned)interrupts_ISR_38, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x39, (unsigned)interrupts_ISR_39, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x3A, (unsigned)interrupts_ISR_3A, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x3B, (unsigned)interrupts_ISR_3B, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x3C, (unsigned)interrupts_ISR_3C, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x3D, (unsigned)interrupts_ISR_3D, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x3E, (unsigned)interrupts_ISR_3E, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
    interrupts_setGate(0x3F, (unsigned)interrupts_ISR_3F, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTGATE32);
}

void interrupts_remap8259s(uint8 baseIRQ) {
    portIO_write8(IDT_PIC1_BASEADDRESS, 0x11);
    portIO_write8(IDT_PIC2_BASEADDRESS, 0x11);

    portIO_write8(IDT_PIC1_BASEADDRESS+1, baseIRQ);
    portIO_write8(IDT_PIC2_BASEADDRESS+1, baseIRQ+0x08);

    portIO_write8(IDT_PIC1_BASEADDRESS+1, 0x04);
    portIO_write8(IDT_PIC2_BASEADDRESS+1, 0x02);

    portIO_write8(IDT_PIC1_BASEADDRESS+1, 0x01);
    portIO_write8(IDT_PIC2_BASEADDRESS+1, 0x01);

    portIO_write8(IDT_PIC1_BASEADDRESS+1, 0x0);
    portIO_write8(IDT_PIC2_BASEADDRESS+1, 0x0);
}

void interrupts_setGate(uint8 Number, uint32 Address, uint16 Selector, uint8 Flags) {
    IDT_Table[Number].Address_Low = (Address & 0xFFFF);
    IDT_Table[Number].Address_High = (Address >> 16) & 0xFFFF;

    IDT_Table[Number].Selector = Selector;
    IDT_Table[Number].Empty = 0x00;
    IDT_Table[Number].Flags = Flags;
}
