void IDT_Init();
void IDT_SetGate(unsigned char Number, unsigned long Address, unsigned short Selector, unsigned char Flags);
void ISRs_Install();
void PIC_Remap(void);

#define IDT_DisableInts()	__asm__ volatile("cli")
#define IDT_EnableInts()	__asm__ volatile("sti")
