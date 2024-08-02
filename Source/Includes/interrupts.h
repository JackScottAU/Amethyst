/**
 *  Amethyst Operating System - x86 interrupts support.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Registers.h>
#include <Types.h>

#ifndef INCLUDES_INTERRUPTS_H_
#define INCLUDES_INTERRUPTS_H_

#ifdef __cplusplus
extern "C" {
#endif

//-- Public Data Structures --//
typedef struct interrupts_handlerCallback_s {
    void (* funcToCall)(uint32 arbitraryNumber);
    uint8 interruptNumber;
    uint32 arbitraryNumber;
    struct interrupts_handlerCallback_s* next;
} interrupts_handlerCallback;

//-- Public Interface --//
void interrupts_initialise(void);
interrupts_handlerCallback* interrupts_addHandler(uint8 interruptNumber, uint32 argument, void (* callback)(uint32));
void interrupts_removeHandler(interrupts_handlerCallback* request);

//-- Private Interface --//
void interrupts_disableInterrupts(void);
void interrupts_enableInterrupts(void);

void interrupts_installEmptyIDT(void);
void interrupts_installISRs(void);
void interrupts_remap8259s(uint8 baseIRQ);
void interrupts_setGate(uint8 Number, uint32 Address, uint16 GDTSelector, uint8 Flags);

void interrupts_handler(struct Registers_S *Registers);

//-- Private Definitions --//
// Defines whether the interrupt table entry is valid.
#define IDT_FLAG_PRESENT    0x80

// Minimum level for callers.
#define IDT_FLAG_RING0      0x00
#define IDT_FLAG_RING1      0x20
#define IDT_FLAG_RING2      0x40
#define IDT_FLAG_RING3      0x60

// Type of gate.
#define IDT_FLAG_TASKGATE   0x05
#define IDT_FLAG_INTGATE16  0x06
#define IDT_FLAG_INTGATE32  0x0E
#define IDT_FLAG_TRAPGATE16 0x07
#define IDT_FLAG_TRAPGATE32 0x0F

// Base address for 8529 PICs
#define IDT_PIC1_BASEADDRESS    0x20
#define IDT_PIC2_BASEADDRESS    0xA0

//-- Interrupt Service Routine Stubs --//
void interrupts_ISR_00();
void interrupts_ISR_01();
void interrupts_ISR_02();
void interrupts_ISR_03();
void interrupts_ISR_04();
void interrupts_ISR_05();
void interrupts_ISR_06();
void interrupts_ISR_07();
void interrupts_ISR_08();
void interrupts_ISR_09();
void interrupts_ISR_0A();
void interrupts_ISR_0B();
void interrupts_ISR_0C();
void interrupts_ISR_0D();
void interrupts_ISR_0E();
void interrupts_ISR_0F();
void interrupts_ISR_10();
void interrupts_ISR_11();
void interrupts_ISR_12();
void interrupts_ISR_13();
void interrupts_ISR_14();
void interrupts_ISR_15();
void interrupts_ISR_16();
void interrupts_ISR_17();
void interrupts_ISR_18();
void interrupts_ISR_19();
void interrupts_ISR_1A();
void interrupts_ISR_1B();
void interrupts_ISR_1C();
void interrupts_ISR_1D();
void interrupts_ISR_1E();
void interrupts_ISR_1F();
void interrupts_ISR_20();
void interrupts_ISR_21();
void interrupts_ISR_22();
void interrupts_ISR_23();
void interrupts_ISR_24();
void interrupts_ISR_25();
void interrupts_ISR_26();
void interrupts_ISR_27();
void interrupts_ISR_28();
void interrupts_ISR_29();
void interrupts_ISR_2A();
void interrupts_ISR_2B();
void interrupts_ISR_2C();
void interrupts_ISR_2D();
void interrupts_ISR_2E();
void interrupts_ISR_2F();
void interrupts_ISR_30();
void interrupts_ISR_31();
void interrupts_ISR_32();
void interrupts_ISR_33();
void interrupts_ISR_34();
void interrupts_ISR_35();
void interrupts_ISR_36();
void interrupts_ISR_37();
void interrupts_ISR_38();
void interrupts_ISR_39();
void interrupts_ISR_3A();
void interrupts_ISR_3B();
void interrupts_ISR_3C();
void interrupts_ISR_3D();
void interrupts_ISR_3E();
void interrupts_ISR_3F();
void interrupts_ISR_40();
void interrupts_ISR_41();
void interrupts_ISR_42();
void interrupts_ISR_43();
void interrupts_ISR_44();
void interrupts_ISR_45();
void interrupts_ISR_46();
void interrupts_ISR_47();
void interrupts_ISR_48();
void interrupts_ISR_49();
void interrupts_ISR_4A();
void interrupts_ISR_4B();
void interrupts_ISR_4C();
void interrupts_ISR_4D();
void interrupts_ISR_4E();
void interrupts_ISR_4F();
void interrupts_ISR_50();
void interrupts_ISR_51();
void interrupts_ISR_52();
void interrupts_ISR_53();
void interrupts_ISR_54();
void interrupts_ISR_55();
void interrupts_ISR_56();
void interrupts_ISR_57();
void interrupts_ISR_58();
void interrupts_ISR_59();
void interrupts_ISR_5A();
void interrupts_ISR_5B();
void interrupts_ISR_5C();
void interrupts_ISR_5D();
void interrupts_ISR_5E();
void interrupts_ISR_5F();
void interrupts_ISR_60();
void interrupts_ISR_61();
void interrupts_ISR_62();
void interrupts_ISR_63();
void interrupts_ISR_64();
void interrupts_ISR_65();
void interrupts_ISR_66();
void interrupts_ISR_67();
void interrupts_ISR_68();
void interrupts_ISR_69();
void interrupts_ISR_6A();
void interrupts_ISR_6B();
void interrupts_ISR_6C();
void interrupts_ISR_6D();
void interrupts_ISR_6E();
void interrupts_ISR_6F();
void interrupts_ISR_70();
void interrupts_ISR_71();
void interrupts_ISR_72();
void interrupts_ISR_73();
void interrupts_ISR_74();
void interrupts_ISR_75();
void interrupts_ISR_76();
void interrupts_ISR_77();
void interrupts_ISR_78();
void interrupts_ISR_79();
void interrupts_ISR_7A();
void interrupts_ISR_7B();
void interrupts_ISR_7C();
void interrupts_ISR_7D();
void interrupts_ISR_7E();
void interrupts_ISR_7F();

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_INTERRUPTS_H_
