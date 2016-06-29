/*
 * libstub.S
 */

	.text

	.globl	__aeabi_uidiv
__aeabi_uidiv:
	push	{r3, lr}
	mov	r2, #0
	bl	Divide
	pop	{r3, pc}

	.globl	__aeabi_uidivmod
__aeabi_uidivmod:
	push	{lr}
	sub	sp, sp, #12
	add	r2, sp, #4
	bl	Divide
	ldr	r1, [sp, #4]
	add	sp, sp, #12
	pop	{pc}

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

 .globl dummy
dummy:
    bx lr

/* End */
