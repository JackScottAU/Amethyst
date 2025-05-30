/**
 *  Amethyst Operating System - Interrupts handler functionality.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <portIO.h>
#include <Clock.h>
#include <Registers.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <stream.h>
#include <thread.h>
#include <debug.h>

interrupts_handlerCallback* interrupts_callbacks = (interrupts_handlerCallback*) NULL;

/**
 * @param Registers The state of the registers on the stack provided by the ISR stub.
 */
void interrupts_handler(struct Registers_S *Registers) {
    // If it is an exception.
    if (Registers->IntNum <= 0x1F) {
        debug(LOGLEVEL_CRITICAL, "\033[37m\033[41m\n");
        debug(LOGLEVEL_CRITICAL, "                                                                                ");
        debug(LOGLEVEL_CRITICAL, "                                ! SYSTEM PANIC !                                ");
        debug(LOGLEVEL_CRITICAL, "                                                                                ");
        debug(LOGLEVEL_CRITICAL, "\033[0m\n");

        debug(LOGLEVEL_CRITICAL, "Interrupt number: %d\tError code: %h\n", Registers->IntNum, Registers->ErrorCode);
        debug(LOGLEVEL_CRITICAL, "EAX: %h\tEBX: %h\tECX: %h\tEDX: %h\n", Registers->EAX, Registers->EBX, Registers->ECX, Registers->EDX);
        debug(LOGLEVEL_CRITICAL, "CS: %h\tDS: %h\tES: %h\tFS: %h\tGS: %h\t\n", Registers->CS, Registers->DS, Registers->ES, Registers->FS, Registers->GS);
        debug(LOGLEVEL_CRITICAL, "EDI: %h\tESI: %h\tEBP: %h\tESP: %h\n", Registers->EDI, Registers->ESI, Registers->EBP, Registers->ESP);
        debug(LOGLEVEL_CRITICAL, "EIP: %h\tEFlags: %h\tUserESP: %h\tSS: %h\n", Registers->EIP, Registers->EFlags, Registers->UserESP, Registers->SS);

        interrupts_disableInterrupts();
        haltCPU();
    }

    // Check all interrupt handler callbacks to see if any need to be processed.
    interrupts_handlerCallback* current = interrupts_callbacks;
    while (current) {
        // Check to see if this callback is for the current interrupt number.
        if ((current->interruptNumber == Registers->IntNum)) {
            // We need to process this interrupt callback by calling the stored function.
            void (*foo)(uint32);                                          // Declare variable to store the function pointer.
            foo = current->funcToCall;                              // Retrieve function pointer.
            (*foo)(current->arbitraryNumber);    // Call the function.
        }
        current = current->next;
    }

    // If it is an IRQ.
    if ((Registers->IntNum >= 0x20) && (Registers->IntNum <= 0x2F)) {
        // Send EOI to 8259 controller.
        if (Registers->IntNum >= 0x28) {
            // If it is the slave, send it to that.
            portIO_write8(0xA0, 0x20);
        }

        // But send it to the master no matter what.
        portIO_write8(0x20, 0x20);
    }

    scheduler();
}

interrupts_handlerCallback* interrupts_addHandler(uint8 interruptNumber, uint32 argument, void (* callback)(uint32)) {
    interrupts_handlerCallback* request = memoryManager_allocate(sizeof(interrupts_handlerCallback));

    request->interruptNumber = interruptNumber;
    request->arbitraryNumber = argument;
    request->funcToCall = callback;

    // Add it to the front of the list. We'll figure out sorting it later.
    request->next = interrupts_callbacks;
    interrupts_callbacks = request;

    return(request);
}

void interrupts_removeHandler(interrupts_handlerCallback* request) {
    interrupts_handlerCallback* current = interrupts_callbacks;
    interrupts_handlerCallback* oldCurrent = 0;

    if (current == request) {
        interrupts_callbacks = current->next;
        memoryManager_free(current);
    }

    while (current) {
        // Perform check.
        if (current == request) {
            // Remove it from the list.
            interrupts_handlerCallback*  temp = current;

            oldCurrent->next = current->next;
            current = oldCurrent->next;

            memoryManager_free(temp);
        } else {
            oldCurrent = current;
            current = current->next;
        }
    }
}
