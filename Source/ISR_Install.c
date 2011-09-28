#include <IDT.h>

void ISR_00();
void ISR_01();
void ISR_02();
void ISR_03();
void ISR_04();
void ISR_05();
void ISR_06();
void ISR_07();
void ISR_08();
void ISR_09();
void ISR_0A();
void ISR_0B();
void ISR_0C();
void ISR_0D();
void ISR_0E();
void ISR_0F();
void ISR_10();
void ISR_11();
void ISR_12();
void ISR_13();
void ISR_14();
void ISR_15();
void ISR_16();
void ISR_17();
void ISR_18();
void ISR_19();
void ISR_1A();
void ISR_1B();
void ISR_1C();
void ISR_1D();
void ISR_1E();
void ISR_1F();

//IRQs
void ISR_20();
void ISR_21();
void ISR_22();
void ISR_23();
void ISR_24();
void ISR_25();
void ISR_26();
void ISR_27();
void ISR_28();
void ISR_29();
void ISR_2A();
void ISR_2B();
void ISR_2C();
void ISR_2D();
void ISR_2E();
void ISR_2F();

//Kernel Functions
void ISR_30();
void ISR_31();

void ISRs_Install()
{
	IDT_SetGate(0x00, (unsigned)ISR_00, 0x08, 0x8E);
	IDT_SetGate(0x01, (unsigned)ISR_01, 0x08, 0x8E);
	IDT_SetGate(0x02, (unsigned)ISR_02, 0x08, 0x8E);
	IDT_SetGate(0x03, (unsigned)ISR_03, 0x08, 0x8E);
	IDT_SetGate(0x04, (unsigned)ISR_04, 0x08, 0x8E);
	IDT_SetGate(0x05, (unsigned)ISR_05, 0x08, 0x8E);
	IDT_SetGate(0x06, (unsigned)ISR_06, 0x08, 0x8E);
	IDT_SetGate(0x07, (unsigned)ISR_07, 0x08, 0x8E);
	IDT_SetGate(0x08, (unsigned)ISR_08, 0x08, 0x8E);
	IDT_SetGate(0x09, (unsigned)ISR_09, 0x08, 0x8E);
	IDT_SetGate(0x0A, (unsigned)ISR_0A, 0x08, 0x8E);
	IDT_SetGate(0x0B, (unsigned)ISR_0B, 0x08, 0x8E);
	IDT_SetGate(0x0C, (unsigned)ISR_0C, 0x08, 0x8E);
	IDT_SetGate(0x0D, (unsigned)ISR_0D, 0x08, 0x8E);
	IDT_SetGate(0x0E, (unsigned)ISR_0E, 0x08, 0x8E);
	IDT_SetGate(0x0F, (unsigned)ISR_0F, 0x08, 0x8E);
	IDT_SetGate(0x10, (unsigned)ISR_10, 0x08, 0x8E);
	IDT_SetGate(0x11, (unsigned)ISR_11, 0x08, 0x8E);
	IDT_SetGate(0x12, (unsigned)ISR_12, 0x08, 0x8E);
	IDT_SetGate(0x13, (unsigned)ISR_13, 0x08, 0x8E);
	IDT_SetGate(0x14, (unsigned)ISR_14, 0x08, 0x8E);
	IDT_SetGate(0x15, (unsigned)ISR_15, 0x08, 0x8E);
	IDT_SetGate(0x16, (unsigned)ISR_16, 0x08, 0x8E);
	IDT_SetGate(0x17, (unsigned)ISR_17, 0x08, 0x8E);
	IDT_SetGate(0x18, (unsigned)ISR_18, 0x08, 0x8E);
	IDT_SetGate(0x19, (unsigned)ISR_19, 0x08, 0x8E);
	IDT_SetGate(0x1A, (unsigned)ISR_1A, 0x08, 0x8E);
	IDT_SetGate(0x1B, (unsigned)ISR_1B, 0x08, 0x8E);
	IDT_SetGate(0x1C, (unsigned)ISR_1C, 0x08, 0x8E);
	IDT_SetGate(0x1D, (unsigned)ISR_1D, 0x08, 0x8E);
	IDT_SetGate(0x1E, (unsigned)ISR_1E, 0x08, 0x8E);
	IDT_SetGate(0x1F, (unsigned)ISR_1F, 0x08, 0x8E);
	IDT_SetGate(0x20, (unsigned)ISR_20, 0x08, 0x8E);
	IDT_SetGate(0x21, (unsigned)ISR_21, 0x08, 0x8E);
	IDT_SetGate(0x22, (unsigned)ISR_22, 0x08, 0x8E);
	IDT_SetGate(0x23, (unsigned)ISR_23, 0x08, 0x8E);
	IDT_SetGate(0x24, (unsigned)ISR_24, 0x08, 0x8E);
	IDT_SetGate(0x25, (unsigned)ISR_25, 0x08, 0x8E);
	IDT_SetGate(0x26, (unsigned)ISR_26, 0x08, 0x8E);
	IDT_SetGate(0x27, (unsigned)ISR_27, 0x08, 0x8E);
	IDT_SetGate(0x28, (unsigned)ISR_28, 0x08, 0x8E);
	IDT_SetGate(0x29, (unsigned)ISR_29, 0x08, 0x8E);
	IDT_SetGate(0x2A, (unsigned)ISR_2A, 0x08, 0x8E);
	IDT_SetGate(0x2B, (unsigned)ISR_2B, 0x08, 0x8E);
	IDT_SetGate(0x2C, (unsigned)ISR_2C, 0x08, 0x8E);
	IDT_SetGate(0x2D, (unsigned)ISR_2D, 0x08, 0x8E);
	IDT_SetGate(0x2E, (unsigned)ISR_2E, 0x08, 0x8E);
	IDT_SetGate(0x2F, (unsigned)ISR_2F, 0x08, 0x8E);
}
