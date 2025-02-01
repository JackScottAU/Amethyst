#include <StandardIO.hpp>
#include <Types.h>
#include <Structures/linkedlist.hpp>

#ifndef INCLUDES_DEVICE_HPP_
#define INCLUDES_DEVICE_HPP_

/**
 * An abstract class representing a hardware device. Calling initialise will set up the device (and any children managed by this device).
 */
class Device {
 public:
    virtual void Initialise() = 0;
    virtual void PrintDeviceDetails(StandardIO* stdio, bool verbose, uint16 depth) = 0;
    virtual LinkedList<Device*>* GetChildren() = 0;
};

#endif  // INCLUDES_DEVICE_HPP_
