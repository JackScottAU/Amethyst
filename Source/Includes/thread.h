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
    void* process_control_block; // 0
    uint32 kernel_stack_top; // 4
    uint32 cr3; // 8 - move this to process block when we get that far

} thread_control_block;

void initialise_multitasking(); // current task ends up current_task_tcb
void switch_to_task(thread_control_block *next_thread);
thread_control_block* new_task(void (* callback)(), thread_control_block* currentTask);

// https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial


#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_THREAD_H_
