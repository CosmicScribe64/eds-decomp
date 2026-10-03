	thumb_func_start EffectRiryokuChainB
EffectRiryokuChainB: @ 0x080409E0
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r0, _080409FC @ =0x02017A40
	ldr r1, _08040A00 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #1
	beq _08040A3C
	cmp r0, #1
	bgt _08040A04
	cmp r0, #0
	beq _08040A0E
	b _08040B0C
	.align 2, 0
_080409FC: .4byte 0x02017A40
_08040A00: .4byte 0x000003E5
_08040A04:
	cmp r0, #2
	beq _08040A80
	cmp r0, #3
	beq _08040AA4
	b _08040B0C
_08040A0E:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	bl EffectRiryokuPrepare
	cmp r0, #0
	beq _08040B0C
	ldr r0, _08040A30 @ =0x00000206
	ldr r1, _08040A34 @ =0x00000712
	ldr r3, _08040A38 @ =0x0808484C
	b _08040A86
	.align 2, 0
_08040A30: .4byte 0x00000206
_08040A34: .4byte 0x00000712
_08040A38: .4byte gStrDesignateMonsterToHalveAtk
_08040A3C:
	ldr r1, _08040A70 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08040AB0
	ldr r0, _08040A74 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08040A92
	ldr r0, _08040A78 @ =0x0201CFB0
	ldr r3, _08040A7C @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r5, #0
	bl TryAddEffectTarget
	b _08040A8C
_08040A70: .4byte 0x03000040
_08040A74: .4byte 0x00E000E0
_08040A78: .4byte 0x0201CFB0
_08040A7C: .4byte 0x00000824
_08040A80:
	ldr r0, _08040A98 @ =0x00000206
	ldr r1, _08040A9C @ =0x00000712
	ldr r3, _08040AA0 @ =0x08084520
_08040A86:
	mov r2, #0xB
	bl TextBoxOpen
_08040A8C:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08040A92:
	mov r0, #0
	b _08040B0E
	.align 2, 0
_08040A98: .4byte 0x00000206
_08040A9C: .4byte 0x00000712
_08040AA0: .4byte gStrDesignateMonsterToIncreaseAtk
_08040AA4:
	ldr r1, _08040AB8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040ABC
_08040AB0:
	mov r0, #0
	strb r0, [r4]
	b _08040B0E
	.align 2, 0
_08040AB8: .4byte 0x03000040
_08040ABC:
	ldr r0, _08040AF4 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08040A92
	ldr r0, _08040AF8 @ =0x0201CFB0
	ldr r1, _08040AFC @ =0x00000824
	add r2, r0, r1
	ldr r3, _08040B00 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	ldr r1, [r2]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	ldrb r2, [r2]
	orr r0, r2
	ldrh r2, [r5, #0xC]
	cmp r2, r0
	bne _08040B04
	mov r0, #3
	bl PlaySE
	b _08040A92
	.align 2, 0
_08040AF4: .4byte 0x00E000E0
_08040AF8: .4byte 0x0201CFB0
_08040AFC: .4byte 0x00000824
_08040B00: .4byte 0x00000828
_08040B04:
	add r0, r5, #0
	add r2, r3, #0
	bl TryAddEffectTarget
_08040B0C:
	mov r0, #1
_08040B0E:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectRiryokuChainB

