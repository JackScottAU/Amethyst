# System Architecture - Brief Notes for Now

## Kernel / Userland

The kernel will be a microkernel. Drivers will exist in userland.

The kernel will present three different APIs to the userland:
 - Driver API
 - POSIX API
 - Amethyst Native API

All three will use the same ABI (kernel will provide all calls for all processes) but statically linked library will be different.

## Process Model

Session is a collection of processes sharing a security context (user, etc) and user interface (GUI / terminal session / etc).
Process is code/data/heap shared.
Thread is instruction pointer and stack.

Kernel will natively support threads.

Threads have priorities, processes have priorities, sessions have priorities (driver session has highest priority, for example).

## IPC

 * messages
 * shared memory

drivers are allowed to request shared memory with the HAL for use with devices.

## Memory

Paging is used. 0GB-3GB is userspace. 3GB-4GB is kernelspace. The first 4MB of physical memory is always mapped to the kernel.