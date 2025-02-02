#include <Graphics/TargaImage.hpp>
#include <memoryManager.h>
#include <Types.h>
#include "drivers/vesa_framebuffer.h"
#include <debug.h>

// Code ripped wholesale from https://wiki.osdev.org/Loading_Icons
TargaImage::TargaImage(uint8* ptr, uint32 length, uint32 locx, uint32 locy, Canvas* canvas)
{
    this->children = new LinkedList<Widget*>();
    this->canvas = canvas;
    this->x = locx;
    this->y = locy;

    uint32* data;
    int i, j, k, x, y;
    int o = (ptr[11] << 8) + ptr[10];
    int m = ((ptr[1]? (ptr[7]>>3)*ptr[5] : 0) + 18);

    this->w = (ptr[13] << 8) + ptr[12];
    this->h = (ptr[15] << 8) + ptr[14];

  //  if(w<1 || h<1) return NULL;

    data = (uint32*)memoryManager_allocate((w*h+2)*sizeof(unsigned int));
  //  if(!data) return NULL;

    switch(ptr[2]) {
        case 1:
            if(ptr[6]!=0 || ptr[4]!=0 || ptr[3]!=0 || (ptr[7]!=24 && ptr[7]!=32)) { memoryManager_free(data); }
            for(y=i=0; y<h; y++) {
                k = ((!o?h-y-1:y)*w);
                for(x=0; x<w; x++) {
                    j = ptr[m + k++]*(ptr[7]>>3) + 18;
                    data[2 + i++] = ((ptr[7]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                }
            }
            break;
        case 2:
            if(ptr[5]!=0 || ptr[6]!=0 || ptr[1]!=0 || (ptr[16]!=24 && ptr[16]!=32)) { memoryManager_free(data);  }
            for(y=i=0; y<h; y++) {
                j = ((!o?h-y-1:y)*w*(ptr[16]>>3));
                for(x=0; x<w; x++) {
                    data[2 + i++] = ((ptr[16]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                    j += ptr[16]>>3;
                }
            }
            break;
        case 9:
            if(ptr[6]!=0 || ptr[4]!=0 || ptr[3]!=0 || (ptr[7]!=24 && ptr[7]!=32)) { memoryManager_free(data);  }
            y = i = 0;
            for(x=0; x<w*h && m<length;) {
                k = ptr[m++];
                if(k > 127) {
                    k -= 127; x += k;
                    j = ptr[m++]*(ptr[7]>>3) + 18;
                    while(k--) {
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[7]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                    }
                } else {
                    k++; x += k;
                    while(k--) {
                        j = ptr[m++]*(ptr[7]>>3) + 18;
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[7]==32?ptr[j+3]:0xFF) << 24) | (ptr[j+2] << 16) | (ptr[j+1] << 8) | ptr[j];
                    }
                }
            }
            break;
        case 10:
            if(ptr[5]!=0 || ptr[6]!=0 || ptr[1]!=0 || (ptr[16]!=24 && ptr[16]!=32)) { memoryManager_free(data);  }
            y = i = 0;
            for(x=0; x<w*h && m<length;) {
                k = ptr[m++];
                if(k > 127) {
                    k -= 127; x += k;
                    while(k--) {
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[16]==32?ptr[m+3]:0xFF) << 24) | (ptr[m+2] << 16) | (ptr[m+1] << 8) | ptr[m];
                    }
                    m += ptr[16]>>3;
                } else {
                    k++; x += k;
                    while(k--) {
                        if(!(i%w)) { i=((!o?h-y-1:y)*w); y++; }
                        data[2 + i++] = ((ptr[16]==32?ptr[m+3]:0xFF) << 24) | (ptr[m+2] << 16) | (ptr[m+1] << 8) | ptr[m];
                        m += ptr[16]>>3;
                    }
                }
            }
            break;
        default:
            memoryManager_free(data); 
    }

this->pixels = &data[2];

    data[0] = w;
    data[1] = h;

    Redraw();
}

void TargaImage::Redraw() {
    // draw.
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            uint32 pixel = pixels[(j * h + i)];

            vga_putPixel(canvas, x + j, y + i, pixel);
        }
    }
}

void TargaImage::HandleUIEvent(void* eventData) {

}

void Widget::SetPosition(sint32 x, sint32 y) {
    this->x = x;
    this->y = y;
}

void Widget::AddChild(Widget* widget) {
    children->Add(widget);
}

void Widget::RemoveChild(Widget* widget) {
    children->Remove(widget);
}

void Widget::RedrawChildren()
{
        debug(LOGLEVEL_WARNING, "rdeware?");
    children->Reset();
        debug(LOGLEVEL_WARNING, "after reset?");

    do {
        Widget* child = children->Current();

        debug(LOGLEVEL_WARNING, "Child?");

        if(child != nullptr) {
            child->Redraw();
        }
        
    } while(children->Next());
}
