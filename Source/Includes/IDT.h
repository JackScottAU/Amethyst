#include <Registers.h>
#include <Types.h>

#ifndef _IDT_H
#define _IDT_H

	void interrupts_installEmptyIDT(void);
	void interrupts_setGate(uint8 Number, uint32 Address, uint16 GDTSelector, uint8 Flags);
	void interrupts_installISRs(void);

	void interrupts_initialise(void);
	
	void interrupts_disableInterrupts(void);
	void interrupts_enableInterrupts(void);
	
	void ISR_Handler(struct Registers_S *Registers);

	void interrupts_remap8259s(uint8 baseIRQ);

	//Defines whether the interrupt table entry is valid.
	#define IDT_FLAG_PRESENT	0x80

	//Minimum level for callers.
	#define IDT_FLAG_RING0		0x00
	#define IDT_FLAG_RING1		0x20
	#define IDT_FLAG_RING2		0x40
	#define IDT_FLAG_RING3		0x60

	//Type of gate.
	#define IDT_FLAG_TASKGATE	0x05
	#define IDT_FLAG_INTGATE16	0x06
	#define IDT_FLAG_INTGATE32	0x0E
	#define IDT_FLAG_TRAPGATE16	0x07
	#define IDT_FLAG_TRAPGATE32	0x0F

	#define IDT_PIC1_BASEADDRESS 0x20
#define IDT_PIC2_BASEADDRESS 0xA0

	void interrupts_ISR_00();
void interrupts_ISR_01();
void interrupts_ISR_02();
void interrupts_ISR_03();
void interrupts_ISR_04();
void interrupts_ISR_05();
void interrupts_ISR_06();
void interrupts_ISR_07();
void interrupts_ISR_08();
void interrupts_ISR_09();
void interrupts_ISR_0A();
void interrupts_ISR_0B();
void interrupts_ISR_0C();
void interrupts_ISR_0D();
void interrupts_ISR_0E();
void interrupts_ISR_0F();
void interrupts_ISR_10();
void interrupts_ISR_11();
void interrupts_ISR_12();
void interrupts_ISR_13();
void interrupts_ISR_14();
void interrupts_ISR_15();
void interrupts_ISR_16();
void interrupts_ISR_17();
void interrupts_ISR_18();
void interrupts_ISR_19();
void interrupts_ISR_1A();
void interrupts_ISR_1B();
void interrupts_ISR_1C();
void interrupts_ISR_1D();
void interrupts_ISR_1E();
void interrupts_ISR_1F();

//IRQs
void interrupts_ISR_20();
void interrupts_ISR_21();
void interrupts_ISR_22();
void interrupts_ISR_23();
void interrupts_ISR_24();
void interrupts_ISR_25();
void interrupts_ISR_26();
void interrupts_ISR_27();
void interrupts_ISR_28();
void interrupts_ISR_29();
void interrupts_ISR_2A();
void interrupts_ISR_2B();
void interrupts_ISR_2C();
void interrupts_ISR_2D();
void interrupts_ISR_2E();
void interrupts_ISR_2F();

#endif