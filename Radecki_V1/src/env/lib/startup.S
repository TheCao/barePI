/*
 * startup.S
 *
 * This file contains code taken from Linux:
 *	safe_svcmode_maskall macro
 *	defined in arch/arm/include/asm/assembler.h
 *	Copyright (C) 1996-2000 Russell King
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <uspienv/sysconfig.h>

/*
 * Helper macro to enter SVC mode cleanly and mask interrupts. reg is
 * a scratch register for the macro to overwrite.
 *
 * This macro is intended for forcing the CPU into SVC mode at boot time.
 * you cannot return to the original mode.
 */
	.macro safe_svcmode_maskall reg:req

	mrs	\reg , cpsr
	eor	\reg, \reg, #0x1A		/* test for HYP mode */
	tst	\reg, #0x1F
	bic	\reg , \reg , #0x1F		/* clear mode bits */
	orr	\reg , \reg , #0xC0 | 0x13	/* mask IRQ/FIQ bits and set SVC mode */
	bne	1f				/* branch if not HYP mode */
	orr	\reg, \reg, #0x100		/* mask Abort bit */
	adr	lr, 2f
	msr	spsr_cxsf, \reg
	.word	0xE12EF30E			/* msr ELR_hyp, lr */
	.word	0xE160006E			/* eret */
1:	msr	cpsr_c, \reg
2:
	.endm

	.text

	.globl	_start
_start:
	safe_svcmode_maskall r0

	@ L1 cache
	//To enable L1 Cache on the ARMv6 CPU and enable branch prediction we set bits 11 and 12 in the Control Registor of CP15.
//Bit 12 enables the L1 instruction Cache, and is available in the StrongARM and all later ARM CPUs.
//Bit 11 enables branch prediction, and is only available in ARMv6 and all later CPUs.
//If you wish you can also enable Bit 2 to enable the Data Cache, though be careful with this one.
    mrc p15,0,r0,c1,c0
    mov r1,#3
    orr r0,r0,r1,LSL#11      //Set bits 11 and 12.
    mcr p15,0,r0,c1,c0      //Update the CP15 Control registor (C1) from R0.

	@ enable the VFP
	mrc p15, 0, r0, c1, c0, 2		 /* move from coprocessor to register(s) */
	orr r0, r0, #0x300000            /* single precision */
	orr r0, r0, #0xC00000            /* double precision */
	mcr p15, 0, r0, c1, c0, 2		/* move to coprocessor from register */
	mov r0, #0x40000000       		/* start address of mapped physical memory */
	fmxr fpexc,r0					/* store all floating-point operation results from fpexc in physhical memory */

	cps	#0x1F				/* set system mode */
	mov	sp, #MEM_KERNEL_STACK


	b	sysinit

#if RASPPI != 1
	
	.globl	_start_secondary
_start_secondary:
	dsb /* data synchronization barrier */
1:	wfi /* wait for interrupt - suspend execution until IRQ/FIQ interrupt, Imprecise Data abort or Debug Entry request */
	b	1b /* branch to label 1 behind  */

#endif

/* End */
