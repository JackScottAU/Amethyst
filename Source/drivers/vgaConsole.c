/**
 *  Amethyst Operating System - VGA Console functionality.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

/**
 * Text console driver used by the kernel at boot-up, before VESA (or other video) drivers are loaded.
 * Also includes a basic kernel printf()-style function.
 * Unlike the rest of the system, this operates in ASCII instead of UTF-8.
 * It's also designed to rely on as few other system components as possible. Basically, just the port IO functions.
 */

#include <portIO.h>
#include <Types.h>
#include <vgaConsole.h>
#include <stdarg.h>
#include <stream.h>
#include <serial.h>
#include <memoryManager.h>
#include <string.h>

// These are defined here and not in .h because they are only relevant to implementation.
#define VGACONSOLE_HEIGHT   25    // Defines the height of the screen in characters.
#define VGACONSOLE_WIDTH    80    // Defines the width of the screen in characters.

/** Holds the base address of the text area for the VGA controller. */
uint16* vgaConsole_videoMemory    = (uint16*) 0xC00B8000;

/** Holds the horizontal offset in characters from the left of the screen. */
uint8 vgaConsole_cursorX    = 0;

/** Holds the vertical offset in lines from the top of the screen. */
uint8 vgaConsole_cursorY    = 0;

/** Holds the current value of the VGA attribute byte (which controls the colour of the displayed text). */
uint8 vgaConsole_colour        = VGACONSOLE_LIGHT_GREY;

void vgaConsole_setColour(uint8 foreColour, uint8 backColour);
void vgaConsole_scroll(void);
void vgaConsole_updateCursor(void);
void vgaConsole_setCursor(uint8 x, uint8 y);
void vgaConsole_clearScreen(void);

void vga_writeRegister(uint8 registerNo, uint8 data);
void vga_writeRegister(uint8 registerNo, uint8 data) {
    portIO_write8(0x3D4, registerNo);
    portIO_write8(0x3D5, data);
}

void vgaConsole_handleControlSequenceIntroducer(char c);
void vgaConsole_handleSelectGraphicsRendition(void);

void vgaConsole_setForeColour(uint8 foreColour);
void vgaConsole_setBackColour(uint8 backColour);

bool isInCSI;
char* csiParameters;    // 32 bytes of control sequence storage.

/**
 * Initialises the VGA console ready for characters to be sent.
 */
void vgaConsole_initialise(void) {
    // enable cursor
    vga_writeRegister(0x0A, 0x00);
    vga_writeRegister(0x0B, 0x1F);

    isInCSI = false;
}

/**
 * Clears the screen.
 */
void vgaConsole_clearScreen(void) {
    // While this isn't a particularly gruelling calculation, we may as well only do it once.
    // LSB is a space character, MSB is the currently selected colour.
    uint16 blank = 0x20 | (vgaConsole_colour << 8);

    // Copy this repeatedly to video memory, to cover the whole screen.
    for (int i = 0; i < VGACONSOLE_HEIGHT * VGACONSOLE_WIDTH; i++) {
        vgaConsole_videoMemory[i] = blank;
    }

    // Update the location of the cursor on screen.
    vgaConsole_cursorX = 0;
    vgaConsole_cursorY = 0;
    vgaConsole_updateCursor();
}

/**
 * Puts a single character on the screen, obeying all ASCII rules.
 * @param c A single character.
 */
void vgaConsole_putChar(char c) {
    if(isInCSI) {
        vgaConsole_handleControlSequenceIntroducer(c);
        return;
    }

    switch (c) {
        case 0x08:    // Backspace
            if (vgaConsole_cursorX != 0) {
                vgaConsole_cursorX--;
            }
            vgaConsole_videoMemory[(vgaConsole_cursorY*VGACONSOLE_WIDTH)+vgaConsole_cursorX] =
                ' ' | (vgaConsole_colour << 8);
            break;

        case 0x09:    // Tab
            vgaConsole_cursorX = (vgaConsole_cursorX + 8) & ~(8 - 1);
            break;

        case 0x0A:    // New Line
            vgaConsole_cursorX = 0;
            vgaConsole_cursorY++;
            break;

        case 0x0D:    //  Carriage Return
            vgaConsole_cursorX = 0;
            break;

        case 0x1B:  // ANSI Escape Code Handling
            isInCSI = true;
            csiParameters = memoryManager_allocate(32);
            for(int i = 0; i < 32; i++){
                csiParameters[i] = 0;
            }
            break;

        default:    // All other characters
            if (c >= 0x20 && c <= 0x7E) {
                // If it smaller than a space, then it is a control char,
                // and should be ignored. Or it's a delete (7E) or a UTF char.

                vgaConsole_videoMemory[(vgaConsole_cursorY*VGACONSOLE_WIDTH)+vgaConsole_cursorX] =
                    c | (vgaConsole_colour << 8);
                vgaConsole_cursorX++;
            }
            break;
    }

    // New Line if edge reached
    if (vgaConsole_cursorX >= VGACONSOLE_WIDTH) {
        vgaConsole_cursorX = 0;
        vgaConsole_cursorY++;
    }

    // Scroll if needed.
    if (vgaConsole_cursorY >= VGACONSOLE_HEIGHT) {
        vgaConsole_scroll();
    }

    vgaConsole_updateCursor();
}

void vgaConsole_handleControlSequenceIntroducer(char c) {
    
    // if we get a command, execute that and remove isInCSI.
    // else store characters into parameter string.

    int i = 0;

    switch(c) {
        case 'm':
            vgaConsole_handleSelectGraphicsRendition();
            isInCSI = false;
            memoryManager_free(csiParameters);
            return;

        default:
            while(csiParameters[i] != 0) {
                i++;
              //  stream_printf(serial_writeChar, "CSI(%d): %d\n", i, csiParameters[i]);
            }

            // now we are somewhere to store our parameter bit.
            csiParameters[i] = c;
            csiParameters[i+1] = 0;
       //       serial_writeString("CSI: ");
        //      serial_writeLine(csiParameters);
            return;
    }
}

void vgaConsole_decodeSGR(uint32 parameter);

/** When given a parameter string,  */
void vgaConsole_handleSelectGraphicsRendition() {
    int i = 0;
    while(csiParameters[i] != '\0') {
        if(csiParameters[i] == '[' || csiParameters[i] == ';') {
            int sgr = string_parseInt(csiParameters + i + 1);

            vgaConsole_decodeSGR(sgr);
        }

        i++;
    }
}

void vgaConsole_decodeSGR(uint32 parameter) {
    switch(parameter) {
        case 0:
            vgaConsole_setColour(VGACONSOLE_LIGHT_GREY, VGACONSOLE_BLACK);
            break;

        case 30:
            vgaConsole_setForeColour(VGACONSOLE_BLACK);
            break;
            
        case 31:
            vgaConsole_setForeColour(VGACONSOLE_RED);
            break;

        case 32:
            vgaConsole_setForeColour(VGACONSOLE_GREEN);
            break;

        case 33:
            vgaConsole_setForeColour(VGACONSOLE_BROWN);
            break;

        case 34:
            vgaConsole_setForeColour(VGACONSOLE_BLUE);
            break;
            
        case 35:
            vgaConsole_setForeColour(VGACONSOLE_MAGENTA);
            break;

        case 36:
            vgaConsole_setForeColour(VGACONSOLE_CYAN);
            break;

        case 37:
            vgaConsole_setForeColour(VGACONSOLE_LIGHT_GREY);
            break;

        case 40:
            vgaConsole_setBackColour(VGACONSOLE_BLACK);
            break;
            
        case 41:
            vgaConsole_setBackColour(VGACONSOLE_RED);
            break;

        case 42:
            vgaConsole_setBackColour(VGACONSOLE_GREEN);
            break;

        case 43:
            vgaConsole_setBackColour(VGACONSOLE_BROWN);
            break;

        case 44:
            vgaConsole_setBackColour(VGACONSOLE_BLUE);
            break;
            
        case 45:
            vgaConsole_setBackColour(VGACONSOLE_MAGENTA);
            break;

        case 46:
            vgaConsole_setBackColour(VGACONSOLE_CYAN);
            break;

        case 47:
            vgaConsole_setBackColour(VGACONSOLE_LIGHT_GREY);
            break;
    }
}

void vgaConsole_scroll(void) {
    for (uint16 i = 80; i < (80*25); i++) {
        vgaConsole_videoMemory[i-80] = vgaConsole_videoMemory[i];
    }

    for (uint16 i = 80*25; i < (80*26); i++) {
        vgaConsole_videoMemory[i-80] = 0x0000;
    }

    vgaConsole_cursorY--;
    vgaConsole_updateCursor();
}

/**
 * Changes current colour by setting attribute byte in VideoInfo structure.
 * @param foreColour Foreground Colour (0-15)
 * @param backColour Background Colour (0-15)
 */
void vgaConsole_setColour(unsigned char foreColour, unsigned char backColour) {
    if ((foreColour < 16) && (backColour < 16)) {
        vgaConsole_colour = (backColour << 4) | (foreColour);
    } else {
        /* INVALID COLOUR ARRANGEMENT */
    }
}

void vgaConsole_setForeColour(uint8 foreColour) {
    uint8 backColour = vgaConsole_colour >> 4;

    vgaConsole_colour = backColour << 4 | foreColour;
}

void vgaConsole_setBackColour(uint8 backColour) {
    uint8 foreColour = vgaConsole_colour & 0xF;

    vgaConsole_colour = backColour << 4 | foreColour;
}

/**
 * Moves the VGA BIOS cursor to the current X and Y positions.
 */
void vgaConsole_updateCursor(void) {
    // QEMU doesn't like setting cursor to line 24 for some reason. No idea why.
    vgaConsole_setCursor(vgaConsole_cursorX, vgaConsole_cursorY);
}

void vgaConsole_setCursor(uint8 x, uint8 y) {
    // The offset, in characters, from the top-left of the screen.
    uint16 offset;

    // Compute the offset from the current X and Y positions.
    offset = (y * 80) + x;

    uint8 high = (offset >> 8);
    uint8 low = offset & 0xFF;

    // Write the values to the CRT control register indices 14 and 15.
    vga_writeRegister(0x0F, low);
    vga_writeRegister(0x0E, high);
}
