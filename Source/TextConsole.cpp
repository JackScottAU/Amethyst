#include <Graphics/TextConsole.hpp>
#include <memoryManager.h>
#include <debug.h>
#include <memory.h>
#include <string.h>
#include <Graphics/screenfont.h>

TextConsole::TextConsole(Canvas* canvas, ScreenFont* font, uint32 x, uint32 y, uint16 rows, uint16 columns) {
    this->x = x;
    this->y = y;
    this->rows = rows;
    this->columns = columns;
    this->currentColumn = 0;
    this->currentRow = 0;
    this->canvas = canvas;
    this->font = font;
    this->colour = 0x00FFFFFFF; // white.
    this->backcolour = 0x0; //black
    // do stuff.

    this->characterBuffer = (char**)memoryManager_allocate(sizeof(char*) * rows);
    for(int i = 0; i < rows; i++){
        this->characterBuffer[i] = (char*)memoryManager_allocate(sizeof(char) * columns);
        memset(this->characterBuffer[i], ' ', columns);
    }

    Redraw();

    // Set aside 128 characters for CSI parameter storage.
    csiParameters = (char*)memoryManager_allocate(128);
    isInCSI = false;
}

void TextConsole::PutChar(char c) {
    if (isInCSI) {
        HandleControlSequenceIntroducer(c);
        return;
    }


    switch(c) {
        case 0x1B:  // ANSI Escape Code Handling
            isInCSI = true;
            memset(csiParameters, 0, sizeof(char) * 128);
            break;

        case 0x08:    // Backspace
            if (currentColumn != 0) {
                currentColumn--;
            }
            
            this->characterBuffer[this->currentRow][this->currentColumn] = ' ';
            canvas_drawRect(canvas, x + (currentColumn * 8), y + (currentRow * 16), 8, 16, backcolour);
            break;

        case 0x09:    // Tab
            currentColumn = (currentColumn + 8) & ~(8 - 1);
            break;

        case '\n':
            currentColumn = 0;
            currentRow++;
            break;

        case '\r':
            currentColumn = 0;
            break;

        default:
            //printable character.
            this->characterBuffer[this->currentRow][this->currentColumn] = c;
            screenfont_drawChar(canvas, font, x + (currentColumn * 8), y + (currentRow * 16), this->colour, c);

            currentColumn++;

            if(currentColumn >= columns) {
                currentColumn = 0;
                currentRow++;
            }
            break;
    }

    if (currentRow >= rows) {
        debug(LOGLEVEL_TRACE, "Need to scrooll.");
        Scroll();
    }

    debug(LOGLEVEL_TRACE, "Framebuffer row = %d, Framebuffer column = %d", this->currentRow, this->currentColumn);
}

void TextConsole::HandleControlSequenceIntroducer(char c) {
    // if we get a command, execute that and remove isInCSI.
    // else store characters into parameter string.

    int i = 0;

    switch (c) {
        case 'm':
            HandleSelectGraphicsRendition();
            isInCSI = false;
            return;

        default:
            while (csiParameters[i] != 0) {
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

/** When given a parameter string,  */
void TextConsole::HandleSelectGraphicsRendition() {
    int i = 0;
    while (csiParameters[i] != '\0') {
        if (csiParameters[i] == '[' || csiParameters[i] == ';') {
            int sgr = string_parseInt(csiParameters + i + 1);

            DecodeSGR(sgr);
        }

        i++;
    }
}

void TextConsole::DecodeSGR(uint32 parameter) {
    switch (parameter) {
        case 0:
            colour = 0x00FFFFFF;
            backcolour = 0x00000000;
            break;

        case 30:
            colour = 0x00000000;
            break;

        case 31:
            colour = 0x00FF0000;
            break;

        case 32:
            colour = 0x0000FF00;
            break;

        case 33:
            colour = 0x00F08000;
            break;

        case 34:
            colour = 0x000000FF;
            break;

        case 35:
            colour = 0x00FF00FF;
            break;

        case 36:
            colour = 0x00008080;
            break;

        case 37:
            colour = 0x00808080;
            break;

        case 40:
            backcolour = 0x00000000;
            break;

        case 41:
            backcolour = 0x00FF0000;
            break;

        case 42:
            backcolour = 0x0000FF00;
            break;

        case 43:
            backcolour = 0x00F08000;
            break;

        case 44:
            backcolour = 0x000000FF;
            break;

        case 45:
            backcolour = 0x00FF00FF;
            break;

        case 46:
            backcolour = 0x00008080;
            break;

        case 47:
            backcolour = 0x00808080;
            break;
    }
}

void TextConsole::Scroll() {
    debug(LOGLEVEL_TRACE, "Scrolling...");
    // save address of top row.
    char* oldTopRow = characterBuffer[0];

    // move every row up one except first row
    for(int i = 1; i < rows; i++) {
        characterBuffer[i-1] = characterBuffer[i];
    }
    // set last row as old first row.
    characterBuffer[rows - 1] = oldTopRow;

    // empty new last row
    for(int i = 0; i < columns; i++) {
        characterBuffer[rows - 1][i] = ' ';
    }

    currentRow--;

    // redraw
    Redraw();
}

void TextConsole::Redraw() {
    canvas_drawRect(canvas, x, y, x + (columns * 8), y + (rows * 16), backcolour);

    for(int row = 0; row < rows; row++) {
        for(int column = 0; column < columns; column++) {
            screenfont_drawChar(canvas, font, x + (column * 8), y + (row * 16), this->colour, characterBuffer[row][column]);
        }
    }

    debug(LOGLEVEL_TRACE, "Redrawed.");
}
