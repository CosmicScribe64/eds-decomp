	thumb_func_start sub_0805EF00
sub_0805EF00: @ 0x0805EF00
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r0
	add r4, r2, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	ldrh r1, [r4]
	lsl r0, r1, #1
	add r1, r0, #0
	add r1, #8
	add r6, r4, r1
	add r0, #0x10
	add r7, r4, r0
	mov r2, r9
	lsl r5, r2, #5
	ldr r0, _0805EFFC @ =0x0201CFB8
	mov sl, r0
	ldrh r2, [r6]
	cmp r4, #0
	beq _0805EFEE
	add r0, r5, r0
	lsl r2, r2, #4
	add r1, r7, #0
	bl sub_08075294
	mov r0, #0x80
	lsl r0, r0, #3
	add r0, sl
	add r0, r5, r0
	ldrh r6, [r6]
	lsl r2, r6, #4
	add r1, r7, r2
	bl sub_08075294
	mov r1, r8
	lsl r0, r1, #5
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	add r1, r4, #0
	add r1, #8
	ldrh r4, [r4]
	lsl r2, r4, #1
	bl sub_080752B0
	mov r2, r8
	lsl r6, r2, #0x1C
	lsr r6, r6, #0x10
	ldr r7, _0805F000 @ =0x03000040
	mov r8, r7
	mov r0, r9
	lsl r3, r0, #0x10
	lsr r3, r3, #0x10
	mov r4, #0x90
	lsl r4, r4, #2
	add r5, r3, r4
	lsl r5, r5, #1
	ldr r1, _0805F004 @ =0x00002C1C
	add r8, r1
	add r5, r8
	ldr r2, _0805F008 @ =0x00000FFF
	add r0, r2, #0
	ldrh r7, [r5]
	and r0, r7
	strh r0, [r5]
	mov r1, r9
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r4, r1, r4
	lsl r4, r4, #1
	add r4, r8
	add r0, r2, #0
	ldrh r7, [r4]
	and r0, r7
	strh r0, [r4]
	mov r0, #0x98
	lsl r0, r0, #2
	mov r9, r0
	add r3, r9
	lsl r3, r3, #1
	add r3, r8
	add r0, r2, #0
	ldrh r7, [r3]
	and r0, r7
	strh r0, [r3]
	add r1, r9
	lsl r1, r1, #1
	add r1, r8
	ldrh r0, [r1]
	and r2, r0
	strh r2, [r1]
	add r0, r6, #0
	ldrh r2, [r5]
	orr r0, r2
	strh r0, [r5]
	add r0, r6, #0
	ldrh r7, [r4]
	orr r0, r7
	strh r0, [r4]
	add r0, r6, #0
	ldrh r2, [r3]
	orr r0, r2
	strh r0, [r3]
	ldrh r7, [r1]
	orr r6, r7
	strh r6, [r1]
	mov r1, #0x80
	lsl r1, r1, #4
	add r1, sl
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0805EFEE:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805EFFC: .4byte 0x0201CFB8
_0805F000: .4byte 0x03000040
_0805F004: .4byte 0x00002C1C
_0805F008: .4byte 0x00000FFF
	thumb_func_end sub_0805EF00

