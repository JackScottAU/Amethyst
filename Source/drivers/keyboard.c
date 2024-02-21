#include <keyboard.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>
#include <Structures/fifobuffer.h>
#include <deviceTree.h>
#include <stream.h>



bool shift = false;
bool capsActive = false;
bool commandMode = false;
FIFOBuffer* keyboard_buffer;

void keyboard_interruptHandler(uint32 eventData);

void keyboard_interruptHandler(uint32 eventData) {
    eventData++;

    uint8 data = portIO_read8(0x60);

    if(commandMode == true) {
        // discard data for now.
        stream_printf(serial_writeChar, "keyboard: %h\n", data);
    } else {

        FIFOBuffer_WriteBytes(keyboard_buffer, &data, 1);

        stream_printf(serial_writeChar, "keyboard: %h\n", data);
    }
}

deviceTree_Entry* keyboard_initialise() {
    interrupts_addHandler(0x21, 0, (*keyboard_interruptHandler));

    keyboard_buffer = FIFOBuffer_new(1024);

    shift = false;
    capsActive = false;
    commandMode = false;

    return deviceTree_createDevice("Generic PS/2 Keyboard", DEVICETREE_TYPE_OTHER, NULL);
}

// 8 scancodes per line = 16 lines.
uint8 keyboard_scanCodesNormal[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', 
    '7', '8', '9', '0', '-', '=', '\b', '\t', 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 
    'o', 'p', '[', ']', '\n', 0, 'a', 's', 
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 
    'b', 'n', 'm', ',', '.', '/', 0, 0, 
    0, ' ', 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
};

// 8 scancodes per line = 16 lines.
uint8 keyboard_scanCodesShift[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', 
    '&', '*', '(', ')', '_', '+', '\b', '\t', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 
    'O', 'P', '{', '}', '\n', 0, 'A', 'S', 
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 
    'B', 'N', 'M', '<', '>', '?', 0, 0, 
    0, ' ', 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
};

char keyboard_readChar(void)
{
    while(FIFOBuffer_ContentsSize(keyboard_buffer) == 0) {

    }

    uint8 data;

    FIFOBuffer_ReadBytes(keyboard_buffer, &data, 1);

    stream_printf(serial_writeChar, "keyboard readchar: %h\n", data);

    if(data & 0x80) {
        // A key has been released.

        if(data == 0xBA) {
            if(capsActive == true) {
                capsActive = false;
            }
            else {
                capsActive = true;
            }
        }

        if(data == 0xAA || data == 0xB6) {
            shift = false;
        }

        // Recurse to get an actual key.
        return keyboard_readChar();
    }

    if(data == 0x2A || data == 0x36) {
        shift = true;
    }

    char key;

    if(shift || capsActive) {
        key = keyboard_scanCodesShift[data];
    } else{
        key = keyboard_scanCodesNormal[data];
    }

    if(key != 0) {
        return key;
    } else {
        return keyboard_readChar();
    }
}
