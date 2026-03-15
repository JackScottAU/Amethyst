/**
 *  Amethyst Operating System - Threading and scheduling functions.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <thread.h>
#include <memoryManager.h>
#include <debug.h>
#include <Clock.h>

extern void* boot_page_directory;

// Current TCB for this CPU.
thread_control_block* current_task_TCB;

scheduler_entry* scheduler_head;

// TODO(JackScottAU): make this thread-safe.
bool schedulerEnabled = false;

thread_control_block* new_task(void (* callback)(), thread_control_block* currentTask) {
    debug(LOGLEVEL_INFO, "Creating new thread...");

    thread_control_block* tcb = (thread_control_block*)memoryManager_allocate(sizeof(thread_control_block));

    uint32* stack = (uint32*)memoryManager_allocate(sizeof(uint32) * 1024);     // 4KiB stack (one page). - later this will be an actual page allocation.

    debug(LOGLEVEL_DEBUG, "stack: %h", stack);

    // Set up the initial stack frame.
    stack[1023] = (uint32)callback;         // EIP
    stack[1022] = 0;                        // EBX
    stack[1021] = 0;                        // ESI
    stack[1020] = 0;                        // EDI
    stack[1019] = (uint32)(stack + 1023);   // EBP

    tcb->kernel_stack_top = (void*)(stack + 1019);
    tcb->cr3 = currentTask->cr3;

    // Add to the scheduler queue.
    scheduler_entry* entry = (scheduler_entry*)memoryManager_allocate(sizeof(scheduler_entry));
    entry->thread = tcb;
    entry->prev = scheduler_head;
    entry->next = scheduler_head->next;
    scheduler_head->next->prev = entry;
    scheduler_head->next = entry;

    return tcb;
}

thread_control_block* initialise_multitasking() {
    current_task_TCB = memoryManager_allocate(sizeof(thread_control_block));

    current_task_TCB->cr3 = (uint32) &boot_page_directory - 0xC0000000;
    current_task_TCB->process_control_block = NULL;

    // Set up the linked list for thread control block storage.
    scheduler_head = memoryManager_allocate(sizeof(scheduler_entry));
    scheduler_head->thread = current_task_TCB;
    scheduler_head->prev = scheduler_head;
    scheduler_head->next = scheduler_head;

    return current_task_TCB;
}

void thread_startScheduler() {
    schedulerEnabled = true;
}

void thread_stopScheduler() {
    schedulerEnabled = false;
}

void scheduler() {
    if (schedulerEnabled) {
       // thread_control_block* nextThread = current_task_TCB->nextThread;
        thread_control_block* nextThread = scheduler_head->next->thread;
        scheduler_head = scheduler_head->next;

        if (nextThread != NULL) {
            switch_to_task(nextThread);
        }
    }
}
