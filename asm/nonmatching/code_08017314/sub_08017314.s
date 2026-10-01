	thumb_func_start sub_08017314
sub_08017314: @ 0x08017314
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	str r0, [sp, #4]
	mov sl, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	mov r1, #1
	and r1, r0
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	ldr r3, _080173BC @ =0x00000D64
	add r0, r1, #0
	mul r0, r3
	add r2, r2, r0
	ldr r1, _080173C0 @ =0x0201930C
	add r2, r2, r1
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0801734A
	b _08017450
_0801734A:
	ldr r4, [sp, #4]
	add r0, r2, #0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r0, #0
	ble _080173FA
	mov r8, r2
	mov r1, r8
	add r1, #0x4A
	str r1, [sp, #0]
_0801735E:
	sub r0, #1
	lsl r1, r0, #1
	mov r2, r8
	add r2, #0xA
	add r2, r2, r1
	ldr r3, [sp, #0]
	add r1, r3, r1
	ldrb r3, [r1]
	ldrh r6, [r2]
	ldrb r5, [r2]
	lsr r1, r6, #8
	mov ip, r0
	cmp r3, #1
	bne _080173F4
	add r0, r5, #0
	and r0, r3
	mov r7, #0x94
	mul r1, r7
	ldr r2, _080173BC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r7, _080173C0 @ =0x0201930C
	add r1, r1, r7
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _080173F4
	mov r0, #2
	ldrb r7, [r1, #6]
	and r0, r7
	cmp r0, #0
	beq _080173F4
	ldr r0, _080173C4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _080173C8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _080173CC @ =0x0000042C
	cmp r2, r0
	beq _080173D4
	ldr r0, _080173D0 @ =0x000005EA
	cmp r2, r0
	beq _080173EC
	b _080173F4
	.align 2, 0
_080173BC: .4byte 0x00000D64
_080173C0: .4byte 0x0201930C
_080173C4: .4byte 0x000007FF
_080173C8: .4byte gUnk_08622AB4
_080173CC: .4byte 0x0000042C
_080173D0: .4byte 0x000005EA
_080173D4:
	add r0, r1, #0
	add r0, #0x91
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1F
	cmp r6, r9
	bne _080173E4
	mov r0, #1
_080173E4:
	add r4, r5, #0
	cmp r0, #0
	beq _080173F4
	b _080173F2
_080173EC:
	add r4, r5, #0
	cmp r6, r9
	bne _080173F4
_080173F2:
	sub r4, r3, r4
_080173F4:
	mov r0, ip
	cmp r0, #0
	bgt _0801735E
_080173FA:
	ldr r3, [sp, #4]
	cmp r4, r3
	beq _08017450
	add r0, r4, #0
	bl sub_08008A44
	add r3, r0, #0
	mov r0, #1
	neg r0, r0
	cmp r3, r0
	bne _08017432
	ldr r7, [sp, #4]
	lsl r0, r7, #0x18
	mov r2, sl
	lsl r1, r2, #0x18
	lsr r0, r0, #8
	orr r0, r1
	lsr r0, r0, #0x10
	mov r1, r9
	mov r2, #1
	bl sub_08009424
	ldr r0, [sp, #4]
	mov r1, sl
	mov r2, #1
	bl sub_08018544
	b _08017450
_08017432:
	ldr r7, [sp, #4]
	lsl r1, r7, #0x18
	mov r2, sl
	lsl r0, r2, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	lsl r2, r4, #0x18
	lsl r0, r3, #0x18
	lsr r2, r2, #8
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r7, #0
	bl sub_08019078
_08017450:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08017314

