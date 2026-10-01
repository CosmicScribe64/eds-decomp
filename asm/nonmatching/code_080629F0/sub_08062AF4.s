	thumb_func_start sub_08062AF4
sub_08062AF4: @ 0x08062AF4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1C
	mov sl, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, #0
	mov r9, r0
	cmp r1, #0x67
	bne _08062B10
	b _08062C98
_08062B10:
	cmp r1, #0x67
	bgt _08062B1A
	cmp r1, #0x66
	beq _08062BD0
	b _08062D78
_08062B1A:
	cmp r1, #0x6E
	beq _08062B20
	b _08062D78
_08062B20:
	mov r6, #0
_08062B22:
	mov r1, #1
	mov r9, r1
	add r2, r6, #1
	str r2, [sp, #0x14]
	lsl r0, r6, #1
	add r0, sl
	str r0, [sp, #8]
_08062B30:
	mov r3, #0
	mov r8, r3
	bl sub_08076F9C
	ldr r1, _08062BC0 @ =0x00000335
	bl __modsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	ldr r7, _08062BC4 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _08062BC8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _08062BCC @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08062B94
	mov r3, #1
	mov r8, r3
	cmp r6, #0
	ble _08062B94
	mov ip, r7
	add r5, r1, #0
	add r0, r4, #0
	mov r1, ip
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r3, [r0]
	mov r1, sl
	add r2, r6, #0
_08062B78:
	mov r0, ip
	ldrh r7, [r1]
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r3
	bne _08062B8C
	mov r0, #0
	mov r8, r0
_08062B8C:
	add r1, #2
	sub r2, #1
	cmp r2, #0
	bne _08062B78
_08062B94:
	mov r1, r8
	cmp r1, #0
	beq _08062BB0
	mov r2, #0
	mov r9, r2
	add r0, r4, #0
	ldr r3, _08062BC4 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r7, _08062BC8 @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	ldr r1, [sp, #8]
	strh r0, [r1]
_08062BB0:
	mov r2, r9
	cmp r2, #0
	bne _08062B30
	ldr r6, [sp, #0x14]
	cmp r6, #4
	ble _08062B22
	b _08062D48
	.align 2, 0
_08062BC0: .4byte 0x00000335
_08062BC4: .4byte 0x000007FF
_08062BC8: .4byte gUnk_08622AB4
_08062BCC: .4byte 0xFFFFF880
_08062BD0:
	mov r6, #0
_08062BD2:
	mov r7, #1
	mov r9, r7
	add r0, r6, #1
	str r0, [sp, #0x14]
	lsl r0, r6, #1
	add r0, sl
	str r0, [sp, #0xC]
_08062BE0:
	mov r1, #0
	mov r8, r1
	bl sub_08076F9C
	ldr r1, _08062C84 @ =0x00000335
	bl __modsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r1, r4, #0
	ldr r2, _08062C88 @ =0x000007FF
	and r1, r2
	lsl r0, r1, #1
	ldr r3, _08062C8C @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	ldr r7, _08062C90 @ =0xFFFFF880
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08062C58
	lsl r0, r1, #2
	ldr r1, _08062C94 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08062C58
	mov r2, #1
	mov r8, r2
	cmp r6, #0
	ble _08062C58
	ldr r3, _08062C88 @ =0x000007FF
	mov ip, r3
	ldr r5, _08062C8C @ =0x08622AB4
	add r0, r4, #0
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r3, [r0]
	mov r1, sl
	add r2, r6, #0
_08062C3C:
	mov r0, ip
	ldrh r7, [r1]
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r3
	bne _08062C50
	mov r0, #0
	mov r8, r0
_08062C50:
	add r1, #2
	sub r2, #1
	cmp r2, #0
	bne _08062C3C
_08062C58:
	mov r1, r8
	cmp r1, #0
	beq _08062C74
	mov r2, #0
	mov r9, r2
	add r0, r4, #0
	ldr r3, _08062C88 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r7, _08062C8C @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	ldr r1, [sp, #0xC]
	strh r0, [r1]
_08062C74:
	mov r2, r9
	cmp r2, #0
	bne _08062BE0
	ldr r6, [sp, #0x14]
	cmp r6, #4
	ble _08062BD2
	b _08062D48
	.align 2, 0
_08062C84: .4byte 0x00000335
_08062C88: .4byte 0x000007FF
_08062C8C: .4byte gUnk_08622AB4
_08062C90: .4byte 0xFFFFF880
_08062C94: .4byte gUnk_08621DE0
_08062C98:
	mov r6, #0
_08062C9A:
	mov r7, #1
	mov r9, r7
	add r0, r6, #1
	str r0, [sp, #0x14]
	lsl r0, r6, #1
	add r0, sl
	str r0, [sp, #0x10]
_08062CA8:
	mov r1, #0
	mov r8, r1
	bl sub_08076F9C
	ldr r1, _08062D5C @ =0x00000335
	bl __modsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r1, r4, #0
	ldr r2, _08062D60 @ =0x000007FF
	and r1, r2
	lsl r0, r1, #1
	ldr r3, _08062D64 @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	ldr r7, _08062D68 @ =0xFFFFF880
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08062D20
	lsl r0, r1, #2
	ldr r1, _08062D6C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08062D20
	mov r2, #1
	mov r8, r2
	cmp r6, #0
	ble _08062D20
	ldr r3, _08062D60 @ =0x000007FF
	mov ip, r3
	ldr r5, _08062D64 @ =0x08622AB4
	add r0, r4, #0
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r3, [r0]
	mov r1, sl
	add r2, r6, #0
_08062D04:
	mov r0, ip
	ldrh r7, [r1]
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r3
	bne _08062D18
	mov r0, #0
	mov r8, r0
_08062D18:
	add r1, #2
	sub r2, #1
	cmp r2, #0
	bne _08062D04
_08062D20:
	mov r1, r8
	cmp r1, #0
	beq _08062D3C
	mov r2, #0
	mov r9, r2
	add r0, r4, #0
	ldr r3, _08062D60 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r7, _08062D64 @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	ldr r1, [sp, #0x10]
	strh r0, [r1]
_08062D3C:
	mov r2, r9
	cmp r2, #0
	bne _08062CA8
	ldr r6, [sp, #0x14]
	cmp r6, #4
	ble _08062C9A
_08062D48:
	ldr r0, _08062D70 @ =0x02015160
	mov r3, #0x89
	lsl r3, r3, #1
	add r1, r0, r3
	ldr r0, _08062D74 @ =0x0000270F
	strh r0, [r1]
	mov r0, #1
	neg r0, r0
	b _08062ED0
	.align 2, 0
_08062D5C: .4byte 0x00000335
_08062D60: .4byte 0x000007FF
_08062D64: .4byte gUnk_08622AB4
_08062D68: .4byte 0xFFFFF880
_08062D6C: .4byte gUnk_08621DE0
_08062D70: .4byte 0x02015160
_08062D74: .4byte 0x0000270F
_08062D78:
	mov r6, #0
	ldr r0, _08062D9C @ =0x081A562C
_08062D7C:
	ldrh r7, [r0, #4]
	cmp r7, r1
	bne _08062D86
	ldr r2, [r0]
	mov r9, r2
_08062D86:
	add r0, #8
	add r6, #1
	cmp r6, #0x1B
	bls _08062D7C
	mov r3, r9
	cmp r3, #0
	bne _08062DA0
	mov r0, #1
	neg r0, r0
	b _08062ED0
	.align 2, 0
_08062D9C: .4byte gUnk_081A562C
_08062DA0:
	mov r0, r9
	bl sub_08062A0C
	str r0, [sp, #0]
	mov r0, r9
	bl sub_080629F0
	str r0, [sp, #4]
	mov r0, r9
	ldr r1, [sp, #0]
	bl sub_08062AD4
	mov r7, sl
	strh r0, [r7]
	mov r6, #0
	ldr r0, [sp, #4]
	lsl r3, r0, #3
	mov r1, r9
	add r0, r3, r1
	ldr r0, [r0, #4]
	cmp r6, r0
	bge _08062DE8
	ldr r4, _08062EE0 @ =0x02015160
	add r4, #2
_08062DD0:
	lsl r1, r6, #1
	mov r7, r9
	add r2, r3, r7
	ldr r0, [r2]
	add r1, r1, r0
	ldrh r0, [r1]
	strh r0, [r4]
	add r4, #2
	add r6, #1
	ldr r0, [r2, #4]
	cmp r6, r0
	blt _08062DD0
_08062DE8:
	mov r6, #0
	ldr r0, [sp, #4]
	lsl r1, r0, #3
	mov r3, r9
	add r2, r1, r3
	ldr r0, [r2, #4]
	lsl r0, r0, #1
	str r1, [sp, #0x18]
	cmp r6, r0
	bge _08062E30
	add r5, r2, #0
	ldr r7, _08062EE4 @ =0x02015162
_08062E00:
	bl sub_08076F9C
	ldr r1, [r5, #4]
	bl __modsi3
	add r4, r0, #0
	bl sub_08076F9C
	ldr r1, [r5, #4]
	bl __modsi3
	lsl r4, r4, #1
	add r4, r4, r7
	ldrh r2, [r4]
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r1, [r0]
	strh r1, [r4]
	strh r2, [r0]
	add r6, #1
	ldr r0, [r5, #4]
	lsl r0, r0, #1
	cmp r6, r0
	blt _08062E00
_08062E30:
	mov r2, #0
	ldr r7, _08062EE0 @ =0x02015160
	add r7, #2
	mov r8, r7
	mov r7, sl
	add r7, #2
	ldr r0, [sp, #0x18]
	add r9, r0
	mov r6, #3
_08062E42:
	lsl r0, r2, #1
	add r0, r8
	ldrh r0, [r0]
	mov r1, sl
	ldrh r1, [r1]
	cmp r0, r1
	bne _08062E6C
	mov r3, r9
	ldr r5, [r3, #4]
	add r4, r0, #0
_08062E56:
	add r2, #1
	add r0, r2, #0
	add r1, r5, #0
	bl __modsi3
	add r2, r0, #0
	lsl r0, r2, #1
	add r0, r8
	ldrh r0, [r0]
	cmp r0, r4
	beq _08062E56
_08062E6C:
	lsl r0, r2, #1
	add r0, r8
	ldrh r0, [r0]
	strh r0, [r7]
	add r2, #1
	mov r0, r9
	ldr r1, [r0, #4]
	add r0, r2, #0
	bl __modsi3
	add r2, r0, #0
	add r7, #2
	sub r6, #1
	cmp r6, #0
	bge _08062E42
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	cmp r1, r2
	beq _08062EA0
	mov r3, sl
	ldrh r0, [r3]
	ldr r7, _08062EE0 @ =0x02015160
	mov r2, #0x89
	lsl r2, r2, #1
	add r1, r7, r2
	strh r0, [r1]
_08062EA0:
	mov r6, #0x18
_08062EA2:
	bl sub_08076F9C
	mov r1, #5
	bl __modsi3
	add r4, r0, #0
	bl sub_08076F9C
	mov r1, #5
	bl __modsi3
	lsl r4, r4, #1
	add r4, sl
	ldrh r2, [r4]
	lsl r0, r0, #1
	add r0, sl
	ldrh r1, [r0]
	strh r1, [r4]
	strh r2, [r0]
	sub r6, #1
	cmp r6, #0
	bge _08062EA2
	ldr r0, [sp, #0]
_08062ED0:
	add sp, #0x1C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08062EE0: .4byte 0x02015160
_08062EE4: .4byte 0x02015162
	thumb_func_end sub_08062AF4

