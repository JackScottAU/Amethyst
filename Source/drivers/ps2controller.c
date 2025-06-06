/**
 *  Amethyst Operating System - Driver for 8042 PS/2 Controller.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  This code will initialise the controller, determine what devices are plugged into it, and call the drivers 
 *  responsible for initialising the connected devices.
*/

#include <portIO.h>
#include <memoryManager.h>
#include <deviceTree.h>
#include <stream.h>
#include <debug.h>

#include <Drivers/ps2controller.h>
#include <Drivers/keyboard.h>
#include <Drivers/mouse.h>

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

uint8 ps2controller_getConfigurationByte(void);
void ps2controller_setConfigurationByte(uint8 configByte);

void ps2controller_enableInterrupts(void);
void ps2controller_disableInterrupts(void);

/**
 * 
 */
deviceTree_Entry* ps2controller_initialise(void) {
    bool channel1Status = true;     // By default, channel one is available.
    bool channel2Status = false;    // By default, channel two is unavailable (assume a single-channel controller).

    // Resource: https://wiki.osdev.org/%228042%22_PS/2_Controller

    // Missing steps: 1 (usb init), 2 (determine controller exists) - these are done at the level above.

    // Step 1: Disable Devices & Flush Output (Read) Buffer
    portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_DISABLE_PORT1);
    portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_DISABLE_PORT2);
    portIO_read8(PS2CONTROLLER_DATAPORT);

    // Step 2: Perform Controller Self-Test
    portIO_write8(PS2CONTROLLER_CONTROLPORT, 0xAA);
    ps2controller_waitForRead();
    if (portIO_read8(PS2CONTROLLER_DATAPORT) != 0x55) {
        debug(LOGLEVEL_ERROR, "PS/2 Controller failed self-test.");
        return NULL;
    }

    // Step 3: Determine if there are two channels.
    uint8 configByte = ps2controller_getConfigurationByte();
    channel2Status = configByte & PS2CONTROLLER_CONFIG_DISABLE_PORT2;

    // Step 4: Perform Interface Tests
    if(channel1Status) {
        portIO_write8(PS2CONTROLLER_CONTROLPORT, 0xAB);
        ps2controller_waitForRead();
        if (portIO_read8(PS2CONTROLLER_DATAPORT) != 0x00) {
            debug(LOGLEVEL_WARNING, "PS/2 Controller channel one failed interface test.");
            channel1Status = false;
        }
    }

    if(channel2Status) {
        portIO_write8(PS2CONTROLLER_CONTROLPORT, 0xA9);
        ps2controller_waitForRead();
        if (portIO_read8(PS2CONTROLLER_DATAPORT) != 0x00) {
            debug(LOGLEVEL_WARNING, "PS/2 Controller channel two failed interface test.");
            channel2Status = false;
        }
    }

    // Step 5: Enable Devices.
    ps2controller_disableInterrupts();
    if(channel1Status) {
        portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_ENABLE_PORT1);
    }
    if(channel2Status) {
        portIO_write8(PS2CONTROLLER_CONTROLPORT, PS2CONTROLLER_COMMAND_ENABLE_PORT2);
    }

    // Step 6: Initialise child devices.
    deviceTree_Entry* parent = deviceTree_createDevice("PS/2 Controller", DEVICETREE_TYPE_OTHER, NULL);

    parent->ResourceCount = 1 + channel1Status + channel2Status;
    parent->Resources = memoryManager_allocate(sizeof(DeviceResource) * parent->ResourceCount);
    parent->Resources[0].Type = DEVICE_RESOURCETYPE_IO;
    parent->Resources[0].StartAddress = 0x60;
    parent->Resources[0].Length = 8;

    // TODO: add code to detect what is plugged in where, so we could have two keyboards or two mice or mouse on channel 1 and keyboard on channel 2.

    if(channel1Status) {
        deviceTree_addChild(parent, keyboard_initialise());
        parent->Resources[1].Type = DEVICE_RESOURCETYPE_IRQ;
        parent->Resources[1].Flags = 1;
    }

    if(channel2Status) {
        deviceTree_addChild(parent, mouse_initialise());
        parent->Resources[2].Type = DEVICE_RESOURCETYPE_IRQ;
        parent->Resources[2].Flags = 12;
    }

    ps2controller_enableInterrupts();

    debug(LOGLEVEL_INFO, "PS/2 Controller initialisation complete.");

    return parent;
}

void ps2controller_disableInterrupts() {
    debug(LOGLEVEL_DEBUG, "PS/2 Controller: Disabling interrupts...");

    uint8 configByte = ps2controller_getConfigurationByte();

    configByte = configByte & 0xF4;     // Disable interrupts on both devices.
    
    ps2controller_setConfigurationByte(configByte);

    debug(LOGLEVEL_DEBUG, "PS/2 Controller: Disabled interrupts.");
}

void ps2controller_enableInterrupts() {
    debug(LOGLEVEL_DEBUG, "PS/2 Controller: Enabling interrupts...");

    uint8 configByte = ps2controller_getConfigurationByte();
    
    configByte = configByte | 0x03;     // Enable interrupts on both devices.
    
    ps2controller_setConfigurationByte(configByte);

    debug(LOGLEVEL_DEBUG, "PS/2 Controller: Enabled interrupts.");
}

uint8 ps2controller_getConfigurationByte(void) {
    portIO_write8(PS2CONTROLLER_CONTROLPORT, 0x20);
    ps2controller_waitForRead();
    uint8 configByte = portIO_read8(PS2CONTROLLER_DATAPORT);

    debug(LOGLEVEL_DEBUG, "PS/2 Controller: Got configuration byte, value = %h", configByte);

    return configByte;
}

void ps2controller_setConfigurationByte(uint8 configByte) {
    portIO_write8(PS2CONTROLLER_CONTROLPORT, 0x60);
    ps2controller_waitForWrite();
    portIO_write8(PS2CONTROLLER_DATAPORT, configByte);

    debug(LOGLEVEL_DEBUG, "PS/2 Controller: Set configuration byte, value = %h", configByte);
}

void ps2controller_sendByteToDevice(uint8 channel, uint8 data) {
    if(channel == 2) {
        ps2controller_waitForWrite();
        portIO_write8(PS2CONTROLLER_CONTROLPORT, 0xD4); // next command to channel two.
    }

    ps2controller_waitForWrite();
    portIO_write8(PS2CONTROLLER_DATAPORT, data);
}

uint8 ps2controller_receiveByteFromDevice(uint8 channel) {
    ps2controller_waitForRead();
    return portIO_read8(PS2CONTROLLER_DATAPORT);
}

/**
 *  Waits until the PS/2 Controller data port can be succesfully read from.
*/
void ps2controller_waitForRead() {
    while (!portIO_read8(PS2CONTROLLER_CONTROLPORT) & 0x01) { }
}

/**
 *  Waits until the PS/2 Controller data port can be succesfully written to.
*/
void ps2controller_waitForWrite() {
    while (portIO_read8(PS2CONTROLLER_CONTROLPORT) & 0x02) { }
}
