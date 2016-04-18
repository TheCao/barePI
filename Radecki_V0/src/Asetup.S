.text

.global asm_start
.global exc_stack
.global supervisor_sp

asm_start:
# Przygotowanie tablicy vektorow przerwan, nastepnie jest przeskok do resetu!!
   ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,hyp_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
hyp_handler:        .word hang
irq_handler:        .word interrupt_vector
fiq_handler:        .word hang

   #bl not_main

reset:
    mov r0, $0x0000
    mov r1, $0x10000
    ldmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}

	// Set stack pointer to 0x7000 for IRQ mode
	// switch to IRQ mode
	mov r0, #(0x12 | 0x80 | 0x40)
	msr cpsr_c, r0

	// FIQ, ABORT, UNDEFINED modes nie wykorzystywane ale zdefiniowane
	// switch to FIQ mode
	mov r0, #(0x11 | 0x80 | 0x40)
	msr cpsr_c, r0

	//ABORT mode
	mov r0, #(0x17 | 0x80 | 0x40)
	msr cpsr_c, r0

	//UNDEFINED mode
	mov r0, #(0x1b | 0x80 | 0x40)
	msr cpsr_c, r0

	// set stack
	mov sp, #0x7000

	// go back to supervisor mode
	mov r0, #(0x13 | 0x80 | 0x40)
	msr cpsr_c, r0

	/* Wyzej to samo co V
	mrs r0, cpsr
bic r0, $0b11111111
orr r0, $0b11010010
msr cpsr, r0
mov sp, $0x4000
bic r0, $0b11111111
orr r0, $0b11010011
msr cpsr, r0
	*/

    mov sp, $0x10000
    bl not_main



hang: b hang

.globl MemoryBarrier
MemoryBarrier:
	mcr	p15, 0, ip, c7, c5, 0		@ invalidate I cache
	mcr	p15, 0, ip, c7, c5, 6		@ invalidate BTB
	mcr	p15, 0, ip, c7, c10, 4		@ drain write buffer
	mcr	p15, 0, ip, c7, c5, 4		@ prefetch flush
	mov	pc, lr

.globl dummy
dummy:
    bx lr

   
   .space 0x100
temp_stack:

   .space 1024
exc_stack:

supervisor_sp:
   .space 4

#dodane do obslugi przerwan od Arm Timer
 .globl _enable_interrupts
 _enable_interrupts:
 	mrs r0,cpsr
 	bic r0, r0, #0x80
 	msr cpsr_c, r0

 	mov pc, lr
