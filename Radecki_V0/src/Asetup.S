.text

.global asm_start
.global exc_stack
.global supervisor_sp

asm_start:

   # initialize Stack pointer for exception modes
   mrs r4, cpsr
   bic r4, r4, #0x1f

   #FIQ Mode
   orr r3, r4, #0x11
   msr cpsr_c, r3
   ldr sp, =exc_stack

   #IRQ Mode
   orr r3, r4, #0x12
   msr cpsr_c, r3
   ldr sp, =exc_stack

   #ABORT Mode
   orr r3, r4, #0x17
   msr cpsr_c, r3
   ldr sp, =exc_stack

   #UNDEFINED Mode
   orr r3, r4, #0x1b
   msr cpsr_c, r3
   ldr sp, =exc_stack

   # switch to supervisor mode
   orr r3, r4, #0x13
   msr cpsr_c, r3
   
   ldr sp, =temp_stack
   
   #
   # Create the first stack frame.
   #
   mov fp, #0
   mov ip, sp
   push {fp, ip, lr, pc}
   sub fp, ip, #4

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

 .globl _enable_interrupts
 _enable_interrupts:
 	mrs r0,cpsr
 	bic r0, r0, #0x80
 	msr cpsr_c, r0

 	mov pc, lr
