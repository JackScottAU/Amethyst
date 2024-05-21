/**
 * GDT.h - Functions interacting with the system GDT table.
 * Part of the Synergy Operating System.
 * Licensed under the ISC license.
 */

#ifndef INCLUDES_GDT_H_
#define INCLUDES_GDT_H_

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * Installs a basic GDT table.
     */
    void gdt_install();

    #ifdef	__cplusplus
}
#endif

#endif  // INCLUDES_GDT_H_
