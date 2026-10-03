	thumb_func_start EffectMagicTargetChainB
EffectMagicTargetChainB: @ 0x0803E8F8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	mov r2, #1
	ldrb r0, [r7, #2]
	and r2, r0
	cmp r2, #0
	beq _0803E9D4
	mov r0, #8
	neg r0, r0
	ldrb r1, [r7, #0xA]
	and r0, r1
	strb r0, [r7, #0xA]
	mov r5, #0
	mov r2, #1
	mov r9, r2
	ldr r3, _0803E9C4 @ =0x00000D64
	mov r8, r3
_0803E920:
	mov r4, #5
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r6, r8
	mul r6, r0
_0803E92C:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0803E9C8 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0803E974
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E974
	ldr r3, _0803E9CC @ =0x000007FF
	add r0, r3, #0
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _0803E9D0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0803E974
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803E9BE
_0803E974:
	add r4, #1
	cmp r4, #0xA
	ble _0803E92C
	mov r4, #5
	mov r0, #1
	and r0, r5
	ldr r1, _0803E9C4 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_0803E986:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0803E9C8 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803E9B2
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803E9B2
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803E9BE
_0803E9B2:
	add r4, #1
	cmp r4, #0xA
	ble _0803E986
	add r5, #1
	cmp r5, #1
	ble _0803E920
_0803E9BE:
	mov r0, #1
	b _0803EA80
	.align 2, 0
_0803E9C4: .4byte 0x00000D64
_0803E9C8: .4byte 0x0201930C
_0803E9CC: .4byte 0x000007FF
_0803E9D0: .4byte gCardStats
_0803E9D4:
	ldr r0, _0803EA24 @ =0x02017A40
	ldr r1, _0803EA28 @ =0x000003E5
	add r3, r0, r1
	ldrb r0, [r3]
	cmp r0, #0
	bne _0803EA38
	mov r0, #8
	neg r0, r0
	ldrb r2, [r7, #0xA]
	and r0, r2
	strb r0, [r7, #0xA]
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl CountSpellTrapsFiltered
	cmp r0, #0
	bne _0803EA0A
	mov r0, #1
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl CountSpellTrapsFiltered
	cmp r0, #0
	beq _0803E9BE
_0803EA0A:
	ldr r0, _0803EA2C @ =0x00000206
	ldr r1, _0803EA30 @ =0x00000712
	ldr r3, _0803EA34 @ =0x08083F60
	mov r2, #0xB
	bl TextBoxOpen
	ldr r0, _0803EA24 @ =0x02017A40
	ldr r3, _0803EA28 @ =0x000003E5
	add r0, r0, r3
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803EA7E
_0803EA24: .4byte 0x02017A40
_0803EA28: .4byte 0x000003E5
_0803EA2C: .4byte 0x00000206
_0803EA30: .4byte 0x00000712
_0803EA34: .4byte gStrDesignateMagicToDestroy
_0803EA38:
	ldr r1, _0803EA48 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803EA4C
	strb r2, [r3]
	b _0803EA7E
_0803EA48: .4byte 0x03000040
_0803EA4C:
	ldr r0, _0803EA8C @ =0x00060006
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803EA7E
	ldr r0, _0803EA90 @ =0x0201CFB0
	ldr r2, _0803EA94 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803EA98 @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r7, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803E9BE
	mov r0, #3
	bl PlaySE
_0803EA7E:
	mov r0, #0
_0803EA80:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803EA8C: .4byte 0x00060006
_0803EA90: .4byte 0x0201CFB0
_0803EA94: .4byte 0x00000824
_0803EA98: .4byte 0x00000828
	thumb_func_end EffectMagicTargetChainB

