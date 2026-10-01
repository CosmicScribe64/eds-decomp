	thumb_func_start sub_0802DDFC
sub_0802DDFC: @ 0x0802DDFC
	push {r4, r5, lr}
	add r3, r0, #0
	ldrb r4, [r3, #6]
	ldrh r0, [r3, #6]
	lsr r5, r0, #8
	mov r0, #0xFC
	ldrb r1, [r3, #3]
	and r0, r1
	cmp r0, #0x14
	beq _0802DE14
	cmp r0, #0x18
	bne _0802DEBA
_0802DE14:
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _0802DE64 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802DE68 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802DEBA
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802DEBA
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r4, r0
	beq _0802DEBA
	ldr r0, _0802DE6C @ =0x000007FF
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0802DE70 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802DE74 @ =0x000002A9
	cmp r1, r0
	beq _0802DEA2
	cmp r1, r0
	bgt _0802DE78
	sub r0, #1
	cmp r1, r0
	beq _0802DE98
	b _0802DEBA
_0802DE64: .4byte 0x00000D64
_0802DE68: .4byte 0x0201930C
_0802DE6C: .4byte 0x000007FF
_0802DE70: .4byte gUnk_08622AB4
_0802DE74: .4byte 0x000002A9
_0802DE78:
	ldr r0, _0802DE90 @ =0x000003EA
	cmp r1, r0
	bne _0802DEBA
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800C894
	mov r2, #0
	ldr r1, _0802DE94 @ =0x000003E7
	cmp r0, r1
	ble _0802DEB6
	b _0802DEB4
_0802DE90: .4byte 0x000003EA
_0802DE94: .4byte 0x000003E7
_0802DE98:
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800C8A8
	b _0802DEAA
_0802DEA2:
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800C894
_0802DEAA:
	mov r2, #0
	mov r1, #0xFA
	lsl r1, r1, #1
	cmp r0, r1
	bgt _0802DEB6
_0802DEB4:
	mov r2, #1
_0802DEB6:
	add r0, r2, #0
	b _0802DEBC
_0802DEBA:
	mov r0, #0
_0802DEBC:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802DDFC
	.align 2, 0

