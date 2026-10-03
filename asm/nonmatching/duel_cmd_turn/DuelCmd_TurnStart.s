	thumb_func_start DuelCmd_TurnStart
DuelCmd_TurnStart: @ 0x08013CDC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _08013E34 @ =0x020185C0
	ldrh r0, [r0]
	lsr r7, r0, #0xF
	ldr r1, _08013E38 @ =0x020192E4
	mov r4, #1
	add r0, r7, #0
	and r0, r4
	ldr r5, _08013E3C @ =0x00000D64
	mul r0, r5
	add r3, r0, r1
	ldrb r2, [r3, #0xB]
	lsl r0, r2, #0x1D
	add r6, r1, #0
	cmp r0, #0
	beq _08013D16
	lsr r0, r0, #0x1D
	sub r0, #1
	mov r1, #7
	and r0, r1
	mov r1, #8
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3, #0xB]
_08013D16:
	sub r0, r4, r7
	and r0, r4
	add r2, r0, #0
	mul r2, r5
	add r2, r2, r6
	mov r1, #9
	neg r1, r1
	add r0, r1, #0
	ldrb r4, [r2, #8]
	and r0, r4
	strb r0, [r2, #8]
	ldrb r0, [r3, #8]
	and r1, r0
	strb r1, [r3, #8]
	mov r4, #0
	mov r9, r6
	mov r1, #1
	mov r8, r1
_08013D3A:
	add r0, r4, #0
	mov r2, r8
	and r0, r2
	mul r0, r5
	mov r1, r9
	add r3, r0, r1
	ldrb r2, [r3, #7]
	lsl r1, r2, #0x18
	lsr r0, r1, #0x1E
	cmp r0, #0
	beq _08013D5E
	add r1, r0, #0
	sub r1, #1
	lsl r1, r1, #6
	mov r0, #0x3F
	and r0, r2
	orr r0, r1
	strb r0, [r3, #7]
_08013D5E:
	add r4, #1
	cmp r4, #1
	ble _08013D3A
	add r1, r6, #0
	ldrb r2, [r1, #7]
	mov r3, #0xC0
	add r0, r3, #0
	and r0, r2
	cmp r0, #0
	beq _08013D80
	ldr r4, _08013E40 @ =0x00000D6B
	add r1, r1, r4
	add r0, r3, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08013D92
_08013D80:
	mov r0, #0x3F
	add r1, r0, #0
	and r1, r2
	strb r1, [r6, #7]
	ldr r5, _08013E40 @ =0x00000D6B
	add r1, r6, r5
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
_08013D92:
	mov r0, #0
	mov r9, r0
	ldr r1, _08013E44 @ =0x0201930C
	mov sl, r1
	mov r2, #0x3D
	neg r2, r2
	mov r8, r2
_08013DA0:
	ldr r4, _08013E3C @ =0x00000D64
	add r3, r7, #0
	mul r3, r4
	mov r5, sl
	add r2, r3, r5
	mov r0, #0x94
	mov r1, r9
	mul r1, r0
	add r4, r2, r1
	mov r6, #0xB9
	lsl r6, r6, #2
	add r0, r1, r6
	add r6, r2, r0
	add r1, r1, r3
	add r0, r1, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldr r0, _08013E48 @ =0x020195F0
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r3, #0
	bne _08013DD4
	b _08013F2C
_08013DD4:
	mov r1, #5
	neg r1, r1
	add r0, r1, #0
	ldrb r2, [r4, #7]
	and r0, r2
	strb r0, [r4, #7]
	ldrb r2, [r4, #6]
	mov r0, #2
	and r0, r2
	cmp r0, #0
	bne _08013DEC
	b _08013F2C
_08013DEC:
	lsl r1, r2, #0x1A
	lsr r0, r1, #0x1C
	cmp r0, #0xE
	bhi _08013E04
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	mov r1, r8
	and r1, r2
	orr r1, r0
	strb r1, [r4, #6]
_08013E04:
	ldr r0, _08013E4C @ =0x000007FF
	add r1, r0, #0
	add r0, r3, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08013E50 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08013E54 @ =0x000002DA
	cmp r1, r0
	beq _08013EFC
	cmp r1, r0
	bgt _08013E6C
	mov r0, #0xD6
	lsl r0, r0, #1
	cmp r1, r0
	beq _08013EFC
	cmp r1, r0
	bgt _08013E58
	cmp r1, #0xF
	beq _08013EFC
	cmp r1, #0x52
	beq _08013E9C
	b _08013F2C
_08013E34: .4byte 0x020185C0
_08013E38: .4byte 0x020192E4
_08013E3C: .4byte 0x00000D64
_08013E40: .4byte 0x00000D6B
_08013E44: .4byte 0x0201930C
_08013E48: .4byte 0x020195F0
_08013E4C: .4byte 0x000007FF
_08013E50: .4byte gCardIdToNumber
_08013E54: .4byte 0x000002DA
_08013E58:
	ldr r0, _08013E68 @ =0x00000243
	cmp r1, r0
	beq _08013EFC
	add r0, #0x24
	cmp r1, r0
	beq _08013ED2
	b _08013F2C
	.align 2, 0
_08013E68: .4byte 0x00000243
_08013E6C:
	ldr r0, _08013E84 @ =0x00000536
	cmp r1, r0
	beq _08013EFC
	cmp r1, r0
	bgt _08013E8C
	ldr r0, _08013E88 @ =0x000002E6
	cmp r1, r0
	beq _08013EFC
	mov r0, #0x8B
	lsl r0, r0, #3
	b _08013E96
	.align 2, 0
_08013E84: .4byte 0x00000536
_08013E88: .4byte 0x000002E6
_08013E8C:
	mov r0, #0xA8
	lsl r0, r0, #3
	cmp r1, r0
	beq _08013F18
	add r0, #0xA9
_08013E96:
	cmp r1, r0
	beq _08013EFC
	b _08013F2C
_08013E9C:
	mov r0, #0x20
	ldrb r2, [r4, #7]
	and r0, r2
	cmp r0, #0
	bne _08013EBC
	ldrb r0, [r4, #6]
	lsl r1, r0, #0x1A
	lsr r1, r1, #0x1C
	add r1, #1
	mov r2, #0xF
	and r1, r2
	lsl r1, r1, #2
	mov r2, r8
	and r0, r2
	orr r0, r1
	strb r0, [r4, #6]
_08013EBC:
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #6
	bls _08013F2C
	mov r0, r8
	and r0, r1
	mov r1, #0x18
	orr r0, r1
	strb r0, [r4, #6]
	b _08013F2C
_08013ED2:
	ldrb r2, [r4, #6]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1C
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	mov r3, r8
	add r1, r3, #0
	and r1, r2
	orr r1, r0
	strb r1, [r4, #6]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #4
	bls _08013F2C
	and r1, r3
	mov r0, #0x10
	orr r1, r0
	strb r1, [r4, #6]
	b _08013F2C
_08013EFC:
	mov r0, #0x20
	ldrb r1, [r4, #7]
	orr r0, r1
	strb r0, [r4, #7]
	mov r4, r9
	lsl r2, r4, #0x18
	lsr r2, r2, #0x10
	orr r2, r7
	add r0, r7, #0
	add r1, r3, #0
	mov r3, #0xB
	bl QueueRemoveZoneLink
	b _08013F2C
_08013F18:
	ldrb r1, [r4, #7]
	mov r2, #0x20
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	bne _08013F2C
	mov r0, #4
	orr r0, r1
	orr r0, r2
	strb r0, [r4, #7]
_08013F2C:
	cmp r5, #0
	beq _08013F96
	ldrb r2, [r6, #6]
	mov r0, #2
	and r0, r2
	cmp r0, #0
	beq _08013F70
	ldr r1, _08013F68 @ =0x000007FF
	add r0, r1, #0
	and r5, r0
	lsl r0, r5, #1
	ldr r4, _08013F6C @ =0x08622AB4
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, #0x47
	bne _08013F96
	lsl r1, r2, #0x1A
	lsr r0, r1, #0x1C
	cmp r0, #0xB
	bhi _08013F96
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	mov r1, r8
	and r1, r2
	orr r1, r0
	strb r1, [r6, #6]
	b _08013F96
	.align 2, 0
_08013F68: .4byte 0x000007FF
_08013F6C: .4byte gCardIdToNumber
_08013F70:
	ldr r1, _08014000 @ =0x000007FF
	add r0, r1, #0
	and r5, r0
	lsl r0, r5, #2
	ldr r2, _08014004 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08013F96
	add r1, r6, #0
	add r1, #0x91
	mov r0, #4
	ldrb r4, [r1]
	orr r0, r4
	strb r0, [r1]
_08013F96:
	mov r5, #1
	add r9, r5
	mov r6, r9
	cmp r6, #4
	bgt _08013FA2
	b _08013DA0
_08013FA2:
	mov r4, #0
	ldr r3, _08014008 @ =0x020192E0
	ldr r1, _0801400C @ =0x00000D64
	add r0, r7, #0
	mul r0, r1
	add r2, r0, r3
	ldr r5, _08014010 @ =0x020185C0
	ldrb r0, [r2, #0xA]
	cmp r4, r0
	bge _08013FE4
	mul r1, r7
	ldr r6, _08014014 @ =0x00000CC8
	add r0, r3, r6
	add r3, r1, r0
_08013FBE:
	ldrh r0, [r3]
	ldrb r1, [r3]
	cmp r1, #2
	bne _08013FDA
	lsr r1, r0, #8
	add r0, r1, #0
	cmp r0, #4
	bhi _08013FDA
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	mov r1, #2
	orr r0, r1
	strh r0, [r3]
_08013FDA:
	add r3, #2
	add r4, #1
	ldrb r6, [r2, #0xA]
	cmp r4, r6
	blt _08013FBE
_08013FE4:
	ldr r0, _08014018 @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08014000: .4byte 0x000007FF
_08014004: .4byte gCardStats
_08014008: .4byte 0x020192E0
_0801400C: .4byte 0x00000D64
_08014010: .4byte 0x020185C0
_08014014: .4byte 0x00000CC8
_08014018: .4byte 0x0000080D
	thumb_func_end DuelCmd_TurnStart

