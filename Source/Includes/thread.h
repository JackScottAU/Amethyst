/**
 *  Amethyst Operating System - Threading and Multitasking
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_THREAD_H_
#define INCLUDES_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Types.h>

typedef struct  {
    void* process_control_block;
    uint32 kernel_stack_top;
    uint32 cr3;

} thread_control_block;

// https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial


#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_THREAD_H_
