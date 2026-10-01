	thumb_func_start sub_08065AB4
sub_08065AB4: @ 0x08065AB4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x18]
	ldr r3, _08065BCC @ =0x0201DB20
	ldr r1, _08065BD0 @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r4, #0xC4
	lsl r4, r4, #3
	add r3, r3, r4
	add r2, r2, r3
	ldrh r2, [r2]
	bl sub_08068D1C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x1C]
	ldr r0, _08065BD4 @ =0x000007FF
	ldr r7, [sp, #0x1C]
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08065BD8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bge _08065B10
	b _08065D3C
_08065B10:
	cmp r0, #0x17
	bgt _08065B16
	b _08065E4E
_08065B16:
	cmp r0, #0x18
	beq _08065B1C
	b _08065D3C
_08065B1C:
	ldr r2, [sp, #0x18]
	lsl r0, r2, #5
	add r1, r6, r0
	lsl r1, r1, #1
	add r1, r8
	mov r3, #0xCC
	lsl r3, r3, #1
	add r2, r3, #0
	strh r2, [r1]
	mov r5, #0
	str r0, [sp, #0x24]
	add r4, r6, #1
	mov ip, r4
	add r7, r6, #2
	add r4, r6, #3
	add r3, r6, #4
	ldr r0, [sp, #0x18]
	add r0, #1
	str r0, [sp, #0x20]
	mov r1, #0x1F
	mov sl, r1
	mov r2, ip
	and r2, r1
	mov ip, r2
	and r7, r1
	str r7, [sp, #0x28]
	ldr r0, _08065BDC @ =0x00002230
	mov r9, r0
	and r4, r1
	and r3, r1
_08065B58:
	ldr r1, [sp, #0x18]
	add r2, r1, r5
	mov r7, sl
	and r2, r7
	lsl r2, r2, #5
	mov r1, ip
	add r0, r1, r2
	lsl r0, r0, #1
	add r0, r8
	ldr r7, _08065BE0 @ =0x00002258
	add r1, r7, #0
	strh r1, [r0]
	ldr r1, [sp, #0x28]
	add r0, r1, r2
	lsl r0, r0, #1
	add r0, r8
	mov r7, r9
	strh r7, [r0]
	add r0, r4, r2
	lsl r0, r0, #1
	add r0, r8
	strh r7, [r0]
	add r2, r3, r2
	lsl r2, r2, #1
	add r2, r8
	strh r7, [r2]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #1
	bls _08065B58
	mov r0, #0x1F
	ldr r1, [sp, #0x20]
	and r0, r1
	lsl r0, r0, #5
	add r0, r6, r0
	lsl r0, r0, #1
	add r0, r8
	ldr r2, _08065BE4 @ =0x00000199
	add r1, r2, #0
	strh r1, [r0]
	ldr r0, _08065BD4 @ =0x000007FF
	ldr r3, [sp, #0x1C]
	and r0, r3
	lsl r0, r0, #1
	ldr r4, _08065BE8 @ =0x08622AB4
	add r0, r0, r4
	ldrh r1, [r0]
	ldr r0, _08065BEC @ =0x00000777
	cmp r1, r0
	beq _08065C66
	cmp r1, r0
	bgt _08065BF0
	sub r0, #1
	cmp r1, r0
	beq _08065BFA
	b _08065E4E
	.align 2, 0
_08065BCC: .4byte 0x0201DB20
_08065BD0: .4byte 0x00001C1C
_08065BD4: .4byte 0x000007FF
_08065BD8: .4byte gUnk_08621DE0
_08065BDC: .4byte 0x00002230
_08065BE0: .4byte 0x00002258
_08065BE4: .4byte 0x00000199
_08065BE8: .4byte gUnk_08622AB4
_08065BEC: .4byte 0x00000777
_08065BF0:
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	beq _08065CD2
	b _08065E4E
_08065BFA:
	add r4, sp, #0x14
	add r0, r4, #0
	mov r1, #0
	mov r2, #4
	bl memset
	mov r0, #4
	strb r0, [r4]
	mov r5, #0
	mov r7, #0xC0
	lsl r7, r7, #2
	mov r9, r7
	mov r7, #1
	ldr r1, [sp, #0x20]
	lsl r0, r1, #5
	add r0, #1
	mov sl, r0
_08065C1C:
	mov r4, sp
	add r4, r4, r5
	add r4, #0x14
	mov r0, r9
	ldrb r2, [r4]
	orr r0, r2
	ldr r1, [sp, #0x24]
	add r1, #1
	add r1, r6, r1
	lsl r1, r1, #1
	add r1, r8
	str r7, [sp, #0]
	mov r2, #2
	mov r3, #0
	bl sub_080792A0
	mov r0, r9
	ldrb r4, [r4]
	orr r0, r4
	add r1, r6, #0
	add r2, r1, #1
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	add r1, sl
	lsl r1, r1, #1
	add r1, r8
	str r7, [sp, #0]
	mov r2, #2
	mov r3, #0
	bl sub_080792A0
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #3
	bls _08065C1C
	b _08065E4E
_08065C66:
	add r4, sp, #0x14
	add r0, r4, #0
	mov r1, #0
	mov r2, #4
	bl memset
	mov r0, #0xA
	strb r0, [r4]
	mov r5, #0
	mov r3, #0xC0
	lsl r3, r3, #2
	mov r9, r3
	mov r7, #1
	ldr r4, [sp, #0x20]
	lsl r0, r4, #5
	add r0, #1
	mov sl, r0
_08065C88:
	mov r4, sp
	add r4, r4, r5
	add r4, #0x14
	mov r0, r9
	ldrb r1, [r4]
	orr r0, r1
	ldr r1, [sp, #0x24]
	add r1, #1
	add r1, r6, r1
	lsl r1, r1, #1
	add r1, r8
	str r7, [sp, #0]
	mov r2, #2
	mov r3, #0
	bl sub_080792A0
	mov r0, r9
	ldrb r4, [r4]
	orr r0, r4
	add r1, r6, #0
	add r2, r1, #1
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	add r1, sl
	lsl r1, r1, #1
	add r1, r8
	str r7, [sp, #0]
	mov r2, #2
	mov r3, #0
	bl sub_080792A0
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #3
	bls _08065C88
	b _08065E4E
_08065CD2:
	ldr r1, _08065D38 @ =0x08087568
	add r0, sp, #0x14
	mov r2, #4
	bl memcpy
	mov r5, #0
	mov r2, #0xC0
	lsl r2, r2, #2
	mov r9, r2
	mov r7, #1
	ldr r3, [sp, #0x20]
	lsl r0, r3, #5
	add r0, #1
	mov sl, r0
_08065CEE:
	mov r4, sp
	add r4, r4, r5
	add r4, #0x14
	mov r0, r9
	ldrb r1, [r4]
	orr r0, r1
	ldr r1, [sp, #0x24]
	add r1, #1
	add r1, r6, r1
	lsl r1, r1, #1
	add r1, r8
	str r7, [sp, #0]
	mov r2, #2
	mov r3, #0
	bl sub_080792A0
	mov r0, r9
	ldrb r4, [r4]
	orr r0, r4
	add r1, r6, #0
	add r2, r1, #1
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	add r1, sl
	lsl r1, r1, #1
	add r1, r8
	str r7, [sp, #0]
	mov r2, #2
	mov r3, #0
	bl sub_080792A0
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #3
	bls _08065CEE
	b _08065E4E
_08065D38: .4byte gUnk_08087568
_08065D3C:
	ldr r2, [sp, #0x18]
	lsl r0, r2, #5
	add r0, r6, r0
	lsl r0, r0, #1
	add r0, r8
	mov r3, #0xCC
	lsl r3, r3, #1
	add r1, r3, #0
	strh r1, [r0]
	add r2, #1
	mov r0, #0x1F
	and r0, r2
	lsl r0, r0, #5
	add r0, r6, r0
	lsl r0, r0, #1
	add r0, r8
	ldr r4, _08065D88 @ =0x00000199
	add r1, r4, #0
	strh r1, [r0]
	ldr r0, _08065D8C @ =0x000007FF
	ldr r7, [sp, #0x1C]
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08065D90 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	str r2, [sp, #0x20]
	cmp r0, #0x15
	blt _08065D9E
	cmp r0, #0x17
	ble _08065D94
	cmp r0, #0x18
	beq _08065D98
	b _08065D9E
_08065D88: .4byte 0x00000199
_08065D8C: .4byte 0x000007FF
_08065D90: .4byte gUnk_08621DE0
_08065D94:
	mov r0, #0
	b _08065DB6
_08065D98:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08065DB6
_08065D9E:
	ldr r0, _08065E00 @ =0x000007FF
	ldr r2, [sp, #0x1C]
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _08065E04 @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08065DB6:
	add r4, r6, #4
	mov r1, #0x1F
	and r1, r4
	str r1, [sp, #0]
	ldr r7, [sp, #0x18]
	str r7, [sp, #4]
	mov r1, #2
	str r1, [sp, #8]
	mov r1, #0xC0
	lsl r1, r1, #2
	str r1, [sp, #0xC]
	mov r1, #0
	str r1, [sp, #0x10]
	mov r1, #4
	mov r2, #1
	mov r3, r8
	bl sub_080794E0
	ldr r0, _08065E00 @ =0x000007FF
	ldr r1, [sp, #0x1C]
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08065E04 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08065E12
	cmp r0, #0x17
	ble _08065E08
	cmp r0, #0x18
	beq _08065E0C
	b _08065E12
	.align 2, 0
_08065E00: .4byte 0x000007FF
_08065E04: .4byte gUnk_08621DE0
_08065E08:
	mov r0, #0
	b _08065E2A
_08065E0C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08065E2A
_08065E12:
	ldr r0, _08065E60 @ =0x000007FF
	ldr r3, [sp, #0x1C]
	and r0, r3
	lsl r0, r0, #2
	ldr r7, _08065E64 @ =0x08621DE0
	add r0, r0, r7
	ldr r1, [r0]
	ldr r0, _08065E68 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08065E2A:
	mov r1, #0x1F
	and r4, r1
	str r4, [sp, #0]
	ldr r2, [sp, #0x20]
	and r2, r1
	str r2, [sp, #4]
	mov r1, #2
	str r1, [sp, #8]
	mov r1, #0xC0
	lsl r1, r1, #2
	str r1, [sp, #0xC]
	mov r1, #0
	str r1, [sp, #0x10]
	mov r1, #4
	mov r2, #1
	mov r3, r8
	bl sub_080794E0
_08065E4E:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08065E60: .4byte 0x000007FF
_08065E64: .4byte gUnk_08621DE0
_08065E68: .4byte 0x000001FF
	thumb_func_end sub_08065AB4

