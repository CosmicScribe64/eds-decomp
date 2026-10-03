	thumb_func_start Chain_Update
Chain_Update: @ 0x080213C0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r2, _080213E8 @ =0x02017A40
	mov r0, #0xF4
	lsl r0, r0, #2
	add r1, r2, r0
	mov r3, #1
	add r0, r3, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _080213EC
	bl Chain_Build
	b _080213FE
	.align 2, 0
_080213E8: .4byte 0x02017A40
_080213EC:
	ldr r1, _08021404 @ =0x000003D2
	add r0, r2, r1
	add r1, r3, #0
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	beq _08021408
	bl Chain_Resolve
_080213FE:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0802156E
_08021404: .4byte 0x000003D2
_08021408:
	mov r4, #0xF1
	lsl r4, r4, #2
	add r3, r2, r4
	ldrh r0, [r3]
	cmp r0, #0
	bne _08021416
	b _0802156C
_08021416:
	mov r5, #0xF0
	lsl r5, r5, #2
	add r0, r2, r5
	strh r1, [r0]
	mov r4, #0
	ldrh r3, [r3]
	cmp r4, r3
	bge _0802145E
	mov r0, #0xA0
	lsl r0, r0, #2
	add r0, r0, r2
	mov r8, r0
	add r5, r2, r5
	mov r1, #0xF1
	lsl r1, r1, #2
	add r1, r1, r2
	mov r9, r1
	mov r7, r8
	mov r6, #0
_0802143C:
	ldr r1, _080214D8 @ =0xFFFFFD80
	add r1, r8
	add r1, r6, r1
	add r0, r7, #0
	mov r2, #0x14
	bl MemCopy16
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
	add r7, #0x14
	add r6, #0x14
	add r4, #1
	mov r2, r9
	ldrh r2, [r2]
	cmp r4, r2
	blt _0802143C
_0802145E:
	ldr r4, _080214DC @ =0x000004DA
	mov r0, #0
	add r1, r4, #0
	bl CountGraveyardCardsByNumber
	str r0, [sp, #0]
	mov r0, #1
	add r1, r4, #0
	bl CountGraveyardCardsByNumber
	str r0, [sp, #4]
	mov r4, #0
	ldr r1, _080214E0 @ =0x02017A40
	mov r5, #0xF0
	lsl r5, r5, #2
	add r0, r1, r5
	ldrh r2, [r0]
	cmp r4, r2
	bge _08021534
	mov r9, r0
_08021486:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r3, r0, #2
	ldr r5, _080214E0 @ =0x02017A40
	add r2, r3, r5
	mov r0, #0xA0
	lsl r0, r0, #2
	add r1, r2, r0
	ldr r5, _080214E4 @ =0x000007FF
	add r0, r5, #0
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080214E8 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _080214DC @ =0x000004DA
	add r5, r4, #1
	mov sl, r5
	ldrh r0, [r0]
	cmp r0, r1
	bne _0802152A
	ldr r1, _080214EC @ =0x00000282
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	lsl r0, r0, #2
	add r0, sp
	ldr r0, [r0]
	cmp r0, #0
	ble _080214F0
	lsr r0, r1, #0x1F
	lsl r0, r0, #2
	add r0, sp
	lsr r1, r1, #0x1F
	lsl r1, r1, #2
	add r1, sp
	ldr r1, [r1]
	sub r1, #1
	str r1, [r0]
	b _0802152A
_080214D8: .4byte 0xFFFFFD80
_080214DC: .4byte 0x000004DA
_080214E0: .4byte 0x02017A40
_080214E4: .4byte 0x000007FF
_080214E8: .4byte gCardIdToNumber
_080214EC: .4byte 0x00000282
_080214F0:
	mov r2, r9
	ldrh r0, [r2]
	sub r0, #1
	strh r0, [r2]
	add r6, r4, #0
	add r4, r0, #0
	cmp r6, r4
	bge _0802152A
	ldr r0, _08021560 @ =0x02017CC0
	mov r5, #0xA0
	lsl r5, r5, #1
	add r5, r5, r0
	mov r8, r5
	add r5, r3, r0
	add r4, r3, #0
	add r7, r0, #0
	add r7, #0x14
_08021512:
	add r1, r4, r7
	add r0, r5, #0
	mov r2, #0x14
	bl MemCopy16
	add r5, #0x14
	add r4, #0x14
	add r6, #1
	mov r0, r8
	ldrh r0, [r0]
	cmp r6, r0
	blt _08021512
_0802152A:
	mov r4, sl
	mov r1, r9
	ldrh r1, [r1]
	cmp r4, r1
	blt _08021486
_08021534:
	ldr r3, _08021564 @ =0x02017A40
	mov r2, #0xF1
	lsl r2, r2, #2
	add r1, r3, r2
	mov r2, #0
	mov r0, #0
	strh r0, [r1]
	mov r4, #0xF0
	lsl r4, r4, #2
	add r0, r3, r4
	ldrh r0, [r0]
	neg r0, r0
	mov r5, #0xF4
	lsl r5, r5, #2
	add r1, r3, r5
	lsr r0, r0, #0x1F
	strb r0, [r1]
	ldr r1, _08021568 @ =0x000003D1
	add r0, r3, r1
	strb r2, [r0]
	mov r0, #1
	b _0802156E
_08021560: .4byte 0x02017CC0
_08021564: .4byte 0x02017A40
_08021568: .4byte 0x000003D1
_0802156C:
	mov r0, #0
_0802156E:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end Chain_Update
	.align 2, 0

