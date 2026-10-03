	thumb_func_start EffectReflectPlayerMagicResolve
EffectReflectPlayerMagicResolve: @ 0x08039C48
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r6, r1, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	beq _08039C5A
	b _08039D84
_08039C5A:
	ldr r5, _08039CDC @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r5, r2
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _08039CAE
	ldr r0, _08039CE0 @ =0x000004E4
	add r4, r5, r0
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0x14
	bl MemCopy16
	ldr r1, _08039CE4 @ =0x000004E6
	add r3, r5, r1
	ldrb r2, [r3]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	ldrh r0, [r4]
	bl FindCardEffect
	mov r2, #0x9F
	lsl r2, r2, #3
	add r3, r5, r2
	ldr r2, _08039CE8 @ =0x0819A9D4
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	add r2, #4
	add r1, r1, r2
	ldr r0, [r1]
	str r0, [r3]
_08039CAE:
	ldr r0, _08039CEC @ =0x000007FF
	ldrh r6, [r6]
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08039CF0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08039CF4 @ =0x000003F2
	cmp r1, r0
	beq _08039D36
	cmp r1, r0
	bgt _08039D10
	sub r0, #0x2A
	cmp r1, r0
	beq _08039D36
	cmp r1, r0
	bgt _08039CFC
	ldr r0, _08039CF8 @ =0x00000159
	cmp r1, r0
	bgt _08039D84
	sub r0, #8
	b _08039D04
	.align 2, 0
_08039CDC: .4byte 0x02017A40
_08039CE0: .4byte 0x000004E4
_08039CE4: .4byte 0x000004E6
_08039CE8: .4byte gCardEffects
_08039CEC: .4byte 0x000007FF
_08039CF0: .4byte gCardIdToNumber
_08039CF4: .4byte 0x000003F2
_08039CF8: .4byte 0x00000159
_08039CFC:
	ldr r0, _08039D0C @ =0x000003EF
	cmp r1, r0
	bgt _08039D84
	sub r0, #1
_08039D04:
	cmp r1, r0
	blt _08039D84
	b _08039D36
	.align 2, 0
_08039D0C: .4byte 0x000003EF
_08039D10:
	ldr r0, _08039D2C @ =0x0000042F
	cmp r1, r0
	bgt _08039D30
	sub r0, #1
	cmp r1, r0
	bge _08039D36
	sub r0, #0x2D
	cmp r1, r0
	beq _08039D36
	add r0, #0xE
	cmp r1, r0
	beq _08039D36
	b _08039D84
	.align 2, 0
_08039D2C: .4byte 0x0000042F
_08039D30:
	ldr r0, _08039D74 @ =0x00000439
	cmp r1, r0
	bne _08039D84
_08039D36:
	ldr r4, _08039D78 @ =0x02017A40
	mov r2, #0x9F
	lsl r2, r2, #3
	add r1, r4, r2
	sub r2, #0x14
	add r0, r4, r2
	ldr r2, [r1]
	mov r1, #0
	bl _call_via_r2
	mov r1, #0xF8
	lsl r1, r1, #2
	add r4, r4, r1
	strb r0, [r4]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _08039D80
	mov r0, #1
	ldrb r7, [r7, #2]
	and r0, r7
	mov r1, #0xB0
	cmp r0, #0
	beq _08039D66
	ldr r1, _08039D7C @ =0x000080B0
_08039D66:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08039D84
_08039D74: .4byte 0x00000439
_08039D78: .4byte 0x02017A40
_08039D7C: .4byte 0x000080B0
_08039D80:
	ldrb r0, [r4]
	b _08039D86
_08039D84:
	mov r0, #0
_08039D86:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectReflectPlayerMagicResolve

