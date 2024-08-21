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
            uint8* invalidMemoryAddress = (uint8*) 0xA00B8000;
            invalidMemoryAddress[0] = 0x00;
        }

        if(string_compare(line, "Fix-GDT") == 0) {
            gdt_table[5].base_low = 0x10;
            gdt_table[5].limit_low = 0x68;
            gdt_table[5].gran = 0;
            gdt_table[5].present = 1;
            gdt_table[5].big = 1;
            gdt_table[5].access = 0x9;
            continue;
        }

        if (string_compare(line, "show-gdt") == 0) {
            stream_printf(stdout, "GDT Pointer:\t%h\n", &gdt_pointer);
            stream_printf(stdout, "GDT Address:\t%h\n", gdt_table);

            uint16 size = gdt_pointer.size;

            uint8 entries = (gdt_pointer.size + 1) / 8;

            stream_printf(stdout, "GDT Size:\t%d (%h bytes)\n\n", entries, size);


            for (int i = 0; i < entries; i++) {

                uint32 base = (gdt_table[i].base_high << 24) + gdt_table[i].base_low;
                uint32 limit = (gdt_table[i].limit_high << 16) + gdt_table[i].limit_low;

                if (gdt_table[i].gran) {
                    // 4K page granularity.
                    limit = limit << 12 | 0xFFF;
                }

                stream_printf(stdout, "GDT Entry #%d:\t", i);

                if(limit == 0) {
                    stream_printf(stdout, "NULL Segment\n\n");
                    continue;
                }

                stream_printf(stdout, "Ring-%d ", gdt_table[i].DPL);
                if(gdt_table[i].code_data_segment) {
                    if(gdt_table[i].gran) {
                        stream_printf(stdout, "Page-Aligned ");
                    } else {
                        stream_printf(stdout, "Byte-Aligned ");
                    }

                    if(gdt_table[i].access & 0x8) {
                        stream_printf(stdout, "Code Segment\n");
                    } else {
                        stream_printf(stdout, "Data Segment\n");
                    }
                } else {
                    stream_printf(stdout, "TSS Segment\n");
                }
                

                stream_printf(stdout, "\t\tBase:  %H\t", base);
                stream_printf(stdout, "Limit: %H\n\n", limit);
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
