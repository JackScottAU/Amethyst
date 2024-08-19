/**
 *  Amethyst Operating System - x86 interrupt handler register layout.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/


#ifndef INCLUDES_REGISTERS_H_
#define INCLUDES_REGISTERS_H_

    /// @brief Form of the registers passed to the interrupt handler by the CPU.
    struct Registers_S {
        unsigned int GS, FS, ES, DS;
        unsigned int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
        unsigned int IntNum, ErrorCode;
        unsigned int EIP, CS, EFlags, UserESP, SS;
    };

#endif  // INCLUDES_REGISTERS_H_
