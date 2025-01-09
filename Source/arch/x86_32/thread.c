#include <thread.h>
#include <memoryManager.h>
#include <debug.h>
#include <Clock.h>

extern void* boot_page_directory;

extern thread_control_block* current_task_TCB;

clock_timerRequest* schedulerJob;

thread_control_block* new_task(void (* callback)(), thread_control_block* currentTask) {
    debug(LOGLEVEL_INFO, "Creating new thread...\n");

    thread_control_block* tcb = memoryManager_allocate(sizeof(thread_control_block));

    uint32* stack = memoryManager_allocate(sizeof(uint32) * 1024); // 4KiB stack.

    debug(LOGLEVEL_DEBUG, "stack: %h\n", stack);

    stack[1023] = (uint32)callback; // EIP
    stack[1022] = 0; // EBX;
    stack[1021] = 0; // ESI;
    stack[1020] = 0; // EDI;
    stack[1019] = (uint32)(stack + 1023); // EBP

    tcb->kernel_stack_top = (uint32)(stack + 1019);
    tcb->cr3 = currentTask->cr3;

    uint32* kst = tcb->kernel_stack_top;
    uint32 ip = kst[0];

    debug(LOGLEVEL_DEBUG, "top of stack: %h", kst);
    debug(LOGLEVEL_DEBUG, "callback: %h, ip: %h", callback, ip);

    return tcb;
}

void initialise_multitasking() {
    current_task_TCB->cr3 = (uint32) &boot_page_directory - 0xC0000000;
    current_task_TCB->process_control_block = NULL;
}

void thread_startScheduler() {

    schedulerJob = clock_addRepeatRequest(0, 100, scheduler);
}

void thread_stopScheduler() {
    clock_deleteTimerRequest(schedulerJob);
}

void scheduler() {
    thread_control_block* nextThread = current_task_TCB->nextThread;

    if(nextThread != NULL) {
        switch_to_task(nextThread);
    }
}
