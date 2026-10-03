	thumb_func_start EffectBanishGraveToDestroyChainB
EffectBanishGraveToDestroyChainB: @ 0x08041898
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC0
	add r6, r0, #0
	ldr r1, _080418BC @ =0x02017A40
	ldr r2, _080418C0 @ =0x000003E5
	add r0, r1, r2
	ldrb r0, [r0]
	add r5, r1, #0
	cmp r0, #5
	bls _080418B2
	b _08041BB0
_080418B2:
	lsl r0, r0, #2
	ldr r1, _080418C4 @ =0x080418C8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080418BC: .4byte 0x02017A40
_080418C0: .4byte 0x000003E5
_080418C4: .4byte 0x080418C8
_080418C8:
	.4byte _080418E0
	.4byte _08041914
	.4byte _08041AB4
	.4byte _08041AEC
	.4byte _08041B14
	.4byte _08041B70
_080418E0:
	mov r0, #8
	neg r0, r0
	ldrb r3, [r6, #0xA]
	and r0, r3
	strb r0, [r6, #0xA]
	ldr r0, _08041900 @ =0x00000206
	ldr r1, _08041904 @ =0x00000712
	ldr r3, _08041908 @ =0x08083FD0
	mov r2, #0xB
	bl TextBoxOpen
	ldr r0, _0804190C @ =0x02017A40
	ldr r1, _08041910 @ =0x000003E5
	add r0, r0, r1
	b _08041B50
	.align 2, 0
_08041900: .4byte 0x00000206
_08041904: .4byte 0x00000712
_08041908: .4byte gStrDesignateMonsterToDestroy
_0804190C: .4byte 0x02017A40
_08041910: .4byte 0x000003E5
_08041914:
	ldr r1, _0804192C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041934
	ldr r2, _08041930 @ =0x000003E5
	add r1, r5, r2
	mov r0, #0
	strb r0, [r1]
	b _08041BB2
	.align 2, 0
_0804192C: .4byte 0x03000040
_08041930: .4byte 0x000003E5
_08041934:
	ldr r0, _08041994 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08041940
	b _08041B56
_08041940:
	ldr r0, _08041998 @ =0x0201CFB0
	ldr r3, _0804199C @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	mov r8, r1
	ldr r2, _080419A0 @ =0x00000828
	add r1, r0, r2
	add r3, #8
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r7, r1, r0
	mov r2, #1
	mov r0, r8
	and r2, r0
	mov r0, #0x94
	mul r0, r7
	ldr r1, _080419A4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080419A8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	ldr r0, _080419AC @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _080419B0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080419BC
	cmp r0, #0x17
	ble _080419B4
	cmp r0, #0x18
	beq _080419B8
	b _080419BC
_08041994: .4byte 0x00E000E0
_08041998: .4byte 0x0201CFB0
_0804199C: .4byte 0x00000824
_080419A0: .4byte 0x00000828
_080419A4: .4byte 0x00000D64
_080419A8: .4byte 0x0201930C
_080419AC: .4byte 0x000007FF
_080419B0: .4byte gCardStats
_080419B4:
	mov r0, #0
	b _080419D0
_080419B8:
	mov r0, #0xA
	b _080419D0
_080419BC:
	ldr r0, _08041A14 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _08041A18 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080419D0:
	add r4, r0, #0
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl CountGraveyardMonsters
	cmp r4, r0
	bgt _08041AAC
	add r0, r6, #0
	mov r1, r8
	add r2, r7, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08041AAC
	ldr r0, _08041A14 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _08041A18 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08041A24
	cmp r0, #0x17
	ble _08041A1C
	cmp r0, #0x18
	beq _08041A20
	b _08041A24
	.align 2, 0
_08041A14: .4byte 0x000007FF
_08041A18: .4byte gCardStats
_08041A1C:
	mov r0, #0
	b _08041A38
_08041A20:
	mov r0, #0xA
	b _08041A38
_08041A24:
	ldr r0, _08041A64 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _08041A68 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08041A38:
	add r1, r0, #0
	add r0, r6, #0
	bl AddEffectTarget
	ldr r0, _08041A64 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r3, _08041A68 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08041A74
	cmp r0, #0x17
	ble _08041A6C
	cmp r0, #0x18
	beq _08041A70
	b _08041A74
	.align 2, 0
_08041A64: .4byte 0x000007FF
_08041A68: .4byte gCardStats
_08041A6C:
	mov r2, #0
	b _08041A88
_08041A70:
	mov r2, #0xA
	b _08041A88
_08041A74:
	ldr r0, _08041A98 @ =0x000007FF
	and r5, r0
	lsl r0, r5, #2
	ldr r1, _08041A9C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r2, r0, #0x19
_08041A88:
	ldr r0, _08041AA0 @ =0x02017A40
	ldr r3, _08041AA4 @ =0x000003E6
	add r1, r0, r3
	strb r2, [r1]
	ldr r1, _08041AA8 @ =0x000003E5
	add r0, r0, r1
	b _08041B50
	.align 2, 0
_08041A98: .4byte 0x000007FF
_08041A9C: .4byte gCardStats
_08041AA0: .4byte 0x02017A40
_08041AA4: .4byte 0x000003E6
_08041AA8: .4byte 0x000003E5
_08041AAC:
	mov r0, #3
	bl PlaySE
	b _08041B56
_08041AB4:
	ldr r1, _08041AD8 @ =0x08084C3C
	ldr r2, _08041ADC @ =0x000003E6
	add r0, r5, r2
	ldrb r2, [r0]
	mov r0, sp
	bl FormatInt
	ldr r0, _08041AE0 @ =0x00000206
	ldr r1, _08041AE4 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	ldr r3, _08041AE8 @ =0x000003E5
	add r1, r5, r3
	ldrb r0, [r1]
	add r0, #1
	b _08041B98
_08041AD8: .4byte gStrSelectGraveMonstersToBanishFmt
_08041ADC: .4byte 0x000003E6
_08041AE0: .4byte 0x00000206
_08041AE4: .4byte 0x00000712
_08041AE8: .4byte 0x000003E5
_08041AEC:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _08041B08 @ =0x000005FC
	mov r3, #0
	bl CardListView_Open
	ldr r0, _08041B0C @ =0x02017A40
	ldr r1, _08041B10 @ =0x000003E5
	add r0, r0, r1
	b _08041B50
	.align 2, 0
_08041B08: .4byte 0x000005FC
_08041B0C: .4byte 0x02017A40
_08041B10: .4byte 0x000003E5
_08041B14:
	ldr r0, _08041B5C @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	mov r3, #0xD4
	cmp r0, #0
	beq _08041B34
	ldr r3, _08041B60 @ =0x000080D4
_08041B34:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _08041B64 @ =0x02017A40
	ldr r1, _08041B68 @ =0x000003E6
	add r2, r0, r1
	ldrb r1, [r2]
	sub r1, #1
	strb r1, [r2]
	ldr r2, _08041B6C @ =0x000003E5
	add r0, r0, r2
_08041B50:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_08041B56:
	mov r0, #0
	b _08041BB2
	.align 2, 0
_08041B5C: .4byte 0x0201D810
_08041B60: .4byte 0x000080D4
_08041B64: .4byte 0x02017A40
_08041B68: .4byte 0x000003E6
_08041B6C: .4byte 0x000003E5
_08041B70:
	ldr r3, _08041B9C @ =0x000003E6
	add r2, r5, r3
	ldrb r0, [r2]
	cmp r0, #0
	beq _08041BB0
	add r4, sp, #0x40
	ldr r1, _08041BA0 @ =0x08084C84
	add r2, r0, #0
	add r0, r4, #0
	bl FormatInt
	ldr r0, _08041BA4 @ =0x00000206
	ldr r1, _08041BA8 @ =0x00000712
	mov r2, #0xB
	add r3, r4, #0
	bl TextBoxOpen
	ldr r0, _08041BAC @ =0x000003E5
	add r1, r5, r0
	mov r0, #3
_08041B98:
	strb r0, [r1]
	b _08041B56
_08041B9C: .4byte 0x000003E6
_08041BA0: .4byte gStrRemainingCountFmt
_08041BA4: .4byte 0x00000206
_08041BA8: .4byte 0x00000712
_08041BAC: .4byte 0x000003E5
_08041BB0:
	mov r0, #1
_08041BB2:
	add sp, #0xC0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectBanishGraveToDestroyChainB
	.align 2, 0

