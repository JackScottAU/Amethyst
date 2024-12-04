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
    this->commands = new LinkedList<ShellCommand*>();
}

void Shell::RegisterCommand(char* commandString, uint32 (* callback)(StandardIO*))
{
    ShellCommand* command = new ShellCommand();
    command->callback = callback;
    command->command = commandString;

    commands->Add(command);
}

void Shell::Main() {
    while (1) {
        ProcessLine();
    }
}

void Shell::ProcessLine() {
    stdio->Print("> ");

    char* line = stdio->ReadLine(true);

    if (string_compare(line, "Get-DeviceTree") == 0) {
        deviceTree_print(vgaConsole_putChar, true);
        return;
    }

    if (string_compare(line, "Get-Time") == 0) {
        stdio->Print("Time: %h\n", clock_uptime());
        return;
    }

    if (string_compare(line, "Get-CpuInformation") == 0) {
        CPUID cpuid = CPUID();

        stdio->Print("Manufacturer: %s\n", cpuid.getManufacturerString());
        stdio->Print("Family: %h\n", cpuid.getFamily());
        stdio->Print("Model: %h\n", cpuid.getModel());
        stdio->Print("Stepping: %h\n", cpuid.getStepping());
        return;
    }

    // Triggers a CPU exception for testing the kernel panic screen.
    if (string_compare(line, "Trigger-Exception") == 0) {
        uint8* invalidMemoryAddress = (uint8*) 0xA00B8000;
        invalidMemoryAddress[0] = 0x00;
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
        return;
    }

    if (string_compare(line, "Shutdown") == 0) {
        stdio->Print("Shutting down...\n");
        interrupts_disableInterrupts();
        stdio->Print("It is now safe to turn off your PC.");
        haltCPU();
    }

    if (string_compare(line, "Get-PciDetails") == 0) {
        pci_printBuses(vgaConsole_putChar);
        return;
    }

    commands->Reset();
    do {
        ShellCommand* command = commands->Current();

        if (string_compare(line, command->command) == 0) {
            uint32 result = command->callback(stdio);

            if(result != 0) {
                stdio->Print("Command result: %d\n", result);
            }
            
            return;
        }
    } while(commands->Next());

    // None of the built-in commands match the input.
    stdio->Print("Unknown command.\n");
}
