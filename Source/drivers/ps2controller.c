/**
 *  Synergy Operating System - Driver for 8042 PS/2 Controller.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  This code will initialise the controller, determine what devices are plugged into it, and call the drivers 
 *  responsible for initialising the connected devices.
*/

#include <portIO.h>
#include <memoryManager.h>
#include <deviceTree.h>
#include <serial.h>

#define PS2CONTROLLER_DATAPORT      0x60
#define PS2CONTROLLER_CONTROLPORT   0x64    // reading = status register, writing = command register.

#define PS2CONTROLLER_COMMAND_DISABLE_PORT1     0xAD
#define PS2CONTROLLER_COMMAND_DISABLE_PORT2     0xA7
#define PS2CONTROLLER_COMMAND_ENABLE_PORT1     0xAE
#define PS2CONTROLLER_COMMAND_ENABLE_PORT2     0xA8

#define PS2CONTROLLER_CONFIG_ENABLE_INTERRUPTS_PORT1    0x01
#define PS2CONTROLLER_CONFIG_ENABLE_INTERRUPTS_PORT2    0x02
#define PS2CONTROLLER_CONFIG_DISABLE_PORT1              0x10
#define PS2CONTROLLER_CONFIG_DISABLE_PORT2              0x20

void ps2controller_waitForRead(void);
void ps2controller_waitForWrite(void);

deviceTree_Entry* ps2controller_initialise()
{
    // Resource: https://wiki.osdev.org/%228042%22_PS/2_Controller

    // Missing steps: 1 (usb init), 2 (determine controller exists) - these are done at the level above.

    // Step 3: Disable Devices
    portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_DISABLE_PORT1);
    portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_DISABLE_PORT2);

    // Step 4: Flush Buffer
    portIO_read8(PS2CONTROLLER_DATAPORT);

    // Step 5: Set the Controller Configuration Byte.
    portIO_write8(PS2CONTROLLER_CONTROLPORT, 0x20);
    uint8 configByte = portIO_read8(PS2CONTROLLER_DATAPORT);
    bool haveDualChannelController = configByte & PS2CONTROLLER_CONFIG_DISABLE_PORT2;
    stream_printf(serial_writeChar, "ps2controller config: %h\n", configByte);
    configByte = configByte | 0x03;     // Enable interrupts on both devices.
   // configByte = configByte & 0x3F;    // Disable emulation mode on first port. Commeneted out because Hyper-V doesnt support scancode set 2.
    stream_printf(serial_writeChar, "ps2controller config: %h\n", configByte);

    portIO_write8(PS2CONTROLLER_CONTROLPORT, 0x60);
    ps2controller_waitForWrite();
    portIO_write8(PS2CONTROLLER_DATAPORT, configByte);

    // Step 6: Perform Controller Self-Test
    portIO_write8(PS2CONTROLLER_CONTROLPORT, 0xAA);
    ps2controller_waitForRead();
    uint8 ok = portIO_read8(PS2CONTROLLER_DATAPORT);

    stream_printf(serial_writeChar, "ps2controller ok: %h\n", ok);
    if(ok != 0x55)
    {
        return NULL;
    }

    // Step 7: Determine if there are two channels.

    // Step 8: Perform Interface Tests
    // TODO.

    // Step 9: Enable Devices.
    portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_ENABLE_PORT1);
    if(haveDualChannelController) {
        portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_ENABLE_PORT2);
    }
}

/**
 *  Waits until the PS/2 Controller data port can be succesfully read from.
*/
void ps2controller_waitForRead()
{
    while(!portIO_read8(PS2CONTROLLER_CONTROLPORT) & 0x01);
}

/**
 *  Waits until the PS/2 Controller data port can be succesfully written to.
*/
void ps2controller_waitForWrite()
{
    while(portIO_read8(PS2CONTROLLER_CONTROLPORT) & 0x02);
}
