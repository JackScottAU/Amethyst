#include <Types.h>

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
