#include <Graphics/canvas.h>
#include <Graphics/screenfont.h>
#include <Types.h>

#ifndef INCLUDES_FRAMEBUFFER_H_
#define INCLUDES_FRAMEBUFFER_H_

void vga_putPixel(Canvas* canvas, uint32 x, uint32 y, uint32 colour);
void vga_drawRect(Canvas* canvas, uint16 x, uint16 y, uint16 w, uint16 h, uint32 colour);
void vga_drawChar(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char a);
void vga_drawWord(Canvas* canvas, ScreenFont* font, uint16 x, uint16 y, uint32 colour, char* a);

#endif // INCLUDES_FRAMEBUFFER_H_

