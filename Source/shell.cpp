#include <shell.hpp>
#include <cpuid.hpp>
#include <stream.h>
#include <string.h>
#include <deviceTree.h>
#include <Clock.h>
#include <portIO.h>
#include <interrupts.h>
#include <pciBus.h>
#include <GDT.h>

Shell::Shell(void (*stdout)(char), char (*stdin)(void)) {
    this->stdin = stdin;
    this->stdout = stdout;
}

void Shell::Main() {
    while (1) {
        stream_printf(stdout, "> ");

        char* line = stream_readLine(true);

        if (string_compare(line, "Get-DeviceTree") == 0) {
            deviceTree_print(stdout, true);
            continue;
        }

        if (string_compare(line, "Get-Time") == 0) {
            stream_printf(stdout, "Time: %h\n", clock_uptime());
            continue;
        }

        if (string_compare(line, "Get-CpuInformation") == 0) {
            CPUID cpuid = CPUID();

            stream_printf(stdout, "Manufacturer: %s\n", cpuid.getManufacturerString());
            stream_printf(stdout, "Family: %h\n", cpuid.getFamily());
            stream_printf(stdout, "Model: %h\n", cpuid.getModel());
            stream_printf(stdout, "Stepping: %h\n", cpuid.getStepping());
            continue;
        }

        // Triggers a CPU exception for testing the kernel panic screen.
        if (string_compare(line, "Trigger-Exception") == 0) {
            uint16* vgaConsole_videoMemory    = (uint16*) 0xA00B8000;
            stream_printf(stdout, "asdf", vgaConsole_videoMemory[0]);
        }

        if (string_compare(line, "Show-GDT") == 0) {
            stream_printf(stdout, "gdt address: %h\n", gdt_table);

            for (int i = 0; i < 6; i++) {
                uint32 base = (gdt_table[i].base_high << 24) + gdt_table[i].base_low;
                uint32 limit = (gdt_table[i].limit_high << 16) + gdt_table[i].limit_low;

                stream_printf(stdout, "Base: %h\n", base);
                stream_printf(stdout, "Limit: %h\n", limit);

                stream_printf(stdout, "Code: %h\n", gdt_table[i].code);
                stream_printf(stdout, "DPL: %h\n\n", gdt_table[i].DPL);
            }
            continue;
        }

        if (string_compare(line, "Shutdown") == 0) {
            stream_printf(stdout, "Shutting down...\n");
            interrupts_disableInterrupts();
            stream_printf(stdout, "It is now safe to turn off your PC.");
            haltCPU();
            break;
        }

        if (string_compare(line, "Get-PciDetails") == 0) {
            pci_printBuses(stdout);
            continue;
        }

        // None of the built-in commands match the input.
        stream_printf(stdout, "Unknown command.\n");
    }
}
