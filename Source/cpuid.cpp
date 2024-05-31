#include <cpuid.hpp>
#include <cpuid.h>
#include <memoryManager.h>

CPUID::CPUID() {
    unsigned int eaxvar, ebxvar, ecxvar, edxvar;
    __get_cpuid(0, &eaxvar, &ebxvar, &ecxvar, &edxvar);

    maxLevel = eaxvar;
}

uint8* CPUID::getManufacturerString()
{
    uint8* string = (uint8*) memoryManager_allocate(13);
    string[12] = 0; // null terminate.

    unsigned int eaxvar, ebxvar, ecxvar, edxvar;
    __get_cpuid(0, &eaxvar, &ebxvar, &ecxvar, &edxvar);

    string[0] = ((uint8*)&ebxvar)[0];
    string[1] = ((uint8*)&ebxvar)[1];
    string[2] = ((uint8*)&ebxvar)[2];
    string[3] = ((uint8*)&ebxvar)[3];
    
    string[4] = ((uint8*)&edxvar)[0];
    string[5] = ((uint8*)&edxvar)[1];
    string[6] = ((uint8*)&edxvar)[2];
    string[7] = ((uint8*)&edxvar)[3];
    
    string[8] = ((uint8*)&ecxvar)[0];
    string[9] = ((uint8*)&ecxvar)[1];
    string[10] = ((uint8*)&ecxvar)[2];
    string[11] = ((uint8*)&ecxvar)[3];

    return string;
}

uint8 CPUID::getFamily()
{
    if(maxLevel < 1)
        return 0; // we don't have exception support yet.

    unsigned int eaxvar, ebxvar, ecxvar, edxvar;
    __get_cpuid(1, &eaxvar, &ebxvar, &ecxvar, &edxvar);

    uint32 family = (eaxvar >> 8) & 0xF;
    uint32 extendedFamily = (eaxvar >> 20) & 0xFF;

    if(family == 15)
    {
        return family + extendedFamily;
    }
    else
    {
        return family;
    }
}

uint8 CPUID::getModel()
{
    if(maxLevel < 1)
        return 0; // we don't have exception support yet.

    unsigned int eaxvar, ebxvar, ecxvar, edxvar;
    __get_cpuid(1, &eaxvar, &ebxvar, &ecxvar, &edxvar);

    uint32 family = (eaxvar >> 8) & 0xF;
    uint32 model = (eaxvar >> 4) & 0xF;
    uint32 extendedModel = (eaxvar >> 16) & 0xF;

    if(family == 6 || family == 15)
    {
        return model + (extendedModel << 4);
    }
    else
    {
        return model;
    }
}

uint8 CPUID::getStepping()
{
    if(maxLevel < 1)
        return 0; // we don't have exception support yet.

    unsigned int eaxvar, ebxvar, ecxvar, edxvar;
    __get_cpuid(1, &eaxvar, &ebxvar, &ecxvar, &edxvar);

    return eaxvar & 0x0F;
}
