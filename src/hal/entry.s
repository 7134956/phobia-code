
.syntax unified
.cpu cortex-m4
.thumb

.section .vectors

	.word	ldStack

	.word	irqReset
	.word	irqNMI
	.word	irqHardFault
	.word	irqMemoryFault
	.word	irqBusFault
	.word	irqUsageFault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqSVCall
	.word	irqDefault
	.word	irqDefault
	.word	irqPendSV
	.word	irqSysTick

	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqADC
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqUSART1
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqTIM8_UP_TIM13
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDMA2_Stream7
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault
	.word	irqDefault

.section .text

.type irqDefault, function
.align

irqDefault:

	b.n	irqDefault


.global irqReset
.type irqReset, function
.align

irqReset:

	ldr	r2, =(ldSdata)
	ldr	r1, =(ldEtext)
	ldr	r3, =(ldEdata)

	b.n	__dataComp

__dataLoop:

	ldr.w	r0, [r1], #4
	str.w	r0, [r2], #4

__dataComp:

	cmp	r2, r3
	bne.n	__dataLoop

	ldr	r2, =(ldSbss)
	ldr	r1, =(ldEbss)
	mov	r0, #0

	b.n	__bssComp

__bssLoop:

	str.w	r0, [r2], #4

__bssComp:

	cmp	r2, r1
	bne.n	__bssLoop

	bl	halStart
	bl	halMain

	bx	lr


