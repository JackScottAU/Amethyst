
#ifndef INCLUDES_PIIXIDE_H_
#define INCLUDES_PIIXIDE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Types.h>
#include <deviceTree.h>

/**
 * Only thing this does is list resources. This is actually a generic PCI IDE controller, not PIIX specific.
 */
deviceTree_Entry* piixide_initialise(uint32 bus, uint32 slot, uint32 function);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_PIIXIDE_H_
