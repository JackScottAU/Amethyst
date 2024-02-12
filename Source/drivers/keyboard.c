#include <keyboard.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>
#include <Structures/fifobuffer.h>
#include <deviceTree.h>


FIFOBuffer* keyboard_buffer;

void keyboard_interruptHandler(uint32 eventData);

void keyboard_interruptHandler(uint32 eventData) {
    eventData++;

    uint8 data = portIO_read8(0x60);

    FIFOBuffer_WriteBytes(keyboard_buffer, &data, 1);

    stream_printf(serial_writeChar, "keyboard: %h\n", data);
}

deviceTree_Entry* keyboard_initialise() {
    interrupts_addHandler(0x21, 0, (*keyboard_interruptHandler));

    keyboard_buffer = FIFOBuffer_new(1024);

    return deviceTree_createDevice("Generic PS/2 Keyboard", DEVICETREE_TYPE_OTHER, NULL);
}
