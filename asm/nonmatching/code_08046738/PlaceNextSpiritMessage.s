	thumb_func_start PlaceNextSpiritMessage
PlaceNextSpiritMessage: @ 0x08046D3C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r4, r1, #0
	bl FindFreeSpellTrapZone
	mov r8, r0
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mul r0, r4
	ldr r1, _08046D74 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08046D78 @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #1
	beq _08046D90
	cmp r0, #1
	bgt _08046D7C
	cmp r0, #0
	beq _08046D86
	b _08046E7E
_08046D74: .4byte 0x00000D64
_08046D78: .4byte 0x0201930C
_08046D7C:
	cmp r0, #2
	beq _08046D98
	cmp r0, #3
	beq _08046DA0
	b _08046E7E
_08046D86:
	ldr r5, _08046D8C @ =0x00000605
	b _08046DA4
	.align 2, 0
_08046D8C: .4byte 0x00000605
_08046D90:
	ldr r5, _08046D94 @ =0x00000606
	b _08046DA4
_08046D94: .4byte 0x00000606
_08046D98:
	ldr r5, _08046D9C @ =0x00000607
	b _08046DA4
_08046D9C: .4byte 0x00000607
_08046DA0:
	mov r5, #0xC1
	lsl r5, r5, #3
_08046DA4:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, r8
	bl PlaceDeckCardOnField
	cmp r0, #0
	beq _08046DF0
	ldr r0, _08046DBC @ =0x0000FFFF
	cmp r5, r0
	bne _08046DC0
	mov r0, #0
	b _08046DE6
_08046DBC: .4byte 0x0000FFFF
_08046DC0:
	ldr r0, _08046DD0 @ =0x000007CF
	cmp r5, r0
	bhi _08046DD8
	lsl r0, r5, #1
	ldr r1, _08046DD4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08046DE6
_08046DD0: .4byte 0x000007CF
_08046DD4: .4byte gCardNumberToId
_08046DD8:
	mov r0, #0x30
	orr r5, r0
	lsl r0, r5, #1
	ldr r4, _08046DEC @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	add r0, #1
_08046DE6:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	b _08046E54
_08046DEC: .4byte gCardNumberToId
_08046DF0:
	mov r2, #0
	ldr r4, _08046E5C @ =0x020192E4
	mov r0, #1
	and r0, r6
	ldr r1, _08046E60 @ =0x00000D64
	mul r1, r0
	add r3, r1, r4
	ldrb r7, [r3, #2]
	cmp r2, r7
	bge _08046E7E
	ldr r7, _08046E64 @ =0x00000684
	add r0, r4, r7
	mov r4, #0xF
	mov r9, r4
	mov r7, r8
	mov r4, r9
	and r7, r4
	mov ip, r7
	add r4, r1, r0
_08046E16:
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	ldr r0, _08046E68 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r7, _08046E6C @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r0, r5
	bne _08046E74
	mov r3, #0xC5
	cmp r6, #0
	beq _08046E34
	ldr r3, _08046E70 @ =0x000080C5
_08046E34:
	mov r0, r9
	and r2, r0
	lsl r2, r2, #4
	mov r5, ip
	orr r2, r5
	mov r7, #0x80
	lsl r7, r7, #1
	add r0, r7, #0
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
_08046E54:
	add r0, r6, #0
	bl ShowCardEffect
	b _08046E7E
_08046E5C: .4byte 0x020192E4
_08046E60: .4byte 0x00000D64
_08046E64: .4byte 0x00000684
_08046E68: .4byte 0x000007FF
_08046E6C: .4byte gCardIdToNumber
_08046E70: .4byte 0x000080C5
_08046E74:
	add r4, #4
	add r2, #1
	ldrb r0, [r3, #2]
	cmp r2, r0
	blt _08046E16
_08046E7E:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end PlaceNextSpiritMessage
	.align 2, 0

