	thumb_func_start sub_0800DD04
sub_0800DD04: @ 0x0800DD04
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r4, _0800DD34 @ =0x020185C0
	ldrh r0, [r4]
	lsr r6, r0, #0xF
	ldrh r7, [r4, #2]
	ldr r1, _0800DD38 @ =0x0000080A
	add r1, r1, r4
	mov sl, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	cmp r5, #1
	beq _0800DD94
	cmp r5, #1
	bgt _0800DD3C
	cmp r5, #0
	beq _0800DD44
	b _0800DF5C
	.align 2, 0
_0800DD34: .4byte 0x020185C0
_0800DD38: .4byte 0x0000080A
_0800DD3C:
	cmp r5, #2
	bne _0800DD42
	b _0800DE40
_0800DD42:
	b _0800DF5C
_0800DD44:
	mov r0, #0x94
	mul r0, r7
	ldr r1, _0800DD60 @ =0x00000D64
	mul r1, r6
	add r0, r0, r1
	ldr r1, _0800DD64 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800DD6C
	ldr r0, _0800DD68 @ =0x0000080D
	add r1, r4, r0
	b _0800DF70
_0800DD60: .4byte 0x00000D64
_0800DD64: .4byte 0x0201930C
_0800DD68: .4byte 0x0000080D
_0800DD6C:
	add r0, r7, #0
	bl sub_08062354
	add r1, r0, #0
	add r0, r6, #0
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
	b _0800DF78
_0800DD94:
	mov r0, #0x11
	bl sub_08077AEC
	add r0, r6, #0
	and r0, r5
	mov r1, #2
	neg r1, r1
	ldr r2, [sp, #0]
	and r2, r1
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	ldr r0, _0800DE24 @ =0x000001FF
	and r0, r7
	lsl r0, r0, #5
	ldr r1, _0800DE28 @ =0xFFFFC01F
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	add r1, r6, #0
	and r1, r5
	mov r0, #0x94
	add r3, r7, #0
	mul r3, r0
	ldr r0, _0800DE2C @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _0800DE30 @ =0x0201930C
	add r3, r3, r0
	ldrb r1, [r3, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xE
	ldr r1, _0800DE34 @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xF
	ldr r2, _0800DE38 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldr r1, _0800DE3C @ =0x0868CAC0
	mov r0, sp
	mov r2, #0
	mov r3, #0
	bl sub_08024380
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08060FD0
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
	b _0800DF78
	.align 2, 0
_0800DE24: .4byte 0x000001FF
_0800DE28: .4byte 0xFFFFC01F
_0800DE2C: .4byte 0x00000D64
_0800DE30: .4byte 0x0201930C
_0800DE34: .4byte 0xFFFFBFFF
_0800DE38: .4byte 0xFFFF7FFF
_0800DE3C: .4byte gUnk_0868CAC0
_0800DE40:
	ldr r2, _0800DF38 @ =0x00000814
	add r4, r4, r2
	add r0, r6, #0
	mov r1, #1
	and r0, r1
	ldr r1, _0800DF3C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov r9, r2
	ldr r1, _0800DF40 @ =0x0201930C
	add r1, r9
	mov r0, #0x94
	add r2, r7, #0
	mul r2, r0
	mov r8, r2
	add r1, r8
	add r0, r4, #0
	bl sub_08007558
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08008E80
	ldr r4, [r4]
	mov ip, r4
	lsl r0, r4, #0x14
	lsr r0, r0, #0x14
	str r0, [sp, #8]
	ldr r0, _0800DF44 @ =0x000007FF
	ldr r1, [sp, #8]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0800DF48 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r1, _0800DF4C @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0800DF1E
	mov r1, sp
	mov r0, #2
	neg r0, r0
	ldrb r1, [r1]
	and r0, r1
	orr r0, r6
	mov r1, sp
	strb r0, [r1]
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, sp
	strb r0, [r1]
	ldr r2, _0800DF50 @ =0x000001FF
	add r0, r2, #0
	and r7, r0
	lsl r2, r7, #5
	ldr r5, _0800DF54 @ =0xFFFFC01F
	add r0, r5, #0
	ldrh r1, [r1]
	and r0, r1
	orr r0, r2
	mov r1, sp
	strh r0, [r1]
	mov r3, r8
	add r3, r9
	ldr r0, _0800DF40 @ =0x0201930C
	add r3, r3, r0
	ldrb r2, [r3, #6]
	lsl r1, r2, #0x1F
	mov r4, sp
	lsr r1, r1, #0x1F
	lsl r1, r1, #6
	ldrb r2, [r4, #1]
	mov r0, #0x41
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4, #1]
	ldrb r3, [r3, #6]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1F
	lsl r1, r1, #7
	mov r2, #0x7F
	and r0, r2
	orr r0, r1
	strb r0, [r4, #1]
	mov r0, ip
	lsl r1, r0, #0x13
	lsr r1, r1, #0x1F
	mov r2, #1
	and r1, r2
	ldr r0, [sp, #4]
	sub r2, #3
	and r0, r2
	orr r0, r1
	mov r1, #0x1E
	orr r0, r1
	and r0, r5
	ldr r1, _0800DF58 @ =0xFFFFBFFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	add r2, sp, #4
	ldr r0, [sp, #8]
	mov r1, sp
	bl sub_080242C4
_0800DF1E:
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
	b _0800DF78
_0800DF38: .4byte 0x00000814
_0800DF3C: .4byte 0x00000D64
_0800DF40: .4byte 0x0201930C
_0800DF44: .4byte 0x000007FF
_0800DF48: .4byte gUnk_08622AB4
_0800DF4C: .4byte 0xFFFFF880
_0800DF50: .4byte 0x000001FF
_0800DF54: .4byte 0xFFFFC01F
_0800DF58: .4byte 0xFFFFBFFF
_0800DF5C:
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_08024134
	bl sub_080611AC
	ldr r1, _0800DF8C @ =0x020185C0
	ldr r2, _0800DF90 @ =0x0000080D
	add r1, r1, r2
_0800DF70:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0800DF78:
	strb r0, [r1]
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800DF8C: .4byte 0x020185C0
_0800DF90: .4byte 0x0000080D
	thumb_func_end sub_0800DD04

