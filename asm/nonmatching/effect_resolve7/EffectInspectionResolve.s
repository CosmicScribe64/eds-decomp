	thumb_func_start EffectInspectionResolve
EffectInspectionResolve: @ 0x08037CD4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	cmp r0, #0
	beq _08037CF2
	b _08037E74
_08037CF2:
	mov r0, #0xFC
	ldrb r1, [r6, #3]
	and r0, r1
	cmp r0, #0xC
	beq _08037CFE
	b _08037E74
_08037CFE:
	ldr r7, _08037D1C @ =0x02017A40
	mov r0, #0xF8
	lsl r0, r0, #2
	add r0, r0, r7
	mov ip, r0
	ldrb r0, [r0]
	mov r9, r7
	cmp r0, #0x7F
	beq _08037D9C
	cmp r0, #0x7F
	bgt _08037D20
	cmp r0, #0x78
	bne _08037D1A
	b _08037E20
_08037D1A:
	b _08037E74
_08037D1C: .4byte 0x02017A40
_08037D20:
	cmp r0, #0x80
	beq _08037D26
	b _08037E74
_08037D26:
	ldrb r1, [r6, #2]
	lsl r5, r1, #0x1F
	mov r3, #1
	lsr r2, r5, #0x1F
	ldrh r1, [r6, #2]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r0, r1
	ldr r4, _08037D84 @ =0x00000D64
	add r1, r2, #0
	mul r1, r4
	add r0, r0, r1
	ldr r2, _08037D88 @ =0x0201930C
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08037D4E
	b _08037E74
_08037D4E:
	sub r2, #0x28
	lsr r0, r5, #0x1F
	sub r0, r3, r0
	and r0, r3
	mul r0, r4
	add r0, r0, r2
	ldrb r0, [r0, #2]
	lsl r0, r0, #1
	mov sl, r0
	ldr r1, _08037D8C @ =0x000003E1
	add r0, r7, r1
	mov r1, sl
	strb r1, [r0]
	lsr r0, r5, #0x1F
	sub r0, r3, r0
	and r0, r3
	mul r0, r4
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #1
	bne _08037D94
	ldr r1, _08037D90 @ =0x000003E2
	add r0, r7, r1
	mov r1, r8
	strb r1, [r0]
	mov r0, #0x78
	b _08037E76
_08037D84: .4byte 0x00000D64
_08037D88: .4byte 0x0201930C
_08037D8C: .4byte 0x000003E1
_08037D90: .4byte 0x000003E2
_08037D94:
	mov r1, ip
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_08037D9C:
	ldr r1, _08037DE0 @ =0x000003E1
	add r1, r9
	ldrb r0, [r1]
	cmp r0, #0
	beq _08037DEC
	sub r0, #1
	strb r0, [r1]
	ldrb r0, [r6, #2]
	lsl r5, r0, #0x1F
	lsr r5, r5, #0x1F
	mov r4, #1
	sub r5, r4, r5
	bl Random
	ldr r3, _08037DE4 @ =0x020192E4
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r1, r1, #0x1F
	sub r1, r4, r1
	and r1, r4
	ldr r2, _08037DE8 @ =0x00000D64
	mul r1, r2
	add r1, r1, r3
	ldrb r1, [r1, #2]
	bl __modsi3
	add r2, r0, #0
	add r0, r5, #0
	mov r1, #0xB
	bl DuelCursor_Select
	mov r0, #0x7F
	b _08037E76
	.align 2, 0
_08037DE0: .4byte 0x000003E1
_08037DE4: .4byte 0x020192E4
_08037DE8: .4byte 0x00000D64
_08037DEC:
	bl Random
	ldr r3, _08037E14 @ =0x020192E4
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r1, r1, #0x1F
	mov r2, #1
	eor r1, r2
	ldr r2, _08037E18 @ =0x00000D64
	mul r1, r2
	add r1, r1, r3
	ldrb r1, [r1, #2]
	bl __modsi3
	ldr r1, _08037E1C @ =0x000003E2
	add r1, r9
	strb r0, [r1]
	mov r0, #0x78
	b _08037E76
	.align 2, 0
_08037E14: .4byte 0x020192E4
_08037E18: .4byte 0x00000D64
_08037E1C: .4byte 0x000003E2
_08037E20:
	ldrb r1, [r6, #2]
	mov r5, #1
	add r0, r5, #0
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _08037E30
	ldr r3, _08037E84 @ =0x00008008
_08037E30:
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r0, _08037E88 @ =0x000003E2
	add r4, r7, r0
	ldrb r0, [r4]
	lsl r2, r0, #8
	mov r0, #0xB
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r5, r1
	and r1, r5
	ldrb r4, [r4]
	lsl r2, r4, #2
	ldr r3, _08037E8C @ =0x00000D64
	mul r1, r3
	add r2, r2, r1
	ldr r1, _08037E90 @ =0x02019968
	add r2, r2, r1
	ldr r1, [r2]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowCardDetail
_08037E74:
	mov r0, #0
_08037E76:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08037E84: .4byte 0x00008008
_08037E88: .4byte 0x000003E2
_08037E8C: .4byte 0x00000D64
_08037E90: .4byte 0x02019968
	thumb_func_end EffectInspectionResolve

