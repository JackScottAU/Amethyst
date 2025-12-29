/**
 *  Amethyst Operating System - Shell.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#include <shell.hpp>
#include <cpuid.hpp>
#include <stream.h>
#include <string.h>
#include <deviceTree.h>
#include <Clock.h>
#include <portIO.h>
#include <interrupts.h>
#include <Drivers/pciBus.h>
#include <GDT.h>
#include <debug.h>
#include <Drivers/atiRage128.h>

Shell::Shell(StandardIO* stdio) {
    this->stdio = stdio;
    this->commands = new LinkedList<ShellCommand*>();
}

void Shell::RegisterCommand(const char* commandString, uint32 (* callback)(StandardIO*)) {
    ShellCommand* command = new ShellCommand();
    command->callback = callback;
    command->command = (char*) commandString;

    commands->Add(command);
}

void Shell::Main() {
    while (1) {
        ProcessLine();
    }
}

void Shell::ProcessLine() {
    stdio->Print("> ");

    char* input = stdio->ReadLine(true);

    char** strings = string_split(input, ' ');

    int i = 0;
    while (strings[i] != NULL) {
     //   stdio->Print("string: %s\n", strings[i]);
        i++;
    }

    char* line = strings[0];

    debug(LOGLEVEL_DEBUG, "addr of line: %h", line);

    if (*line == 0) {
        debug(LOGLEVEL_WARNING, "No input to shell.");
        return;
    }

    string_toLower(line);

    if (string_compare(line, "get-devicetree") == 0) {
        bool verbose = false;

        if (strings[1] != NULL && string_compare(strings[1], "-v") == 0) {
            verbose = true;
        }

        deviceTree_print(stdio->stdout, verbose);
        return;
    }

    if (string_compare(line, "get-time") == 0) {
        stdio->Print("Time: %h\n", clock_uptime());
        return;
    }

    if (string_compare(line, "gfx-demo") == 0) {
        // Put something on the screen so we can see if it worked.
        uint32* fbmem = (uint32*) 0xFFC00000;
        for (int i = 0; i < 1024 * 768; i++) {
            fbmem[i] = 0x8888CCCC + i;
        }
        return;
    }

    if(string_compare(line, "ati-dump") == 0) {
        atiRage128_dumpRegs(stdio->stdout);
        return;
    }

    if (string_compare(line, "get-cpuinformation") == 0) {
        CPUID cpuid = CPUID();

        stdio->Print("Manufacturer: %s\n", cpuid.getManufacturerString());
        stdio->Print("Family: %h\n", cpuid.getFamily());
        stdio->Print("Model: %h\n", cpuid.getModel());
        stdio->Print("Stepping: %h\n", cpuid.getStepping());
        return;
    }

    // Triggers a CPU exception for testing the kernel panic screen.
    if (string_compare(line, "trigger-exception") == 0) {
        uint8* invalidMemoryAddress = (uint8*) 0xA00B8000;
        invalidMemoryAddress[0] = 0x00;
    }

    if (string_compare(line, "show-gdt") == 0) {
        stdio->Print("GDT Pointer:\t%h\n", &gdt_pointer);
        stdio->Print("GDT Address:\t%h\n", gdt_table);

        uint16 size = gdt_pointer.size;

        uint8 entries = (gdt_pointer.size + 1) / 8;

        stdio->Print("GDT Size:\t%d (%h bytes)\n\n", entries, size);


        for (int i = 0; i < entries; i++) {
            uint32 base = (gdt_table[i].base_high << 24) + gdt_table[i].base_low;
            uint32 limit = (gdt_table[i].limit_high << 16) + gdt_table[i].limit_low;

            if (gdt_table[i].gran) {
                // 4K page granularity.
                limit = limit << 12 | 0xFFF;
            }

            stdio->Print("GDT Entry #%d:\t", i);

            if (limit == 0) {
                stdio->Print("NULL Segment\n\n");
                continue;
            }

            stdio->Print("Ring-%d ", gdt_table[i].DPL);
            if (gdt_table[i].code_data_segment) {
                if (gdt_table[i].gran) {
                    stdio->Print("Page-Aligned ");
                } else {
                    stdio->Print("Byte-Aligned ");
                }

                if (gdt_table[i].access & 0x8) {
                    stdio->Print("Code Segment\n");
                } else {
                    stdio->Print("Data Segment\n");
                }
            } else {
                stdio->Print("TSS Segment\n");
            }

            stdio->Print("\t\tBase:  %H\t", base);
            stdio->Print("Limit: %H\n\n", limit);
        }
        return;
    }

    if (string_compare(line, "shutdown") == 0) {
        stdio->Print("Shutting down...\n");
        interrupts_disableInterrupts();
        stdio->Print("It is now safe to turn off your PC.");
        haltCPU();
    }

    commands->Reset();
    do {
        ShellCommand* command = commands->Current();

        debug(LOGLEVEL_DEBUG, "Testing shell command: %s", command->command);

        string_toLower(command->command);

        if (string_compare(line, command->command) == 0) {
            uint32 result = command->callback(stdio);

            if (result != 0) {
                stdio->Print("Command result: %d\n", result);
            }

            return;
        }
    } while (commands->Next());

    // None of the built-in commands match the input.
    stdio->Print("Unknown command.\n");
}
