	thumb_func_start sub_0805CB2C
sub_0805CB2C: @ 0x0805CB2C
	push {r4, r5, r6, lr}
	sub sp, #4
	ldr r0, _0805CB44 @ =0x02015EF0
	ldrb r0, [r0, #2]
	cmp r0, #5
	bls _0805CB3A
	b _0805CD98
_0805CB3A:
	lsl r0, r0, #2
	ldr r1, _0805CB48 @ =0x0805CB4C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0805CB44: .4byte 0x02015EF0
_0805CB48: .4byte 0x0805CB4C
_0805CB4C:
	.4byte _0805CB64
	.4byte _0805CC58
	.4byte _0805CC94
	.4byte _0805CCC4
	.4byte _0805CC94
	.4byte _0805CD58
_0805CB64:
	ldr r0, _0805CBA0 @ =0x0000013D
	bl sub_08059408
	cmp r0, #0
	beq _0805CB70
	b _0805CD84
_0805CB70:
	ldr r4, _0805CBA4 @ =0x020192E4
	ldr r0, _0805CBA8 @ =0x00000D6C
	add r1, r4, r0
	mov r0, #0x10
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805CB82
	b _0805CC78
_0805CB82:
	mov r0, #1
	bl sub_08008A44
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0805CBB4
	ldr r1, _0805CBAC @ =0x02015F00
	ldr r6, _0805CBB0 @ =0x00001B24
	add r1, r1, r6
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _0805CC86
_0805CBA0: .4byte 0x0000013D
_0805CBA4: .4byte 0x020192E4
_0805CBA8: .4byte 0x00000D6C
_0805CBAC: .4byte 0x02015F00
_0805CBB0: .4byte 0x00001B24
_0805CBB4:
	mov r0, #1
	bl sub_08008A44
	ldr r1, _0805CC34 @ =0x02015F00
	ldr r3, _0805CC38 @ =0x00001B25
	add r1, r1, r3
	strb r0, [r1]
	mov r1, #0
	ldr r6, _0805CC3C @ =0x00000D66
	add r0, r4, r6
	ldrb r0, [r0]
	cmp r1, r0
	bge _0805CBF6
	ldr r6, _0805CC40 @ =0x000004E1
	add r3, r0, #0
	ldr r0, _0805CC44 @ =0x000013E8
	add r2, r4, r0
	ldr r5, _0805CC48 @ =0x000007FF
	ldr r4, _0805CC4C @ =0x08622AB4
_0805CBDA:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r6
	bne _0805CBEE
	b _0805CD64
_0805CBEE:
	add r2, #4
	add r1, #1
	cmp r1, r3
	blt _0805CBDA
_0805CBF6:
	mov r1, #0
	ldr r2, _0805CC50 @ =0x020192E4
	ldr r3, _0805CC3C @ =0x00000D66
	add r0, r2, r3
	ldr r6, _0805CC34 @ =0x02015F00
	ldrb r0, [r0]
	cmp r1, r0
	bge _0805CC2E
	add r0, r2, r3
	ldrb r3, [r0]
	ldr r0, _0805CC44 @ =0x000013E8
	add r2, r2, r0
	ldr r5, _0805CC48 @ =0x000007FF
	ldr r4, _0805CC4C @ =0x08622AB4
_0805CC12:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, #0x3D
	bne _0805CC26
	b _0805CD74
_0805CC26:
	add r2, #4
	add r1, #1
	cmp r1, r3
	blt _0805CC12
_0805CC2E:
	ldr r2, _0805CC54 @ =0x00001B24
	add r1, r6, r2
	b _0805CC7E
_0805CC34: .4byte 0x02015F00
_0805CC38: .4byte 0x00001B25
_0805CC3C: .4byte 0x00000D66
_0805CC40: .4byte 0x000004E1
_0805CC44: .4byte 0x000013E8
_0805CC48: .4byte 0x000007FF
_0805CC4C: .4byte gUnk_08622AB4
_0805CC50: .4byte 0x020192E4
_0805CC54: .4byte 0x00001B24
_0805CC58:
	ldr r0, _0805CC6C @ =0x0000013D
	bl sub_08059408
	cmp r0, #0
	beq _0805CC78
	ldr r2, _0805CC70 @ =0x020192E0
	ldr r6, _0805CC74 @ =0x00001B30
	add r1, r2, r6
	b _0805CCE0
	.align 2, 0
_0805CC6C: .4byte 0x0000013D
_0805CC70: .4byte 0x020192E0
_0805CC74: .4byte 0x00001B30
_0805CC78:
	ldr r1, _0805CC8C @ =0x02015F00
	ldr r2, _0805CC90 @ =0x00001B24
	add r1, r1, r2
_0805CC7E:
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
_0805CC86:
	strb r0, [r1]
_0805CC88:
	mov r0, #0
	b _0805CD9A
_0805CC8C: .4byte 0x02015F00
_0805CC90: .4byte 0x00001B24
_0805CC94:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08049048
	ldr r1, _0805CCB8 @ =0x020192E0
	ldr r6, _0805CCBC @ =0x00001B2C
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805CC88
	ldr r1, _0805CCC0 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
	b _0805CC88
_0805CCB8: .4byte 0x020192E0
_0805CCBC: .4byte 0x00001B2C
_0805CCC0: .4byte 0x02015EF0
_0805CCC4:
	bl sub_080094E4
	mov r1, #0x8D
	lsl r1, r1, #3
	cmp r0, r1
	beq _0805CD58
	add r0, r1, #0
	bl sub_08059408
	cmp r0, #0
	beq _0805CD58
	ldr r2, _0805CD30 @ =0x020192E0
	ldr r0, _0805CD34 @ =0x00001B30
	add r1, r2, r0
_0805CCE0:
	ldr r0, _0805CD38 @ =0xFFFFFC03
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r5, _0805CD3C @ =0x02015EF0
	ldrb r6, [r5, #0xB]
	lsl r0, r6, #2
	ldr r3, _0805CD40 @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805CD44 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805CD48 @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805CD4C @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805CD50 @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805CD54 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
	b _0805CC88
_0805CD30: .4byte 0x020192E0
_0805CD34: .4byte 0x00001B30
_0805CD38: .4byte 0xFFFFFC03
_0805CD3C: .4byte 0x02015EF0
_0805CD40: .4byte 0x000013EC
_0805CD44: .4byte 0x00001B28
_0805CD48: .4byte 0x00001B33
_0805CD4C: .4byte 0x00001B34
_0805CD50: .4byte 0xFFFFFE01
_0805CD54: .4byte 0x00001B2C
_0805CD58:
	ldr r1, _0805CD60 @ =0x02015EF0
	mov r0, #0
	strb r0, [r1, #2]
	b _0805CD9A
_0805CD60: .4byte 0x02015EF0
_0805CD64:
	ldr r0, _0805CD6C @ =0x02015F00
	ldr r2, _0805CD70 @ =0x00001B25
	add r0, r0, r2
	b _0805CD78
_0805CD6C: .4byte 0x02015F00
_0805CD70: .4byte 0x00001B25
_0805CD74:
	ldr r3, _0805CD90 @ =0x00001B25
	add r0, r6, r3
_0805CD78:
	ldrb r2, [r0]
	mov r0, #1
	str r0, [sp, #0]
	mov r3, #0
	bl sub_08055B28
_0805CD84:
	ldr r1, _0805CD94 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
	b _0805CC88
	.align 2, 0
_0805CD90: .4byte 0x00001B25
_0805CD94: .4byte 0x02015EF0
_0805CD98:
	mov r0, #1
_0805CD9A:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0805CB2C
	.align 2, 0

