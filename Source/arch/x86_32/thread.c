#include <thread.h>
#include <memoryManager.h>
#include <debug.h>
#include <Clock.h>

extern void* boot_page_directory;

// Current TCB for this CPU.
thread_control_block* current_task_TCB;

// TODO: make this thread-safe.
bool schedulerEnabled = false;

thread_control_block* new_task(void (* callback)(), thread_control_block* currentTask) {
    debug(LOGLEVEL_INFO, "Creating new thread...");

    thread_control_block* tcb = (thread_control_block*)memoryManager_allocate(sizeof(thread_control_block));

    uint32* stack = (uint32*)memoryManager_allocate(sizeof(uint32) * 1024); // 4KiB stack (one page). - later this will be an actual page allocation.

    debug(LOGLEVEL_DEBUG, "stack: %h", stack);

    // Set up the initial stack frame.
    stack[1023] = (uint32)callback; // EIP
    stack[1022] = 0; // EBX;
    stack[1021] = 0; // ESI;
    stack[1020] = 0; // EDI;
    stack[1019] = (uint32)(stack + 1023); // EBP

    tcb->kernel_stack_top = (void*)(stack + 1019);
    tcb->cr3 = currentTask->cr3;

    return tcb;
}

thread_control_block* initialise_multitasking() {
    current_task_TCB = memoryManager_allocate(sizeof(thread_control_block));

    current_task_TCB->cr3 = (uint32) &boot_page_directory - 0xC0000000;
    current_task_TCB->process_control_block = NULL;

    // TODO: set up the linked lists for thread control block storage.

    return current_task_TCB;
}

void thread_startScheduler() {
    schedulerEnabled = true;
}

void thread_stopScheduler() {
    schedulerEnabled = false;
}

void scheduler() {
    if(schedulerEnabled) {
        thread_control_block* nextThread = current_task_TCB->nextThread;

        if(nextThread != NULL) {
        //    debug(LOGLEVEL_DEBUG, "We would switch task here...");
            switch_to_task(nextThread);
        }
    }
}