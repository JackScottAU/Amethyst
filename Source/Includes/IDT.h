#include <Registers.h>
#include <Types.h>

#ifndef _IDT_H
#define _IDT_H

	void interrupts_installEmptyIDT(void);
	void IDT_SetGate(uint8 Number, uint32 Address, uint16 GDTSelector, uint8 Flags);
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

#endif