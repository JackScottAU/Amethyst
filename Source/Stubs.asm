;Exceptions
global ISR_00
global ISR_01
global ISR_02
global ISR_03
global ISR_04
global ISR_05
global ISR_06
global ISR_07
global ISR_08
global ISR_09
global ISR_0A
global ISR_0B
global ISR_0C
global ISR_0D
global ISR_0E
global ISR_0F
global ISR_10
global ISR_11
global ISR_12
global ISR_13
global ISR_14
global ISR_15
global ISR_16
global ISR_17
global ISR_18
global ISR_19
global ISR_1A
global ISR_1B
global ISR_1C
global ISR_1D
global ISR_1E
global ISR_1F

;IRQs
global ISR_20
global ISR_21
global ISR_22
global ISR_23
global ISR_24
global ISR_25
global ISR_26
global ISR_27
global ISR_28
global ISR_29
global ISR_2A
global ISR_2B
global ISR_2C
global ISR_2D
global ISR_2E
global ISR_2F

;Kernel ISRs
global ISR_30
global ISR_31



;  0: Divide By Zero Exception
ISR_00:
    cli
    push byte 0
    push byte 0
    jmp ISR_Stub

;  1: Debug Exception
ISR_01:
    cli
    push byte 0
    push byte 1
    jmp ISR_Stub

;  2: Non Maskable Interrupt Exception
ISR_02:
    cli
    push byte 0
    push byte 2
    jmp ISR_Stub

;  3: Int 3 Exception
ISR_03:
    cli
    push byte 0
    push byte 3
    jmp ISR_Stub

;  4: INTO Exception
ISR_04:
    cli
    push byte 0
    push byte 4
    jmp ISR_Stub

;  5: Out of Bounds Exception
ISR_05:
    cli
    push byte 0
    push byte 5
    jmp ISR_Stub

;  6: Invalid Opcode Exception
ISR_06:
    cli
    push byte 0
    push byte 6
    jmp ISR_Stub

;  7: Coprocessor Not Available Exception
ISR_07:
    cli
    push byte 0
    push byte 7
    jmp ISR_Stub

;  8: Double Fault Exception (With Error Code!)
ISR_08:
    cli
    push byte 8
    jmp ISR_Stub

;  9: Coprocessor Segment Overrun Exception
ISR_09:
    cli
    push byte 0
    push byte 9
    jmp ISR_Stub

; 10: Bad TSS Exception (With Error Code!)
ISR_0A:
    cli
    push byte 10
    jmp ISR_Stub

; 11: Segment Not Present Exception (With Error Code!)
ISR_0B:
    cli
    push byte 11
    jmp ISR_Stub

; 12: Stack Fault Exception (With Error Code!)
ISR_0C:
    cli
    push byte 12
    jmp ISR_Stub

; 13: General Protection Fault Exception (With Error Code!)
ISR_0D:
    cli
    push byte 13
    jmp ISR_Stub

; 14: Page Fault Exception (With Error Code!)
ISR_0E:
    cli
    push byte 14
    jmp ISR_Stub

; 15: Reserved Exception
ISR_0F:
    cli
    push byte 0
    push byte 15
    jmp ISR_Stub

; 16: Floating Point Exception
ISR_10:
    cli
    push byte 0
    push byte 16
    jmp ISR_Stub

; 17: Alignment Check Exception
ISR_11:
    cli
    push byte 0
    push byte 17
    jmp ISR_Stub

; 18: Machine Check Exception
ISR_12:
    cli
    push byte 0
    push byte 18
    jmp ISR_Stub

; 19: Reserved
ISR_13:
    cli
    push byte 0
    push byte 19
    jmp ISR_Stub

; 20: Reserved
ISR_14:
    cli
    push byte 0
    push byte 20
    jmp ISR_Stub

; 21: Reserved
ISR_15:
    cli
    push byte 0
    push byte 21
    jmp ISR_Stub

; 22: Reserved
ISR_16:
    cli
    push byte 0
    push byte 22
    jmp ISR_Stub

; 23: Reserved
ISR_17:
    cli
    push byte 0
    push byte 23
    jmp ISR_Stub

; 24: Reserved
ISR_18:
    cli
    push byte 0
    push byte 24
    jmp ISR_Stub

; 25: Reserved
ISR_19:
    cli
    push byte 0
    push byte 25
    jmp ISR_Stub

; 26: Reserved
ISR_1A:
    cli
    push byte 0
    push byte 26
    jmp ISR_Stub

; 27: Reserved
ISR_1B:
    cli
    push byte 0
    push byte 27
    jmp ISR_Stub

; 28: Reserved
ISR_1C:
    cli
    push byte 0
    push byte 28
    jmp ISR_Stub

; 29: Reserved
ISR_1D:
    cli
    push byte 0
    push byte 29
    jmp ISR_Stub

; 30: Reserved
ISR_1E:
    cli
    push byte 0
    push byte 30
    jmp ISR_Stub

; 31: Reserved
ISR_1F:
    cli
    push byte 0
    push byte 31
    jmp ISR_Stub


ISR_20:
    cli
    push byte 0
    push byte 0x20
    jmp ISR_Stub

ISR_21:
    cli
    push byte 0
    push byte 0x21
    jmp ISR_Stub

ISR_22:
    cli
    push byte 0
    push byte 0x22
    jmp ISR_Stub

ISR_23:
    cli
    push byte 0
    push byte 0x23
    jmp ISR_Stub

ISR_24:
    cli
    push byte 0
    push byte 0x24
    jmp ISR_Stub

ISR_25:
    cli
    push byte 0
    push byte 0x25
    jmp ISR_Stub

ISR_26:
    cli
    push byte 0
    push byte 0x26
    jmp ISR_Stub

ISR_27:
    cli
    push byte 0
    push byte 0x27
    jmp ISR_Stub

ISR_28:
    cli
    push byte 0
    push byte 0x28
    jmp ISR_Stub

ISR_29:
    cli
    push byte 0
    push byte 0x29
    jmp ISR_Stub

ISR_2A:
    cli
    push byte 0
    push byte 0x2A
    jmp ISR_Stub

ISR_2B:
    cli
    push byte 0
    push byte 0x2B
    jmp ISR_Stub

ISR_2C:
    cli
    push byte 0
    push byte 0x2C
    jmp ISR_Stub

ISR_2D:
    cli
    push byte 0
    push byte 0x2D
    jmp ISR_Stub

ISR_2E:
    cli
    push byte 0
    push byte 0x2E
    jmp ISR_Stub

ISR_2F:
    cli
    push byte 0
    push byte 0x2F
    jmp ISR_Stub
    
ISR_30:
    cli
    push byte 0
    push byte 0x30
    jmp ISR_Stub

ISR_31:
    cli
    push byte 0
    push byte 0x31
    jmp ISR_Stub



; We call a C function in here. We need to let the assembler know
; that '_fault_handler' exists in another file
extern ISR_Handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
ISR_Stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, ISR_Handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret