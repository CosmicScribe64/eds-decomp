	thumb_func_start sub_08079CC8
sub_08079CC8: @ 0x08079CC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	add r7, r0, #0
	str r3, [sp, #0x14]
	ldr r0, [sp, #0x48]
	ldr r3, [sp, #0x4C]
	ldr r6, [sp, #0x50]
	ldr r4, [sp, #0x54]
	ldr r5, [sp, #0x58]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x10]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x18]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp, #0x1C]
	lsl r6, r6, #0x18
	lsr r0, r6, #0x18
	mov r9, r0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	mov r8, r4
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	mov r1, #0
	str r1, [sp, #0x20]
	ldrb r0, [r7]
	cmp r0, #0
	beq _08079D78
	lsr r6, r6, #0x19
	str r6, [sp, #0x24]
	add r0, r2, #0
	add r0, #1
	lsl r6, r0, #0x10
_08079D1E:
	ldrb r0, [r7]
	ldr r2, [sp, #0x20]
	ldr r1, [sp, #0x24]
	add r4, r2, #0
	mul r4, r1
	add r1, r4, #1
	add r1, sl
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, [sp, #0x1C]
	str r2, [sp, #0]
	mov r2, r9
	str r2, [sp, #4]
	mov r2, r8
	str r2, [sp, #8]
	str r5, [sp, #0xC]
	lsr r2, r6, #0x10
	ldr r3, [sp, #0x14]
	bl sub_08079B88
	ldrb r0, [r7]
	add r7, #1
	add r4, sl
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	ldr r1, [sp, #0x18]
	str r1, [sp, #0]
	mov r2, r9
	str r2, [sp, #4]
	mov r1, r8
	str r1, [sp, #8]
	str r5, [sp, #0xC]
	add r1, r4, #0
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0x14]
	bl sub_08079B88
	ldr r0, [sp, #0x20]
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x20]
	ldrb r0, [r7]
	cmp r0, #0
	bne _08079D1E
_08079D78:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08079CC8

