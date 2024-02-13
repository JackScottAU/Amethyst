#include <keyboard.h>
#include <interrupts.h>
#include <memoryManager.h>
#include <portIO.h>
#include <vgaConsole.h>
#include <serial.h>
#include <Structures/fifobuffer.h>
#include <deviceTree.h>



bool shift = false;
bool capsActive = false;
bool capsDown = false;
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

    shift = false;
    capsActive = false;
    capsDown = false;
}

// 8 scancodes per line = 16 lines.
uint8 keyboard_scanCodesNormal[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, '`', 0, 
    0, 0, 0, 0, 0, 'q', '1', 0, 
    0, 0, 'z', 's', 'a', 'w', '2', 0, 
    0, 'c', 'x', 'd', 'e', '4', '3', 0, 
    0, ' ', 'v', 'f', 't', 'r', '5', 0, 
    0, 'n', 'b', 'h', 'g', 'y', '6', 0, 
    0, 0, 'm', 'j', 'u', '7', '8', 0, 
    0, ',', 'k', 'i', 'o', '0', '9', 0, 
    0, '.', '/', 'l', ';', 'p', '-', 0, 
    0, 0, '\'', 0, '[', '=', 0, 0, 
    0, 0, '\n', ']', 0, '\\', 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 
};

// 8 scancodes per line = 16 lines.
uint8 keyboard_scanCodesShift[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, '~', 0, 
    0, 0, 0, 0, 0, 'Q', '!', 0, 
    0, 0, 'Z', 'S', 'A', 'W', '@', 0, 
    0, 'C', 'X', 'D', 'E', '$', '#', 0, 
    0, ' ', 'V', 'F', 'T', 'R', '%', 0, 
    0, 'N', 'B', 'H', 'G', 'Y', '^', 0, 
    0, 0, 'M', 'J', 'U', '&', '*', 0, 
    0, '<', 'K', 'I', 'O', ')', '(', 0, 
    0, '>', '?', 'L', ':', 'P', '_', 0, 
    0, 0, '\'', 0, '{', '+', 0, 0, 
    0, 0, '\n', '}', 0, '\\', 0, 0, 
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

    if(data == 0xF0) {
        // Ignore release for now.
        FIFOBuffer_ReadBytes(keyboard_buffer, &data, 1);

      //  if(data == 0x58) {
      //      capsDown = false;
      //  }

        if(data == 0x59 || data == 0x12) {
            shift = false;
        }

        // Recurse to get an actual key.
        return keyboard_readChar();
    }

    if(data == 0x59 || data == 0x12) {
        shift = true;
    }

    if(shift || capsActive) {
        return keyboard_scanCodesShift[data];
    } else{
        return keyboard_scanCodesNormal[data];
    }


    // pseduocode: have two arrays (one for non-shift and one for shift) which have keycode:ascii mappings.
    // keep track of modifier keys (caps, shift, etc), use these to choose the array to use.
    // return array[scanocode];
}
