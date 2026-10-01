	thumb_func_start sub_08078E80
sub_08078E80: @ 0x08078E80
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	ldr r0, [sp, #0x14]
	ldr r4, [sp, #0x18]
	lsl r1, r1, #0x18
	lsr r5, r1, #0x18
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	cmp r5, #0x9F
	bls _08078EBC
	add r0, r5, #0
	sub r0, #0x20
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	mov r0, #1
	ldr r1, [sp, #0x1C]
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08078EBC
	add r0, r5, #0
	add r0, #0x40
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
_08078EBC:
	lsl r0, r3, #5
	add r0, r2, r0
	lsl r0, r0, #1
	add r0, r0, r7
	orr r5, r4
	lsl r1, r6, #0xC
	orr r5, r1
	strh r5, [r0]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08078E80
	.align 2, 0

