	thumb_func_start sub_08036A68
sub_08036A68: @ 0x08036A68
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	beq _08036A80
	b _08036D20
_08036A80:
	ldr r1, _08036AA0 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	add r5, r1, #0
	cmp r0, #4
	bls _08036A94
	b _08036D20
_08036A94:
	lsl r0, r0, #2
	ldr r1, _08036AA4 @ =0x08036AA8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08036AA0: .4byte 0x02017A40
_08036AA4: .4byte 0x08036AA8
_08036AA8:
	.4byte _08036CD4
	.4byte _08036CB0
	.4byte _08036C98
	.4byte _08036B18
	.4byte _08036ABC
_08036ABC:
	mov r5, #0
_08036ABE:
	mov r4, #0
	add r6, r5, #1
_08036AC2:
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08030028
	ldrb r3, [r7, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl sub_08046CB0
	add r4, #1
	cmp r4, #4
	ble _08036AC2
	add r5, r6, #0
	cmp r5, #1
	ble _08036ABE
	ldr r2, _08036B04 @ =0x02017A40
	ldr r0, _08036B08 @ =0x020192E0
	ldr r1, _08036B0C @ =0x00001B12
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r3, _08036B10 @ =0x000003E1
	add r1, r2, r3
	strb r0, [r1]
	ldr r0, _08036B14 @ =0x000003E2
	add r2, r2, r0
	mov r0, #5
	strb r0, [r2]
_08036B00:
	mov r0, #0x7F
	b _08036D22
_08036B04: .4byte 0x02017A40
_08036B08: .4byte 0x020192E0
_08036B0C: .4byte 0x00001B12
_08036B10: .4byte 0x000003E1
_08036B14: .4byte 0x000003E2
_08036B18:
	ldr r7, _08036BB4 @ =0x020192E4
	ldr r1, _08036BB8 @ =0x000003E1
	add r6, r5, r1
	ldrb r2, [r6]
	mov r3, #1
	mov sl, r3
	add r0, r2, #0
	mov r1, sl
	and r0, r1
	ldr r3, _08036BBC @ =0x00000D64
	mov r9, r3
	mov r1, r9
	mul r1, r0
	add r0, r1, r7
	ldrb r0, [r0, #3]
	cmp r0, #0
	bne _08036B3C
	b _08036D0A
_08036B3C:
	ldr r3, _08036BC0 @ =0x000007C4
	add r0, r7, r3
	add r4, r1, r0
	mov r8, r4
	mov r0, #0x61
	cmp r2, #0
	beq _08036B4C
	ldr r0, _08036BC4 @ =0x00008061
_08036B4C:
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrb r0, [r6]
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019840
	ldr r1, [r4]
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	ldrb r2, [r6]
	cmp r0, r2
	beq _08036BF4
	lsl r0, r1, #0xE
	cmp r0, #0
	bge _08036BF4
	lsl r0, r1, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08036BC8 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08036BCC @ =0x000002FA
	ldrh r0, [r0]
	cmp r0, r1
	bne _08036BF4
	mov r0, #1
	sub r0, r0, r2
	bl sub_08008A1C
	cmp r0, #0
	ble _08036BD8
	ldrb r0, [r6]
	mov r3, #0xC2
	cmp r0, #0
	beq _08036B9A
	ldr r3, _08036BD0 @ =0x000080C2
_08036B9A:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r2, _08036BD4 @ =0x00000544
	add r0, r5, r2
	add r1, r4, #0
	bl sub_08007558
	mov r0, #0x7D
	b _08036D22
_08036BB4: .4byte 0x020192E4
_08036BB8: .4byte 0x000003E1
_08036BBC: .4byte 0x00000D64
_08036BC0: .4byte 0x000007C4
_08036BC4: .4byte 0x00008061
_08036BC8: .4byte gUnk_08622AB4
_08036BCC: .4byte 0x000002FA
_08036BD0: .4byte 0x000080C2
_08036BD4: .4byte 0x00000544
_08036BD8:
	ldrb r0, [r6]
	add r1, r0, #0
	mov r3, sl
	and r1, r3
	mov r2, r9
	mul r2, r1
	add r1, r2, #0
	add r1, r1, r7
	ldrb r1, [r1, #2]
	mov r2, #0
	mov r3, #1
	bl sub_080193D4
	b _08036CC6
_08036BF4:
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _08036C20 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _08036C24 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08036CC6
	cmp r0, #0x15
	blt _08036C30
	cmp r0, #0x17
	ble _08036C28
	cmp r0, #0x18
	beq _08036C2C
	b _08036C30
_08036C20: .4byte 0x000007FF
_08036C24: .4byte gUnk_08621DE0
_08036C28:
	mov r0, #0
	b _08036C44
_08036C2C:
	mov r0, #0xA
	b _08036C44
_08036C30:
	ldr r0, _08036C80 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08036C84 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08036C44:
	cmp r0, #4
	bhi _08036CC6
	add r0, r2, #0
	bl sub_08007834
	cmp r0, #0
	bne _08036CC6
	ldr r5, _08036C88 @ =0x02017A40
	ldr r2, _08036C8C @ =0x000003E1
	add r0, r5, r2
	ldrb r0, [r0]
	mov r3, #0xC2
	cmp r0, #0
	beq _08036C62
	ldr r3, _08036C90 @ =0x000080C2
_08036C62:
	mov r0, r8
	ldrh r1, [r0]
	ldrh r2, [r0, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r1, _08036C94 @ =0x00000544
	add r0, r5, r1
	add r1, r4, #0
	bl sub_08007558
	mov r0, #0x7E
	b _08036D22
	.align 2, 0
_08036C80: .4byte 0x000007FF
_08036C84: .4byte gUnk_08621DE0
_08036C88: .4byte 0x02017A40
_08036C8C: .4byte 0x000003E1
_08036C90: .4byte 0x000080C2
_08036C94: .4byte 0x00000544
_08036C98:
	ldr r2, _08036CA8 @ =0x000003E1
	add r0, r5, r2
	ldrb r0, [r0]
	ldr r3, _08036CAC @ =0x00000544
	add r1, r5, r3
	mov r2, #0
	b _08036CC0
	.align 2, 0
_08036CA8: .4byte 0x000003E1
_08036CAC: .4byte 0x00000544
_08036CB0:
	ldr r0, _08036CCC @ =0x000003E1
	add r1, r5, r0
	mov r0, #1
	ldrb r1, [r1]
	sub r0, r0, r1
	ldr r2, _08036CD0 @ =0x00000544
	add r1, r5, r2
	mov r2, #1
_08036CC0:
	mov r3, #0
	bl sub_08056094
_08036CC6:
	mov r0, #0x7C
	b _08036D22
	.align 2, 0
_08036CCC: .4byte 0x000003E1
_08036CD0: .4byte 0x00000544
_08036CD4:
	ldr r3, _08036D10 @ =0x000003E2
	add r2, r5, r3
	ldrb r0, [r2]
	sub r0, #1
	strb r0, [r2]
	lsl r0, r0, #0x18
	cmp r0, #0
	beq _08036CE6
	b _08036B00
_08036CE6:
	ldr r0, _08036D14 @ =0x000003E1
	add r1, r5, r0
	mov r0, #1
	ldrb r3, [r1]
	sub r0, r0, r3
	strb r0, [r1]
	mov r0, #5
	strb r0, [r2]
	ldr r0, _08036D18 @ =0x020192E0
	ldr r2, _08036D1C @ =0x00001B12
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldrb r1, [r1]
	cmp r1, r0
	beq _08036D0A
	b _08036B00
_08036D0A:
	mov r0, #0x78
	b _08036D22
	.align 2, 0
_08036D10: .4byte 0x000003E2
_08036D14: .4byte 0x000003E1
_08036D18: .4byte 0x020192E0
_08036D1C: .4byte 0x00001B12
_08036D20:
	mov r0, #0
_08036D22:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08036A68

