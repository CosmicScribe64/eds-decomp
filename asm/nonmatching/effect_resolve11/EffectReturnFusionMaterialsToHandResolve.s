	thumb_func_start EffectReturnFusionMaterialsToHandResolve
EffectReturnFusionMaterialsToHandResolve: @ 0x0803C0D0
	push {r4, r5, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	bne _0803C1CC
	ldr r5, _0803C0F8 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r4, r5, r2
	ldrb r0, [r4]
	cmp r0, #0x7E
	beq _0803C150
	cmp r0, #0x7E
	bgt _0803C0FC
	cmp r0, #0x7D
	beq _0803C17C
	b _0803C1CC
	.align 2, 0
_0803C0F8: .4byte 0x02017A40
_0803C0FC:
	cmp r0, #0x7F
	beq _0803C12A
	cmp r0, #0x80
	bne _0803C1CC
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803C13C @ =0x000005F4
	mov r2, #0
	bl CollectEffectTargets
	ldr r3, _0803C140 @ =0x000003E1
	add r1, r5, r3
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	bls _0803C1CC
	mov r0, #2
	strb r0, [r1]
	ldrb r0, [r4]
	sub r0, #1
	strb r0, [r4]
_0803C12A:
	ldr r0, _0803C144 @ =0x00000206
	ldr r1, _0803C148 @ =0x00000613
	ldr r3, _0803C14C @ =0x08083A6C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7E
	b _0803C1CE
	.align 2, 0
_0803C13C: .4byte 0x000005F4
_0803C140: .4byte 0x000003E1
_0803C144: .4byte 0x00000206
_0803C148: .4byte 0x00000613
_0803C14C: .4byte gStrSelectFusionMaterialToAddToHand
_0803C150:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803C174 @ =0x000007FF
	ldrh r3, [r3]
	and r2, r3
	lsl r2, r2, #1
	ldr r3, _0803C178 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7D
	b _0803C1CE
	.align 2, 0
_0803C174: .4byte 0x000007FF
_0803C178: .4byte gCardIdToNumber
_0803C17C:
	ldr r0, _0803C1BC @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r2, [r0, #6]
	add r1, r2, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r3, [r3, #2]
	and r0, r3
	mov r3, #0xD2
	cmp r0, #0
	beq _0803C19C
	ldr r3, _0803C1C0 @ =0x000080D2
_0803C19C:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r3, _0803C1C4 @ =0x000003E1
	add r1, r5, r3
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	cmp r0, #0
	beq _0803C1C8
	mov r0, #0x7F
	b _0803C1CE
_0803C1BC: .4byte 0x0201D810
_0803C1C0: .4byte 0x000080D2
_0803C1C4: .4byte 0x000003E1
_0803C1C8:
	mov r0, #0xA
	b _0803C1CE
_0803C1CC:
	mov r0, #0
_0803C1CE:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectReturnFusionMaterialsToHandResolve

