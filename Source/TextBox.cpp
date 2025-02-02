#include <Graphics/TextBox.hpp>
#include <memoryManager.h>
#include <debug.h>
#include <memory.h>

TextBox::TextBox(Canvas* canvas, ScreenFont* font, uint32 x, uint32 y, uint16 rows, uint16 columns) {
    this->x = x;
    this->y = y;
    this->rows = rows;
    this->columns = columns;
    this->currentColumn = 0;
    this->currentRow = 0;
    this->canvas = canvas;
    this->font = font;
    this->colour = 0x00FFFFFFF; // white.
    // do stuff.

    this->characterBuffer = (char**)memoryManager_allocate(sizeof(char*) * rows);
    for(int i = 0; i < rows; i++){
        this->characterBuffer[i] = (char*)memoryManager_allocate(sizeof(char) * columns);
        memset(this->characterBuffer[i], '    ', columns);
    }
}

void TextBox::PutChar(char c) {
    switch(c) {
        case 0x08:    // Backspace
            if (currentColumn != 0) {
                currentColumn--;
            }
            
            this->characterBuffer[this->currentRow][this->currentColumn] = ' ';
            vga_drawRect(canvas, x + (currentColumn * 8), y + (currentRow * 16), 8, 16, 0x00000000);
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
            vga_drawChar(canvas, font, x + (currentColumn * 8), y + (currentRow * 16), this->colour, c);

            currentColumn++;

            if(currentColumn >= columns) {
                currentColumn = 0;
                currentRow++;
            }
            break;
    }

    if (currentRow >= rows) {
        debug(LOGLEVEL_DEBUG, "Need to scrooll.");
        Scroll();
    }

    debug(LOGLEVEL_DEBUG, "Framebuffer row = %d, Framebuffer column = %d", this->currentRow, this->currentColumn);
}

void TextBox::Scroll() {
    debug(LOGLEVEL_DEBUG, "Scrolling...");
    // save address of top row.
    char* oldTopRow = characterBuffer[0];
    debug(LOGLEVEL_DEBUG, "Old row address: %h", oldTopRow);

    // move every row up one except first row
    for(int i = 1; i < rows; i++) {
        debug(LOGLEVEL_DEBUG, "i: %d", i);
        characterBuffer[i-1] = characterBuffer[i];
    }
    // set last row as old first row.
    characterBuffer[rows - 1] = oldTopRow;

    // empty new last row
    for(int i = 0; i < columns; i++) {
        debug(LOGLEVEL_DEBUG, "i: %d", i);
        characterBuffer[rows - 1][i] = ' ';
    }

    currentRow--;

    // redraw
    Redraw();
}

void TextBox::Redraw() {
    debug(LOGLEVEL_DEBUG, "Redrawing...");
    vga_drawRect(canvas, x, y, x + (columns * 8), y + (rows * 16), 0x00000000);

    for(int row = 0; row < rows; row++) {
        for(int column = 0; column < columns; column++) {
            vga_drawChar(canvas, font, x + (column * 8), y + (row * 16), this->colour, characterBuffer[row][column]);
        }
    }

    debug(LOGLEVEL_DEBUG, "Redrawed.");
}
