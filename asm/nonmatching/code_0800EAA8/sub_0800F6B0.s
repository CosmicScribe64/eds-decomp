	thumb_func_start sub_0800F6B0
sub_0800F6B0: @ 0x0800F6B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	ldr r5, _0800F700 @ =0x020185C0
	ldrh r0, [r5]
	lsr r6, r0, #0xF
	ldrh r1, [r5, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r5, #2]
	orr r0, r2
	str r0, [sp, #4]
	ldrh r7, [r5, #6]
	ldr r0, _0800F704 @ =0x0000080A
	add r0, r0, r5
	mov sl, r0
	ldrb r1, [r0]
	lsl r0, r1, #0x19
	lsr r4, r0, #0x19
	cmp r4, #0
	beq _0800F70C
	cmp r4, #1
	beq _0800F72E
	ldr r0, _0800F708 @ =0x00000814
	add r2, r5, r0
	mov r0, #1
	str r0, [sp, #0]
	add r0, r6, #0
	add r1, r7, #0
	mov r3, #0
	bl sub_08007A4C
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_08024134
	b _0800F7C8
_0800F700: .4byte 0x020185C0
_0800F704: .4byte 0x0000080A
_0800F708: .4byte 0x00000814
_0800F70C:
	add r0, r6, #0
	mov r1, #0xB
	bl sub_080240A8
	mov r0, sl
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, sl
	b _0800F7D8
_0800F72E:
	add r0, r6, #0
	add r1, sp, #4
	bl sub_08007F48
	cmp r0, #0
	beq _0800F7C8
	ldr r2, _0800F7B8 @ =0x00000814
	add r2, r2, r5
	mov r9, r2
	mov r0, r9
	add r1, sp, #4
	bl sub_08007558
	and r6, r4
	mov r8, r6
	mov r6, #2
	neg r6, r6
	ldr r0, [sp, #8]
	and r0, r6
	mov r1, r8
	orr r0, r1
	mov r3, #0x1F
	neg r3, r3
	and r0, r3
	mov r1, #0x1A
	orr r0, r1
	ldr r4, _0800F7BC @ =0xFFFFC01F
	and r0, r4
	ldr r5, _0800F7C0 @ =0xFFFFBFFF
	and r0, r5
	mov r2, #0x80
	lsl r2, r2, #8
	orr r0, r2
	str r0, [sp, #8]
	ldr r0, [sp, #0xC]
	and r0, r6
	mov r1, r8
	orr r0, r1
	and r0, r3
	ldr r1, _0800F7C4 @ =0x000001FF
	and r7, r1
	lsl r1, r7, #5
	and r0, r4
	orr r0, r1
	and r0, r5
	orr r0, r2
	str r0, [sp, #0xC]
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #8
	add r2, sp, #0xC
	bl sub_080242C4
	mov r0, sl
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, sl
	b _0800F7D8
	.align 2, 0
_0800F7B8: .4byte 0x00000814
_0800F7BC: .4byte 0xFFFFC01F
_0800F7C0: .4byte 0xFFFFBFFF
_0800F7C4: .4byte 0x000001FF
_0800F7C8:
	bl sub_080611AC
	ldr r2, _0800F7EC @ =0x0000080D
	add r1, r5, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0800F7D8:
	strb r0, [r1]
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F7EC: .4byte 0x0000080D
	thumb_func_end sub_0800F6B0

