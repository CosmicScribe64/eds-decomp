	thumb_func_start EffectNoblemanResolve
EffectNoblemanResolve: @ 0x08037AF4
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08037B04
	b _08037CCC
_08037B04:
	ldr r0, _08037B20 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	cmp r0, #4
	bls _08037B16
	b _08037CCC
_08037B16:
	lsl r0, r0, #2
	ldr r1, _08037B24 @ =0x08037B28
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08037B20: .4byte 0x02017A40
_08037B24: .4byte 0x08037B28
_08037B28:
	.4byte _08037CA8
	.4byte _08037C84
	.4byte _08037C5C
	.4byte _08037C38
	.4byte _08037B3C
_08037B3C:
	mov r2, #7
	ldrb r0, [r4, #0xA]
	and r2, r0
	cmp r2, #1
	beq _08037B48
	b _08037CCC
_08037B48:
	ldrb r5, [r4, #0xC]
	ldrh r1, [r4, #0xC]
	lsr r6, r1, #8
	and r2, r5
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08037BB8 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08037BBC @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	bne _08037B6C
	b _08037CCC
_08037B6C:
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08037B78
	b _08037CCC
_08037B78:
	strh r2, [r4, #0xE]
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0
	mov r3, #0
	bl FlipFieldCard
	ldrh r1, [r4, #0xE]
	add r0, r5, #0
	bl ShowRevealedCard
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0
	bl BanishFieldCard
	ldr r5, _08037BC0 @ =0x000007FF
	add r0, r5, #0
	ldrh r2, [r4]
	and r0, r2
	lsl r0, r0, #1
	ldr r6, _08037BC4 @ =0x08622AB4
	add r0, r0, r6
	ldrh r1, [r0]
	ldr r0, _08037BC8 @ =0x00000485
	cmp r1, r0
	beq _08037BCC
	add r0, #1
	cmp r1, r0
	beq _08037C14
	b _08037CCC
	.align 2, 0
_08037BB8: .4byte 0x00000D64
_08037BBC: .4byte 0x0201930C
_08037BC0: .4byte 0x000007FF
_08037BC4: .4byte gCardIdToNumber
_08037BC8: .4byte 0x00000485
_08037BCC:
	add r2, r5, #0
	ldrh r0, [r4, #0xE]
	and r2, r0
	lsl r0, r2, #2
	ldr r1, _08037C10 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08037CCC
	lsl r0, r2, #1
	add r0, r0, r6
	ldrh r0, [r0]
	mov r1, #1
	bl HasFlipEffect
	cmp r0, #0
	bne _08037C2E
	add r0, r5, #0
	ldrh r4, [r4, #0xE]
	and r0, r4
	lsl r0, r0, #1
	add r0, r0, r6
	ldrh r0, [r0]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	bne _08037C2E
	b _08037CCC
	.align 2, 0
_08037C10: .4byte gCardStats
_08037C14:
	add r0, r5, #0
	ldrh r4, [r4, #0xE]
	and r0, r4
	lsl r0, r0, #2
	ldr r2, _08037C34 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08037CCC
_08037C2E:
	mov r0, #0x7F
	b _08037CCE
	.align 2, 0
_08037C34: .4byte gCardStats
_08037C38:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08037C54 @ =0x000007FF
	ldrh r4, [r4, #0xE]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _08037C58 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	bl BanishDeckCopies
	mov r0, #0x7E
	b _08037CCE
_08037C54: .4byte 0x000007FF
_08037C58: .4byte gCardIdToNumber
_08037C5C:
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldr r1, _08037C7C @ =0x000007FF
	ldrh r4, [r4, #0xE]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _08037C80 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	bl BanishDeckCopies
	mov r0, #0x7D
	b _08037CCE
_08037C7C: .4byte 0x000007FF
_08037C80: .4byte gCardIdToNumber
_08037C84:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _08037C92
	ldr r1, _08037CA4 @ =0x00008060
_08037C92:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7C
	b _08037CCE
	.align 2, 0
_08037CA4: .4byte 0x00008060
_08037CA8:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _08037CB6
	ldr r1, _08037CC8 @ =0x00008060
_08037CB6:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7B
	b _08037CCE
	.align 2, 0
_08037CC8: .4byte 0x00008060
_08037CCC:
	mov r0, #0
_08037CCE:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectNoblemanResolve

