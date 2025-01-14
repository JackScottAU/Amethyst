/**
 *  Amethyst Operating System - Threading and Context Switching Code.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
 * 
 *  See https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial
*/

#ifndef INCLUDES_THREAD_H_
#define INCLUDES_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Types.h>

/**
 * The data stored in the kernel about a thread, including information needed for context switching, time accounting, etc.
 */
typedef struct thread_control_block_S {
    // The location of the first two elements in this structure (CR3 and ESP) is critical, as they are referenced by offset from taskswitch.asm.

    /**
     * TODO(JackScottAU): Rename this to something processor agnostic, like "KernelStackPointer".
     */
    uint32 kernel_stack_top; // 0

    /**
     * The value of the CR3 register for this thread. This should be a pointer to the start of the thread's page directory.
     * In an ideal world, this would be in the process control block. However, we put it here as it is much easier to get to.
     * 
     * TODO(JackScottAU): Rename this to something processor agnostic, like "PageDirectoryPointer".
     */
    uint32 cr3; // 4 - move this to process block when we get that far

    void* process_control_block; // 8

    /**
     * The next thread in the queue.
     */
    struct thread_control_block_S* nextThread;
 //   char threadName[16]; // 16 characters to store the threads name in?
} thread_control_block;

thread_control_block* initialise_multitasking(); // current task ends up current_task_tcb
void switch_to_task(thread_control_block* next_thread);
thread_control_block* new_task(void (* callback)(), thread_control_block* currentTask);

void thread_startScheduler();
void thread_stopScheduler();
void scheduler();

// TODO: move scheduler into the non-arch area of the kernel.

#ifdef __cplusplus
}
#endif

#endif  // INCLUDES_THREAD_H_
