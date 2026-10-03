	thumb_func_start AnimStateDrawRaw
AnimStateDrawRaw: @ 0x080784E4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	add r5, r0, #0
	ldr r0, [sp, #0x1C]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	lsl r3, r3, #0x18
	lsr r7, r3, #0x18
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	mov r4, #0
	ldrb r0, [r5, #0xC]
	cmp r4, r0
	bcs _08078526
_08078506:
	lsl r1, r4, #3
	ldr r0, [r5, #4]
	add r0, r0, r1
	ldr r1, [sp, #0x20]
	str r1, [sp, #0]
	mov r1, r8
	add r2, r7, #0
	add r3, r6, #0
	bl OamListAddTemplate
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldrb r2, [r5, #0xC]
	cmp r4, r2
	bcc _08078506
_08078526:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end AnimStateDrawRaw
	.align 2, 0

