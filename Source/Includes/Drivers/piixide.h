/**
 *  Amethyst Operating System - Driver for PCI IDE Controllers.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_PIIXIDE_H_
#define INCLUDES_PIIXIDE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Types.h>
#include <deviceTree.h>
#include <Drivers/pciBus.h>

/**
 * Only thing this does is list resources. This is actually a generic PCI IDE controller, not PIIX specific.
 */
deviceTree_Entry* piixide_initialise(pciBus_Entry* device);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_PIIXIDE_H_
