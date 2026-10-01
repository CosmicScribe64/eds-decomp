	thumb_func_start sub_08042B9C
sub_08042B9C: @ 0x08042B9C
	push {r4, r5, r6, lr}
	add r3, r0, #0
	add r5, r1, #0
	add r4, r2, #0
	mov r1, #0
	mov r2, #0xA0
	lsl r2, r2, #1
	add r0, r3, r2
	ldrh r0, [r0]
	cmp r1, r0
	bge _08042BD6
	add r2, r0, #0
	add r3, #2
_08042BB6:
	ldrb r6, [r3]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r5
	bne _08042BCE
	ldrh r6, [r3]
	lsl r0, r6, #0x16
	lsr r0, r0, #0x1A
	cmp r0, r4
	bne _08042BCE
	mov r0, #1
	b _08042BD8
_08042BCE:
	add r3, #0x14
	add r1, #1
	cmp r1, r2
	blt _08042BB6
_08042BD6:
	mov r0, #0
_08042BD8:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08042B9C
	.align 2, 0

