/**
 *  Amethyst Operating System - CPUID stuff.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <Types.h>

#ifndef INCLUDES_CPUID_HPP_
#define INCLUDES_CPUID_HPP_

class CPUID {
 public:
    CPUID();

    uint8* getManufacturerString();

    uint8 getFamily();
    uint8 getModel();
    uint8 getStepping();

 private:
    uint32 maxLevel;
};

#endif  // INCLUDES_CPUID_HPP_
