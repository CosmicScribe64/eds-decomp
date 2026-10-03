	thumb_func_start EffectEventResponsePrepare
EffectEventResponsePrepare: @ 0x0802EBB8
	push {r4, r5, r6, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	ldrb r5, [r4, #6]
	ldrb r6, [r4, #8]
	cmp r2, #0
	beq _0802EBC8
	b _0802EE0E
_0802EBC8:
	ldr r2, _0802EBFC @ =0x000007FF
	add r0, r2, #0
	ldrh r3, [r4]
	and r0, r3
	lsl r0, r0, #1
	ldr r3, _0802EC00 @ =0x08622AB4
	add r0, r0, r3
	ldrh r3, [r0]
	ldr r0, _0802EC04 @ =0x00000473
	cmp r3, r0
	bne _0802EBE0
	b _0802ED20
_0802EBE0:
	cmp r3, r0
	bgt _0802EC1C
	sub r0, #3
	cmp r3, r0
	beq _0802EC9E
	cmp r3, r0
	bgt _0802EC08
	sub r0, #2
	cmp r3, r0
	beq _0802EC66
	add r0, #1
	cmp r3, r0
	beq _0802EC82
	b _0802EE0E
_0802EBFC: .4byte 0x000007FF
_0802EC00: .4byte gCardIdToNumber
_0802EC04: .4byte 0x00000473
_0802EC08:
	ldr r0, _0802EC18 @ =0x00000471
	cmp r3, r0
	beq _0802ECAE
	add r0, #1
	cmp r3, r0
	beq _0802ECEC
	b _0802EE0E
	.align 2, 0
_0802EC18: .4byte 0x00000471
_0802EC1C:
	ldr r0, _0802EC3C @ =0x00000476
	cmp r3, r0
	bne _0802EC24
	b _0802EDA2
_0802EC24:
	cmp r3, r0
	bgt _0802EC40
	sub r0, #2
	cmp r3, r0
	bne _0802EC30
	b _0802ED44
_0802EC30:
	add r0, #1
	cmp r3, r0
	bne _0802EC38
	b _0802ED76
_0802EC38:
	b _0802EE0E
	.align 2, 0
_0802EC3C: .4byte 0x00000476
_0802EC40:
	ldr r0, _0802EC58 @ =0x00000515
	cmp r3, r0
	bne _0802EC48
	b _0802EDE8
_0802EC48:
	cmp r3, r0
	bgt _0802EC5C
	sub r0, #0x98
	cmp r3, r0
	bne _0802EC54
	b _0802EDD2
_0802EC54:
	b _0802EE0E
	.align 2, 0
_0802EC58: .4byte 0x00000515
_0802EC5C:
	mov r0, #0xA3
	lsl r0, r0, #3
	cmp r3, r0
	beq _0802ED44
	b _0802EE0E
_0802EC66:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r5, r0
	beq _0802EC72
	b _0802EE0E
_0802EC72:
	mov r1, #0
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x50
	beq _0802EC80
	b _0802EDCE
_0802EC80:
	b _0802EDCC
_0802EC82:
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	cmp r5, r0
	beq _0802EC8E
	b _0802EE0E
_0802EC8E:
	mov r1, #0
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x54
	beq _0802EC9C
	b _0802EDCE
_0802EC9C:
	b _0802EDCC
_0802EC9E:
	mov r1, #0
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x64
	beq _0802ECAC
	b _0802EDCE
_0802ECAC:
	b _0802EDCC
_0802ECAE:
	cmp r1, #0
	beq _0802ECD8
	add r0, r2, #0
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r3, _0802ECE8 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802ECD8
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #2
	beq _0802ED60
_0802ECD8:
	mov r1, #0
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x60
	bne _0802EDCE
	b _0802EDCC
	.align 2, 0
_0802ECE8: .4byte gCardStats
_0802ECEC:
	cmp r1, #0
	bne _0802ECF2
	b _0802EE0E
_0802ECF2:
	add r0, r2, #0
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802ED1C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0802ED0E
	b _0802EE0E
_0802ED0E:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #4
	beq _0802ED60
	b _0802EE0E
_0802ED1C: .4byte gCardStats
_0802ED20:
	cmp r1, #0
	beq _0802EE0E
	add r0, r2, #0
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _0802ED40 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0802EE0E
	b _0802ED0E
_0802ED40: .4byte gCardStats
_0802ED44:
	ldrb r3, [r4, #3]
	lsr r0, r3, #2
	cmp r0, #0xD
	blt _0802EE0E
	cmp r0, #0xE
	ble _0802ED64
	cmp r0, #0xF
	bne _0802EE0E
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrb r4, [r4, #8]
	cmp r4, r0
	bne _0802EE0E
_0802ED60:
	mov r0, #1
	b _0802EE10
_0802ED64:
	mov r1, #0xF
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	ldrb r4, [r4, #8]
	and r1, r4
	lsr r0, r0, #0x1F
	cmp r1, r0
	beq _0802ED60
	b _0802EE0E
_0802ED76:
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	cmp r5, r0
	beq _0802EE0E
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r2, #1
	neg r2, r2
	add r1, r3, #0
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	bgt _0802EE0E
	mov r1, #0
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x68
	bne _0802EDCE
	b _0802EDCC
_0802EDA2:
	ldrb r2, [r4, #2]
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	cmp r5, r0
	bne _0802EE0E
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r2, #1
	neg r2, r2
	add r1, r3, #0
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	bgt _0802EE0E
	mov r1, #0
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x74
	bne _0802EDCE
_0802EDCC:
	mov r1, #1
_0802EDCE:
	add r0, r1, #0
	b _0802EE10
_0802EDD2:
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r5, r0
	bne _0802EE0E
	mov r0, #0xFC
	ldrb r4, [r4, #3]
	and r0, r4
	cmp r0, #0x6C
	beq _0802ED60
	b _0802EE0E
_0802EDE8:
	ldrb r1, [r4, #3]
	lsr r0, r1, #2
	cmp r0, #0x13
	beq _0802EE00
	cmp r0, #0x1E
	bne _0802EE0E
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	cmp r5, r0
	bne _0802EE0E
	b _0802ED60
_0802EE00:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r0, r1, #0x1F
	cmp r5, r0
	beq _0802ED60
	cmp r6, r0
	beq _0802ED60
_0802EE0E:
	mov r0, #0
_0802EE10:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectEventResponsePrepare
	.align 2, 0

