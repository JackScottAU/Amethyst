#include <portIO.h>
#include <Clock.h>
#include <Registers.h>
#include <interrupts.h>
#include <memoryManager.h>

interrupts_handlerCallback* interrupts_callbacks = (interrupts_handlerCallback*) 0x00;

/**
 * @param Registers The state of the registers on the stack provided by the ISR stub.
 */
void interrupts_handler(struct Registers_S *Registers)
{
	//Variable Declarations.
	//unsigned char temp = 0;
	
	//If it is an exception.
	if(Registers->IntNum <= 0x1F)
	{		
		__asm__("cli");
		__asm__("hlt");
	}
	
	//If it is an IRQ.
	if((Registers->IntNum >= 0x20) && (Registers->IntNum <= 0x2F))
	{
		switch(Registers->IntNum)
		{
		case 0x20:	//Programmable Interval Timer
//			clock_handler_PIC(0);
			break;
		
		case 0x21:	//Keyboard
			//temp = readByte(0x60);
			break;
		
		case 0x24:	//COM1
			break;
		
		default:	//As yet unhandled things.
			break;
		}
		
		//Send EOI to 8259 controller.
		if(Registers->IntNum >= 0x28)
		{
			//If it is the slave, send it to that.
			writeByte(0xA0, 0x20);
		}
		
		//But send it to the master no matter what.
		writeByte(0x20, 0x20);
	}
	
	interrupts_handlerCallback* current = interrupts_callbacks;
	interrupts_handlerCallback* oldCurrent = (interrupts_handlerCallback*) 0;
	while(current)
	{
		//Perform check.
		if((current->interruptNumber==Registers->IntNum))
		{
			//We have a shot!
			void (*foo)();
			foo = current->funcToCall;
			(*foo)(Registers->IntNum, current->arbitraryNumber); //Call the function.
		}
		oldCurrent = current;
		current = current->next;
	}
	oldCurrent++;
}

interrupts_handlerCallback* interrupts_addHandler(uint8 interruptNumber, uint32 arbitraryNumber, void (* funcToCall)(uint32 arbitraryNumber))
{
	interrupts_handlerCallback* request = memoryManager_allocate(sizeof(interrupts_handlerCallback));
	
	request->interruptNumber = interruptNumber;
	request->arbitraryNumber = arbitraryNumber;
	request->funcToCall = funcToCall;
	
	//Add to queue.
	//Do magic to try and find where to place this in the list.
	if(!interrupts_callbacks)
	{
		request->next = (interrupts_handlerCallback*) 0x00;
		interrupts_callbacks = request;
	} else {
		//Add it to the front of the list. We'll figure out sorting it later.
		request->next = interrupts_callbacks;
		interrupts_callbacks = request;
	}
	
	return(request);
}

void interrupts_removeHandler(interrupts_handlerCallback* request)
{
	//TODO.
	interrupts_handlerCallback* current = interrupts_callbacks;
	interrupts_handlerCallback* oldCurrent = 0;
	
	if(current==request)
	{
		interrupts_callbacks = current->next;
		memoryManager_free(current);
	}
	
	while(current)
	{
		//Perform check.
		if(current==request)
		{
			//Remove it from the list.
			interrupts_handlerCallback*  temp = current;

			oldCurrent->next = current->next;
			current = oldCurrent->next;

			memoryManager_free(temp);
		} else {
			oldCurrent = current;
			current = current->next;
		}
	}
}