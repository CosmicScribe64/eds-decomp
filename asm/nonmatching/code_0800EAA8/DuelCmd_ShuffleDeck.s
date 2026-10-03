	thumb_func_start DuelCmd_ShuffleDeck
DuelCmd_ShuffleDeck: @ 0x0800EF38
	push {r4, lr}
	ldr r0, _0800EF5C @ =0x020185C0
	ldrh r1, [r0]
	lsr r4, r1, #0xF
	ldr r2, _0800EF60 @ =0x0000080A
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x19
	add r2, r0, #0
	cmp r1, #0xA
	bls _0800EF52
	b _0800F0D6
_0800EF52:
	lsl r0, r1, #2
	ldr r1, _0800EF64 @ =0x0800EF68
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0800EF5C: .4byte 0x020185C0
_0800EF60: .4byte 0x0000080A
_0800EF64: .4byte 0x0800EF68
_0800EF68:
	.4byte _0800EF94
	.4byte _0800EFC0
	.4byte _0800EFEA
	.4byte _0800F068
	.4byte _0800F0D6
	.4byte _0800F0D6
	.4byte _0800F0D6
	.4byte _0800F0D6
	.4byte _0800F0D6
	.4byte _0800F0D6
	.4byte _0800F0C8
_0800EF94:
	mov r0, #0x50
	bl DuelScreen_StartScroll
	ldr r2, _0800EFB8 @ =0x020185C0
	ldr r0, _0800EFBC @ =0x0000080A
	add r2, r2, r0
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _0800F0E4
_0800EFB8: .4byte 0x020185C0
_0800EFBC: .4byte 0x0000080A
_0800EFC0:
	ldr r0, _0800F038 @ =0x08694EA8
	bl DuelSprAnim_Load
	ldr r2, _0800F03C @ =0x020185C0
	ldr r1, _0800F040 @ =0x0000080A
	add r2, r2, r1
	ldr r0, _0800F044 @ =0xFFFFC07F
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
_0800EFEA:
	add r0, r4, #0
	mov r1, #3
	bl ShuffleDeck
	mov r0, #0
	mov r1, #0
	mov r2, #1
	bl DuelSprAnim_DrawAt
	ldr r0, _0800F048 @ =0x0201CFB0
	ldr r2, _0800F04C @ =0x00000852
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, #0
	bne _0800F0E4
	ldr r0, _0800F03C @ =0x020185C0
	ldr r1, _0800F040 @ =0x0000080A
	add r3, r0, r1
	ldrh r2, [r3]
	lsl r1, r2, #0x12
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #7
	ldr r0, _0800F044 @ =0xFFFFC07F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	lsl r0, r0, #0x12
	lsr r0, r0, #0x19
	cmp r0, #3
	bgt _0800F050
	mov r0, #7
	bl PlaySE
	bl DuelSprAnim_Rewind
	b _0800F0E4
_0800F038: .4byte gDeckShuffleAnim
_0800F03C: .4byte 0x020185C0
_0800F040: .4byte 0x0000080A
_0800F044: .4byte 0xFFFFC07F
_0800F048: .4byte 0x0201CFB0
_0800F04C: .4byte 0x00000852
_0800F050:
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _0800F0E4
_0800F068:
	bl LoadDuelUiGfx
	ldr r1, _0800F0A4 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0800F0B8
	ldr r1, _0800F0A8 @ =0x020192E0
	ldr r2, _0800F0AC @ =0x00001B12
	add r1, r1, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800F0B8
	add r0, r4, #0
	bl DuelLink_SendDeck
	ldr r0, _0800F0B0 @ =0x020185C0
	ldr r1, _0800F0B4 @ =0x0000080A
	add r0, r0, r1
	mov r1, #0x80
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0xA
	orr r1, r2
	strb r1, [r0]
	b _0800F0E4
_0800F0A4: .4byte 0x02015EE8
_0800F0A8: .4byte 0x020192E0
_0800F0AC: .4byte 0x00001B12
_0800F0B0: .4byte 0x020185C0
_0800F0B4: .4byte 0x0000080A
_0800F0B8:
	ldr r1, _0800F0C0 @ =0x020185C0
	ldr r0, _0800F0C4 @ =0x0000080D
	add r1, r1, r0
	b _0800F0DA
_0800F0C0: .4byte 0x020185C0
_0800F0C4: .4byte 0x0000080D
_0800F0C8:
	ldr r0, _0800F0EC @ =0x02017FB0
	ldr r1, _0800F0F0 @ =0x00000305
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	bge _0800F0E4
_0800F0D6:
	ldr r0, _0800F0F4 @ =0x0000080D
	add r1, r2, r0
_0800F0DA:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800F0E4:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800F0EC: .4byte 0x02017FB0
_0800F0F0: .4byte 0x00000305
_0800F0F4: .4byte 0x0000080D
	thumb_func_end DuelCmd_ShuffleDeck

