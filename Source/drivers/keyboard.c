#include <keyboard.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>

char* keyboard_buffer;
uint32 keyboard_bufferLocation;

void keyboard_interruptHandler(uint32 eventData);

void keyboard_interruptHandler(uint32 eventData) {
    eventData++;

    keyboard_buffer[keyboard_bufferLocation] = portIO_read8(0x60);

    vgaConsole_printf("%h", keyboard_buffer[keyboard_bufferLocation]);

    keyboard_bufferLocation++;
}

void keyboard_registerHandler() {
    interrupts_addHandler(0x21, 0, (*keyboard_interruptHandler));

    keyboard_buffer = memoryManager_allocate(sizeof(uint8) * 1024);
    keyboard_bufferLocation = 0;
}
