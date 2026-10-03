	thumb_func_start AiPickEffectTribute
AiPickEffectTribute: @ 0x08056544
	push {r4, r5, r6, r7, lr}
	sub sp, #0xC
	add r6, r0, #0
	mov r4, #0
	ldr r2, _0805657C @ =0x0201A070
	ldr r1, _08056580 @ =0x000007FF
_08056550:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0805658C
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08056584 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	ldr r3, _08056588 @ =0xFFFFF880
	add r0, r0, r3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bhi _0805658C
	add r0, r4, #0
	b _0805663C
	.align 2, 0
_0805657C: .4byte 0x0201A070
_08056580: .4byte 0x000007FF
_08056584: .4byte gCardIdToNumber
_08056588: .4byte 0xFFFFF880
_0805658C:
	add r4, #1
	cmp r4, #4
	ble _08056550
	mov r4, #0
	ldr r3, _080565B4 @ =0x000007FF
	mov r1, #0
_08056598:
	add r0, r1, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080565BC
	cmp r4, r6
	beq _080565BC
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _080565B8 @ =0x08622AB4
	add r0, r0, r1
	mov r1, #0x2F
	b _080565E6
_080565B4: .4byte 0x000007FF
_080565B8: .4byte gCardIdToNumber
_080565BC:
	add r1, #0x94
	add r4, #1
	cmp r4, #4
	ble _08056598
	mov r4, #0
	ldr r3, _080565EC @ =0x000007FF
	mov r1, #0
_080565CA:
	add r0, r1, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080565F8
	cmp r4, r6
	beq _080565F8
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _080565F0 @ =0x08622AB4
	add r0, r0, r2
	ldr r3, _080565F4 @ =0x0000023D
	add r1, r3, #0
_080565E6:
	strh r1, [r0]
	add r0, r4, #0
	b _0805663C
_080565EC: .4byte 0x000007FF
_080565F0: .4byte gCardIdToNumber
_080565F4: .4byte 0x0000023D
_080565F8:
	add r1, #0x94
	add r4, #1
	cmp r4, #4
	ble _080565CA
	ldr r5, _08056644 @ =0x0001869F
	mov r7, #1
	neg r7, r7
	mov r4, #0
_08056608:
	mov r0, #0x94
	mul r0, r4
	ldr r1, _08056648 @ =0x0201A070
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08056634
	cmp r4, r6
	beq _08056634
	mov r0, #1
	add r1, r4, #0
	mov r2, sp
	bl GetZoneCardStats
	ldr r1, [sp, #4]
	ldr r0, [sp, #8]
	add r1, r1, r0
	cmp r5, r1
	ble _08056634
	add r7, r4, #0
	add r5, r1, #0
_08056634:
	add r4, #1
	cmp r4, #4
	ble _08056608
	add r0, r7, #0
_0805663C:
	add sp, #0xC
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08056644: .4byte 0x0001869F
_08056648: .4byte 0x0201A070
	thumb_func_end AiPickEffectTribute

