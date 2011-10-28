#include <Registers.h>

void IDT_Init(void);
void IDT_SetGate(unsigned char Number, unsigned long Address, unsigned short Selector, unsigned char Flags);
void ISRs_Install(void);

#define idt_disableInts()	__asm__ volatile("cli")
#define idt_enableInts()	__asm__ volatile("sti")

void ISR_Handler(struct Registers_S *Registers);

void idt_remapPICs(unsigned char baseIRQ);
