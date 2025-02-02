#include <Graphics/canvas.h>

class TargaImage {
    public:
    TargaImage(uint8* ptr, uint32 length);

    void Draw(Canvas* canvas, uint32 x, uint32 y);

    private:
        uint32 width;
        uint32 height;
        uint32* pixels;
};
