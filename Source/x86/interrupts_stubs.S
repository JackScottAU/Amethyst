#Exceptions
.global interrupts_ISR_00
.global interrupts_ISR_01
.global interrupts_ISR_02
.global interrupts_ISR_03
.global interrupts_ISR_04
.global interrupts_ISR_05
.global interrupts_ISR_06
.global interrupts_ISR_07
.global interrupts_ISR_08
.global interrupts_ISR_09
.global interrupts_ISR_0A
.global interrupts_ISR_0B
.global interrupts_ISR_0C
.global interrupts_ISR_0D
.global interrupts_ISR_0E
.global interrupts_ISR_0F
.global interrupts_ISR_10
.global interrupts_ISR_11
.global interrupts_ISR_12
.global interrupts_ISR_13
.global interrupts_ISR_14
.global interrupts_ISR_15
.global interrupts_ISR_16
.global interrupts_ISR_17
.global interrupts_ISR_18
.global interrupts_ISR_19
.global interrupts_ISR_1A
.global interrupts_ISR_1B
.global interrupts_ISR_1C
.global interrupts_ISR_1D
.global interrupts_ISR_1E
.global interrupts_ISR_1F

#IRQs
.global interrupts_ISR_20
.global interrupts_ISR_21
.global interrupts_ISR_22
.global interrupts_ISR_23
.global interrupts_ISR_24
.global interrupts_ISR_25
.global interrupts_ISR_26
.global interrupts_ISR_27
.global interrupts_ISR_28
.global interrupts_ISR_29
.global interrupts_ISR_2A
.global interrupts_ISR_2B
.global interrupts_ISR_2C
.global interrupts_ISR_2D
.global interrupts_ISR_2E
.global interrupts_ISR_2F

        .global interrupts_ISR_30
        .global interrupts_ISR_31
        .global interrupts_ISR_32
        .global interrupts_ISR_33
        .global interrupts_ISR_34
        .global interrupts_ISR_35
        .global interrupts_ISR_36
        .global interrupts_ISR_37
        .global interrupts_ISR_38
        .global interrupts_ISR_39
        .global interrupts_ISR_3A
        .global interrupts_ISR_3B
        .global interrupts_ISR_3C
        .global interrupts_ISR_3D
        .global interrupts_ISR_3E
        .global interrupts_ISR_3F
        .global interrupts_ISR_40
        .global interrupts_ISR_41
        .global interrupts_ISR_42
        .global interrupts_ISR_43
        .global interrupts_ISR_44
        .global interrupts_ISR_45
        .global interrupts_ISR_46
        .global interrupts_ISR_47
        .global interrupts_ISR_48
        .global interrupts_ISR_49
        .global interrupts_ISR_4A
        .global interrupts_ISR_4B
        .global interrupts_ISR_4C
        .global interrupts_ISR_4D
        .global interrupts_ISR_4E
        .global interrupts_ISR_4F
        .global interrupts_ISR_50
        .global interrupts_ISR_51
        .global interrupts_ISR_52
        .global interrupts_ISR_53
        .global interrupts_ISR_54
        .global interrupts_ISR_55
        .global interrupts_ISR_56
        .global interrupts_ISR_57
        .global interrupts_ISR_58
        .global interrupts_ISR_59
        .global interrupts_ISR_5A
        .global interrupts_ISR_5B
        .global interrupts_ISR_5C
        .global interrupts_ISR_5D
        .global interrupts_ISR_5E
        .global interrupts_ISR_5F
        .global interrupts_ISR_60
        .global interrupts_ISR_61
        .global interrupts_ISR_62
        .global interrupts_ISR_63
        .global interrupts_ISR_64
        .global interrupts_ISR_65
        .global interrupts_ISR_66
        .global interrupts_ISR_67
        .global interrupts_ISR_68
        .global interrupts_ISR_69
        .global interrupts_ISR_6A
        .global interrupts_ISR_6B
        .global interrupts_ISR_6C
        .global interrupts_ISR_6D
        .global interrupts_ISR_6E
        .global interrupts_ISR_6F
        .global interrupts_ISR_70
        .global interrupts_ISR_71
        .global interrupts_ISR_72
        .global interrupts_ISR_73
        .global interrupts_ISR_74
        .global interrupts_ISR_75
        .global interrupts_ISR_76
        .global interrupts_ISR_77
        .global interrupts_ISR_78
        .global interrupts_ISR_79
        .global interrupts_ISR_7A
        .global interrupts_ISR_7B
        .global interrupts_ISR_7C
        .global interrupts_ISR_7D
        .global interrupts_ISR_7E
        .global interrupts_ISR_7F

#  0 Divide By Zero Exception
interrupts_ISR_00: 
    cli
    push $0
    push $0
    jmp interrupts_ISR_Stub

#  1 Debug Exception
interrupts_ISR_01: 
    cli
    push $0
    push $1
    jmp interrupts_ISR_Stub

#  2 Non Maskable Interrupt Exception
interrupts_ISR_02: 
    cli
    push $0
    push $2
    jmp interrupts_ISR_Stub

#  3 Int 3 Exception
interrupts_ISR_03: 
    cli
    push $0
    push $3
    jmp interrupts_ISR_Stub

#  4 INTO Exception
interrupts_ISR_04: 
    cli
    push $0
    push $4
    jmp interrupts_ISR_Stub

#  5 Out of Bounds Exception
interrupts_ISR_05: 
    cli
    push $0
    push $5
    jmp interrupts_ISR_Stub

#  6 Invalid Opcode Exception
interrupts_ISR_06: 
    cli
    push $0
    push $6
    jmp interrupts_ISR_Stub

#  7 Coprocessor Not Available Exception
interrupts_ISR_07: 
    cli
    push $0
    push $7
    jmp interrupts_ISR_Stub

#  8 Double Fault Exception (With Error Code!)
interrupts_ISR_08: 
    cli
    push $8
    jmp interrupts_ISR_Stub

#  9 Coprocessor Segment Overrun Exception
interrupts_ISR_09: 
    cli
    push $0
    push $9
    jmp interrupts_ISR_Stub

# 10: Bad TSS Exception (With Error Code!)
interrupts_ISR_0A: 
    cli
    push $10
    jmp interrupts_ISR_Stub

# 11: Segment Not Present Exception (With Error Code!)
interrupts_ISR_0B: 
    cli
    push $11
    jmp interrupts_ISR_Stub

# 12: Stack Fault Exception (With Error Code!)
interrupts_ISR_0C: 
    cli
    push $12
    jmp interrupts_ISR_Stub

# 13: General Protection Fault Exception (With Error Code!)
interrupts_ISR_0D: 
    cli
    push $13
    jmp interrupts_ISR_Stub

# 14: Page Fault Exception (With Error Code!)
interrupts_ISR_0E: 
    cli
    push $14
    jmp interrupts_ISR_Stub

# 15: Reserved Exception
interrupts_ISR_0F: 
    cli
    push $0
    push $15
    jmp interrupts_ISR_Stub

# 16: Floating Point Exception
interrupts_ISR_10: 
    cli
    push $0
    push $16
    jmp interrupts_ISR_Stub

# 17: Alignment Check Exception
interrupts_ISR_11: 
    cli
    push $0
    push $17
    jmp interrupts_ISR_Stub

# 18: Machine Check Exception
interrupts_ISR_12: 
    cli
    push $0
    push $18
    jmp interrupts_ISR_Stub

# 19: Reserved
interrupts_ISR_13: 
    cli
    push $0
    push $19
    jmp interrupts_ISR_Stub

# 20: Reserved
interrupts_ISR_14: 
    cli
    push $0
    push $20
    jmp interrupts_ISR_Stub

# 21: Reserved
interrupts_ISR_15: 
    cli
    push $0
    push $21
    jmp interrupts_ISR_Stub

# 22: Reserved
interrupts_ISR_16: 
    cli
    push $0
    push $22
    jmp interrupts_ISR_Stub

# 23: Reserved
interrupts_ISR_17: 
    cli
    push $0
    push $23
    jmp interrupts_ISR_Stub

# 24: Reserved
interrupts_ISR_18: 
    cli
    push $0
    push $24
    jmp interrupts_ISR_Stub

# 25: Reserved
interrupts_ISR_19: 
    cli
    push $0
    push $25
    jmp interrupts_ISR_Stub

# 26: Reserved
interrupts_ISR_1A: 
    cli
    push $0
    push $26
    jmp interrupts_ISR_Stub

# 27: Reserved
interrupts_ISR_1B: 
    cli
    push $0
    push $27
    jmp interrupts_ISR_Stub

# 28: Reserved
interrupts_ISR_1C: 
    cli
    push $0
    push $28
    jmp interrupts_ISR_Stub

# 29: Reserved
interrupts_ISR_1D: 
    cli
    push $0
    push $29
    jmp interrupts_ISR_Stub

# 30: Reserved
interrupts_ISR_1E: 
    cli
    push $0
    push $30
    jmp interrupts_ISR_Stub

# 31: Reserved
interrupts_ISR_1F: 
    cli
    push $0
    push $31
    jmp interrupts_ISR_Stub


interrupts_ISR_20: 
    cli
    push $0
    push $0x20
    jmp interrupts_ISR_Stub

interrupts_ISR_21: 
    cli
    push $0
    push $0x21
    jmp interrupts_ISR_Stub

interrupts_ISR_22: 
    cli
    push $0
    push $0x22
    jmp interrupts_ISR_Stub

interrupts_ISR_23: 
    cli
    push $0
    push $0x23
    jmp interrupts_ISR_Stub

interrupts_ISR_24: 
    cli
    push $0
    push $0x24
    jmp interrupts_ISR_Stub

interrupts_ISR_25: 
    cli
    push $0
    push $0x25
    jmp interrupts_ISR_Stub

interrupts_ISR_26: 
    cli
    push $0
    push $0x26
    jmp interrupts_ISR_Stub

interrupts_ISR_27: 
    cli
    push $0
    push $0x27
    jmp interrupts_ISR_Stub

interrupts_ISR_28: 
    cli
    push $0
    push $0x28
    jmp interrupts_ISR_Stub

interrupts_ISR_29: 
    cli
    push $0
    push $0x29
    jmp interrupts_ISR_Stub

interrupts_ISR_2A: 
    cli
    push $0
    push $0x2A
    jmp interrupts_ISR_Stub

interrupts_ISR_2B: 
    cli
    push $0
    push $0x2B
    jmp interrupts_ISR_Stub

interrupts_ISR_2C: 
    cli
    push $0
    push $0x2C
    jmp interrupts_ISR_Stub

interrupts_ISR_2D: 
    cli
    push $0
    push $0x2D
    jmp interrupts_ISR_Stub

interrupts_ISR_2E: 
    cli
    push $0
    push $0x2E
    jmp interrupts_ISR_Stub

interrupts_ISR_2F: 
    cli
    push $0
    push $0x2F
    jmp interrupts_ISR_Stub

interrupts_ISR_30: 
        cli
        push $0
        push $0x30
        jmp interrupts_ISR_Stub

interrupts_ISR_31: 
        cli
        push $0
        push $0x31
        jmp interrupts_ISR_Stub

interrupts_ISR_32: 
        cli
        push $0
        push $0x32
        jmp interrupts_ISR_Stub

interrupts_ISR_33: 
        cli
        push $0
        push $0x33
        jmp interrupts_ISR_Stub

interrupts_ISR_34: 
        cli
        push $0
        push $0x34
        jmp interrupts_ISR_Stub

interrupts_ISR_35: 
        cli
        push $0
        push $0x35
        jmp interrupts_ISR_Stub

interrupts_ISR_36: 
        cli
        push $0
        push $0x36
        jmp interrupts_ISR_Stub

interrupts_ISR_37: 
        cli
        push $0
        push $0x37
        jmp interrupts_ISR_Stub

interrupts_ISR_38: 
        cli
        push $0
        push $0x38
        jmp interrupts_ISR_Stub

interrupts_ISR_39: 
        cli
        push $0
        push $0x39
        jmp interrupts_ISR_Stub

interrupts_ISR_3A: 
        cli
        push $0
        push $0x3A
        jmp interrupts_ISR_Stub

interrupts_ISR_3B: 
        cli
        push $0
        push $0x3B
        jmp interrupts_ISR_Stub

interrupts_ISR_3C: 
        cli
        push $0
        push $0x3C
        jmp interrupts_ISR_Stub

interrupts_ISR_3D: 
        cli
        push $0
        push $0x3D
        jmp interrupts_ISR_Stub

interrupts_ISR_3E: 
        cli
        push $0
        push $0x3E
        jmp interrupts_ISR_Stub

interrupts_ISR_3F: 
        cli
        push $0
        push $0x3F
        jmp interrupts_ISR_Stub

interrupts_ISR_40: 
        cli
        push $0
        push $0x40
        jmp interrupts_ISR_Stub

interrupts_ISR_41: 
        cli
        push $0
        push $0x41
        jmp interrupts_ISR_Stub

interrupts_ISR_42: 
        cli
        push $0
        push $0x42
        jmp interrupts_ISR_Stub

interrupts_ISR_43: 
        cli
        push $0
        push $0x43
        jmp interrupts_ISR_Stub

interrupts_ISR_44: 
        cli
        push $0
        push $0x44
        jmp interrupts_ISR_Stub

interrupts_ISR_45: 
        cli
        push $0
        push $0x45
        jmp interrupts_ISR_Stub

interrupts_ISR_46: 
        cli
        push $0
        push $0x46
        jmp interrupts_ISR_Stub

interrupts_ISR_47: 
        cli
        push $0
        push $0x47
        jmp interrupts_ISR_Stub

interrupts_ISR_48: 
        cli
        push $0
        push $0x48
        jmp interrupts_ISR_Stub

interrupts_ISR_49: 
        cli
        push $0
        push $0x49
        jmp interrupts_ISR_Stub

interrupts_ISR_4A: 
        cli
        push $0
        push $0x4A
        jmp interrupts_ISR_Stub

interrupts_ISR_4B: 
        cli
        push $0
        push $0x4B
        jmp interrupts_ISR_Stub

interrupts_ISR_4C: 
        cli
        push $0
        push $0x4C
        jmp interrupts_ISR_Stub

interrupts_ISR_4D: 
        cli
        push $0
        push $0x4D
        jmp interrupts_ISR_Stub

interrupts_ISR_4E: 
        cli
        push $0
        push $0x4E
        jmp interrupts_ISR_Stub

interrupts_ISR_4F: 
        cli
        push $0
        push $0x4F
        jmp interrupts_ISR_Stub

interrupts_ISR_50: 
        cli
        push $0
        push $0x50
        jmp interrupts_ISR_Stub

interrupts_ISR_51: 
        cli
        push $0
        push $0x51
        jmp interrupts_ISR_Stub

interrupts_ISR_52: 
        cli
        push $0
        push $0x52
        jmp interrupts_ISR_Stub

interrupts_ISR_53: 
        cli
        push $0
        push $0x53
        jmp interrupts_ISR_Stub

interrupts_ISR_54: 
        cli
        push $0
        push $0x54
        jmp interrupts_ISR_Stub

interrupts_ISR_55: 
        cli
        push $0
        push $0x55
        jmp interrupts_ISR_Stub

interrupts_ISR_56: 
        cli
        push $0
        push $0x56
        jmp interrupts_ISR_Stub

interrupts_ISR_57: 
        cli
        push $0
        push $0x57
        jmp interrupts_ISR_Stub

interrupts_ISR_58: 
        cli
        push $0
        push $0x58
        jmp interrupts_ISR_Stub

interrupts_ISR_59: 
        cli
        push $0
        push $0x59
        jmp interrupts_ISR_Stub

interrupts_ISR_5A: 
        cli
        push $0
        push $0x5A
        jmp interrupts_ISR_Stub

interrupts_ISR_5B: 
        cli
        push $0
        push $0x5B
        jmp interrupts_ISR_Stub

interrupts_ISR_5C: 
        cli
        push $0
        push $0x5C
        jmp interrupts_ISR_Stub

interrupts_ISR_5D: 
        cli
        push $0
        push $0x5D
        jmp interrupts_ISR_Stub

interrupts_ISR_5E: 
        cli
        push $0
        push $0x5E
        jmp interrupts_ISR_Stub

interrupts_ISR_5F: 
        cli
        push $0
        push $0x5F
        jmp interrupts_ISR_Stub

interrupts_ISR_60: 
        cli
        push $0
        push $0x60
        jmp interrupts_ISR_Stub

interrupts_ISR_61: 
        cli
        push $0
        push $0x61
        jmp interrupts_ISR_Stub

interrupts_ISR_62: 
        cli
        push $0
        push $0x62
        jmp interrupts_ISR_Stub

interrupts_ISR_63: 
        cli
        push $0
        push $0x63
        jmp interrupts_ISR_Stub

interrupts_ISR_64: 
        cli
        push $0
        push $0x64
        jmp interrupts_ISR_Stub

interrupts_ISR_65: 
        cli
        push $0
        push $0x65
        jmp interrupts_ISR_Stub

interrupts_ISR_66: 
        cli
        push $0
        push $0x66
        jmp interrupts_ISR_Stub

interrupts_ISR_67: 
        cli
        push $0
        push $0x67
        jmp interrupts_ISR_Stub

interrupts_ISR_68: 
        cli
        push $0
        push $0x68
        jmp interrupts_ISR_Stub

interrupts_ISR_69: 
        cli
        push $0
        push $0x69
        jmp interrupts_ISR_Stub

interrupts_ISR_6A: 
        cli
        push $0
        push $0x6A
        jmp interrupts_ISR_Stub

interrupts_ISR_6B: 
        cli
        push $0
        push $0x6B
        jmp interrupts_ISR_Stub

interrupts_ISR_6C: 
        cli
        push $0
        push $0x6C
        jmp interrupts_ISR_Stub

interrupts_ISR_6D: 
        cli
        push $0
        push $0x6D
        jmp interrupts_ISR_Stub

interrupts_ISR_6E: 
        cli
        push $0
        push $0x6E
        jmp interrupts_ISR_Stub

interrupts_ISR_6F: 
        cli
        push $0
        push $0x6F
        jmp interrupts_ISR_Stub

interrupts_ISR_70: 
        cli
        push $0
        push $0x70
        jmp interrupts_ISR_Stub

interrupts_ISR_71: 
        cli
        push $0
        push $0x71
        jmp interrupts_ISR_Stub

interrupts_ISR_72: 
        cli
        push $0
        push $0x72
        jmp interrupts_ISR_Stub

interrupts_ISR_73: 
        cli
        push $0
        push $0x73
        jmp interrupts_ISR_Stub

interrupts_ISR_74: 
        cli
        push $0
        push $0x74
        jmp interrupts_ISR_Stub

interrupts_ISR_75: 
        cli
        push $0
        push $0x75
        jmp interrupts_ISR_Stub

interrupts_ISR_76: 
        cli
        push $0
        push $0x76
        jmp interrupts_ISR_Stub

interrupts_ISR_77: 
        cli
        push $0
        push $0x77
        jmp interrupts_ISR_Stub

interrupts_ISR_78: 
        cli
        push $0
        push $0x78
        jmp interrupts_ISR_Stub

interrupts_ISR_79: 
        cli
        push $0
        push $0x79
        jmp interrupts_ISR_Stub

interrupts_ISR_7A: 
        cli
        push $0
        push $0x7A
        jmp interrupts_ISR_Stub

interrupts_ISR_7B: 
        cli
        push $0
        push $0x7B
        jmp interrupts_ISR_Stub

interrupts_ISR_7C: 
        cli
        push $0
        push $0x7C
        jmp interrupts_ISR_Stub

interrupts_ISR_7D: 
        cli
        push $0
        push $0x7D
        jmp interrupts_ISR_Stub

interrupts_ISR_7E: 
        cli
        push $0
        push $0x7E
        jmp interrupts_ISR_Stub

interrupts_ISR_7F: 
        cli
        push $0
        push $0x7F
        jmp interrupts_ISR_Stub


# We call a C function in here. We need to let the assembler know
# that '_fault_handler' exists in another file
.extern interrupts_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
interrupts_ISR_Stub: 
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    movw $0x10,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%fs
    movw %ax,%gs
    movl %esp,%eax
    pushl %eax
    movl $interrupts_handler, %eax
    call *%eax
    popl %eax
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa
    addl $8,%esp
    iret
