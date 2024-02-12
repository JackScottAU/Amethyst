#include <keyboard.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>

uint8* keyboard_buffer;
uint32 keyboard_bufferLocation;

void keyboard_interruptHandler(uint32 eventData);

void keyboard_interruptHandler(uint32 eventData) {
    eventData++;

    uint8 data = portIO_read8(0x60);

    keyboard_buffer[keyboard_bufferLocation] = data;

    stream_printf(serial_writeChar, "keyboard: %h\n", data);

    keyboard_bufferLocation++;
}

void keyboard_registerHandler() {
    interrupts_addHandler(0x21, 0, (*keyboard_interruptHandler));

    keyboard_buffer = memoryManager_allocate(sizeof(uint8) * 1024);
    keyboard_bufferLocation = 0;
}
