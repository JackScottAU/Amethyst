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
#include <vgaConsole.h>

Shell::Shell(StandardIO* stdio) {
    this->stdio = stdio;
}

void Shell::Main() {
    while (1) {
        stdio->Print("> ");

        char* line = stdio->ReadLine(true);

        if (string_compare(line, "Get-DeviceTree") == 0) {
            deviceTree_print(vgaConsole_putChar, true);
            continue;
        }

        if (string_compare(line, "Get-Time") == 0) {
            stdio->Print("Time: %h\n", clock_uptime());
            continue;
        }

        if (string_compare(line, "Get-CpuInformation") == 0) {
            CPUID cpuid = CPUID();

            stdio->Print("Manufacturer: %s\n", cpuid.getManufacturerString());
            stdio->Print("Family: %h\n", cpuid.getFamily());
            stdio->Print("Model: %h\n", cpuid.getModel());
            stdio->Print("Stepping: %h\n", cpuid.getStepping());
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

        if (string_compare(line, "Show-GDT") == 0) {
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
            continue;
        }

        if (string_compare(line, "Shutdown") == 0) {
            stdio->Print("Shutting down...\n");
            interrupts_disableInterrupts();
            stdio->Print("It is now safe to turn off your PC.");
            haltCPU();
            break;
        }

        if (string_compare(line, "Get-PciDetails") == 0) {
            pci_printBuses(vgaConsole_putChar);
            continue;
        }

        // None of the built-in commands match the input.
        stdio->Print("Unknown command.\n");
    }
}
