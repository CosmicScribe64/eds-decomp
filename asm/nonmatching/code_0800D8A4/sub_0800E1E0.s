	thumb_func_start sub_0800E1E0
sub_0800E1E0: @ 0x0800E1E0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, _0800E220 @ =0x020185C0
	ldrh r0, [r4]
	lsr r6, r0, #0xF
	ldrh r7, [r4, #2]
	ldr r1, _0800E224 @ =0x0000080A
	add r1, r1, r4
	mov r8, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _0800E228
	cmp r5, #1
	beq _0800E286
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08008EB4
	bl sub_080611AC
	add r0, r6, #0
	mov r1, #0
	add r2, r7, #0
	bl sub_08024134
	b _0800E23E
_0800E220: .4byte 0x020185C0
_0800E224: .4byte 0x0000080A
_0800E228:
	mov r0, #0x94
	mul r0, r7
	ldr r1, _0800E250 @ =0x00000D64
	mul r1, r6
	add r0, r0, r1
	ldr r1, _0800E254 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800E25C
_0800E23E:
	ldr r0, _0800E258 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0800E40A
	.align 2, 0
_0800E250: .4byte 0x00000D64
_0800E254: .4byte 0x0201930C
_0800E258: .4byte 0x0000080D
_0800E25C:
	add r0, r7, #0
	bl sub_08062354
	add r1, r0, #0
	add r0, r6, #0
	bl sub_080240A8
	mov r0, r8
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
	mov r1, r8
	strb r0, [r1]
	b _0800E40A
_0800E286:
	ldr r2, _0800E384 @ =0x00000814
	add r2, r2, r4
	mov sl, r2
	add r0, r6, #0
	and r0, r5
	ldr r2, _0800E388 @ =0x00000D64
	add r1, r0, #0
	mul r1, r2
	mov r9, r1
	ldr r1, _0800E38C @ =0x0201930C
	add r1, r9
	mov r0, #0x94
	add r2, r7, #0
	mul r2, r0
	mov r8, r2
	add r1, r8
	mov r0, sl
	bl sub_08007558
	add r0, r6, #0
	add r1, r7, #0
	bl sub_0800747C
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08060FD0
	mov r0, sl
	ldr r0, [r0]
	mov ip, r0
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0800E390 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _0800E394 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bhi _0800E2DA
	b _0800E3EE
_0800E2DA:
	add r0, r6, #0
	and r0, r5
	mov r4, #2
	neg r4, r4
	ldr r2, [sp, #0]
	and r2, r4
	orr r2, r0
	mov r0, #0x1F
	neg r0, r0
	and r2, r0
	ldr r0, _0800E398 @ =0x000001FF
	and r0, r7
	lsl r0, r0, #5
	ldr r1, _0800E39C @ =0xFFFFC01F
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	mov r3, r8
	add r3, r9
	ldr r1, _0800E38C @ =0x0201930C
	add r3, r3, r1
	ldrb r1, [r3, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xE
	ldr r1, _0800E3A0 @ =0xFFFFBFFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xF
	ldr r2, _0800E3A4 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	mov r2, ip
	lsl r1, r2, #0x13
	lsr r1, r1, #0x1F
	and r1, r5
	ldr r0, [sp, #4]
	and r0, r4
	orr r0, r1
	str r0, [sp, #4]
	mov r1, sl
	ldrh r1, [r1]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	bl sub_08007994
	mov r1, #0xB
	cmp r0, #0
	beq _0800E34C
	mov r1, #0xC
_0800E34C:
	lsl r1, r1, #1
	ldr r0, [sp, #4]
	mov r2, #0x1F
	neg r2, r2
	and r0, r2
	orr r0, r1
	str r0, [sp, #4]
	mov r1, sl
	ldrh r1, [r1]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x14
	bl sub_08007994
	cmp r0, #0
	bne _0800E3A8
	ldr r1, _0800E38C @ =0x0201930C
	sub r1, #0x28
	mov r2, sl
	ldr r0, [r2]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	and r0, r5
	ldr r2, _0800E388 @ =0x00000D64
	mul r0, r2
	add r0, r0, r1
	ldrb r0, [r0, #2]
	b _0800E3AA
	.align 2, 0
_0800E384: .4byte 0x00000814
_0800E388: .4byte 0x00000D64
_0800E38C: .4byte 0x0201930C
_0800E390: .4byte gUnk_08622AB4
_0800E394: .4byte 0xFFFFF880
_0800E398: .4byte 0x000001FF
_0800E39C: .4byte 0xFFFFC01F
_0800E3A0: .4byte 0xFFFFBFFF
_0800E3A4: .4byte 0xFFFF7FFF
_0800E3A8:
	mov r0, #0
_0800E3AA:
	lsl r1, r0, #5
	ldr r0, _0800E41C @ =0xFFFFC01F
	ldr r2, [sp, #4]
	and r2, r0
	orr r2, r1
	sub r0, #0x20
	and r2, r0
	str r2, [sp, #4]
	mov r3, #1
	and r6, r3
	mov r0, #0x94
	mul r0, r7
	ldr r1, _0800E420 @ =0x00000D64
	mul r1, r6
	add r0, r0, r1
	ldr r1, _0800E424 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	and r0, r3
	lsl r0, r0, #0xF
	ldr r1, _0800E428 @ =0xFFFF7FFF
	and r2, r1
	orr r2, r0
	str r2, [sp, #4]
	ldr r0, _0800E42C @ =0x02018DD4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
_0800E3EE:
	ldr r2, _0800E430 @ =0x020185C0
	ldr r0, _0800E434 @ =0x0000080A
	add r2, r2, r0
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
_0800E40A:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E41C: .4byte 0xFFFFC01F
_0800E420: .4byte 0x00000D64
_0800E424: .4byte 0x0201930C
_0800E428: .4byte 0xFFFF7FFF
_0800E42C: .4byte 0x02018DD4
_0800E430: .4byte 0x020185C0
_0800E434: .4byte 0x0000080A
	thumb_func_end sub_0800E1E0

