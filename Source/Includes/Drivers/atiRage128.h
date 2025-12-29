/**
 *  Amethyst Operating System - Driver for ATI Rage 128 Series Graphics Cards.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  2D Capabilities:
 *  [X] 2D Framebuffer
 *  [-] Double Buffering
 *  [-] Hardware Sprites (Cursor)
 *  [-] Hardware Copy and Fill (Blitting)
*/

#include <Types.h>

#ifndef INCLUDES_ATIRAGE128_H_
#define INCLUDES_ATIRAGE128_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <deviceTree.h>
#include <Graphics/canvas.h>
#include <Drivers/pciBus.h>

deviceTree_Entry* atiRage128_initialise(pciBus_Entry* pciDetails);

/**
 * Contains all the information needed to set a video mode on most display adapters.
 */
typedef struct {
    char* name;

    /// @brief Number of horizontal pixels (columns) visible on the display.
    uint16 hRes;

    /// @brief Number of vertical pixels (rows/lines) visible on the display.
    uint16 vRes;

    /// @brief Number of pixels before the horizontal sync.
    uint16 hFront;

    /// @brief Pixel duration of the horizontal sync.
    uint16 hSync;

    /// @brief Number of pixels after the horizontal sync.
    uint16 hBack;

    /// @brief Horizontal Sync Polarity. 1 is positive.
    bool hSyncPolarity;

    /// @brief Number of lines before the vertical sync.
    uint16 vFront;

    /// @brief Duration of the vertical sync in lines.
    uint16 vSync;

    /// @brief Number of lines after the vertical sync.
    uint16 vBack;

    /// @brief Vertical sync polarity. 1 is positive.
    bool vSyncPolarity;

    /// @brief Pixel clock in Hz.
    uint32 pixelClock;

    /// @brief Number of bits per pixel.
    /// Currently supported values are 15 (16-bit access, 5 bits each for RGB with top bit zero) and 32 (32-bit access, 8 bits each for RGB with top byte zero).
    uint8 depth;
} VideoMode;

void atiRage128_setMode(VideoMode* mode);

Canvas* atiRage128_getCanvas();

void atiRage128_dumpCursorPos();

void atiRage128_dumpRegs(void (*putChar)(char));

//void atiRagePro_setMode(uint16 width, uint16 height);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_ATIRAGE128_H_
