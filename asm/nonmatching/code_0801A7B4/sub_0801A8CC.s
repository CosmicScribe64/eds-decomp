	thumb_func_start sub_0801A8CC
sub_0801A8CC: @ 0x0801A8CC
	push {r4, lr}
	ldr r1, _0801A8E8 @ =0x03000040
	ldr r2, _0801A8EC @ =0x00004859
	add r0, r1, r2
	ldrb r0, [r0]
	add r2, r1, #0
	cmp r0, #0xB
	bls _0801A8DE
	b _0801AB80
_0801A8DE:
	lsl r0, r0, #2
	ldr r1, _0801A8F0 @ =0x0801A8F4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0801A8E8: .4byte 0x03000040
_0801A8EC: .4byte 0x00004859
_0801A8F0: .4byte 0x0801A8F4
_0801A8F4:
	.4byte _0801A924
	.4byte _0801A92A
	.4byte _0801A958
	.4byte _0801A974
	.4byte _0801A9BC
	.4byte _0801AA00
	.4byte _0801AA44
	.4byte _0801AA84
	.4byte _0801AAC0
	.4byte _0801AAF4
	.4byte _0801AB34
	.4byte _0801AB5C
_0801A924:
	bl sub_08072510
	b _0801AB6A
_0801A92A:
	bl sub_0801F744
	cmp r0, #0
	beq _0801A9AC
	ldr r1, _0801A94C @ =0x02015EE8
	mov r0, #1
	ldrb r2, [r1, #1]
	orr r0, r2
	strb r0, [r1, #1]
	ldr r0, _0801A950 @ =0x03000040
	ldr r1, _0801A954 @ =0x00004859
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801A9AC
	.align 2, 0
_0801A94C: .4byte 0x02015EE8
_0801A950: .4byte 0x03000040
_0801A954: .4byte 0x00004859
_0801A958:
	bl sub_0800817C
	mov r0, #0
	mov r1, #4
	bl sub_08007E68
	ldr r0, _0801A96C @ =0x03000040
	ldr r2, _0801A970 @ =0x00004859
	add r0, r0, r2
	b _0801AB70
_0801A96C: .4byte 0x03000040
_0801A970: .4byte 0x00004859
_0801A974:
	bl sub_08074554
	mov r0, #0x80
	lsl r0, r0, #0x13
	mov r1, #0
	strh r1, [r0]
	add r0, #0x4C
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	ldr r4, _0801A9B0 @ =0x03000040
	ldr r0, _0801A9B4 @ =0x0000040E
	add r1, r4, r0
	mov r0, #2
	strh r0, [r1]
	bl sub_080759F4
	bl sub_08073574
	bl sub_080757AC
	ldr r1, _0801A9B8 @ =0x00004859
	add r4, r4, r1
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0801A9AC:
	mov r0, #0
	b _0801AB94
_0801A9B0: .4byte 0x03000040
_0801A9B4: .4byte 0x0000040E
_0801A9B8: .4byte 0x00004859
_0801A9BC:
	mov r2, #0x80
	lsl r2, r2, #1
	ldr r3, _0801A9EC @ =0x086893D8
	mov r0, #0
	mov r1, #0
	bl sub_0807326C
	ldr r1, _0801A9F0 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801A9E2
	ldr r0, _0801A9F4 @ =0x0000EE03
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
_0801A9E2:
	ldr r0, _0801A9F8 @ =0x03000040
	ldr r2, _0801A9FC @ =0x00004859
	add r0, r0, r2
	b _0801AB70
	.align 2, 0
_0801A9EC: .4byte gUnk_086893D8
_0801A9F0: .4byte 0x0201CFB0
_0801A9F4: .4byte 0x0000EE03
_0801A9F8: .4byte 0x03000040
_0801A9FC: .4byte 0x00004859
_0801AA00:
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0x80
	lsl r2, r2, #1
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #4
	bl sub_08075AE4
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801A9AC
	ldr r2, _0801AA38 @ =0x03000040
	ldr r0, _0801AA3C @ =0x00004870
	add r1, r2, r0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801AA2A
	b _0801AB92
_0801AA2A:
	ldr r0, _0801AA40 @ =0x00004859
	add r1, r2, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0801A9AC
	.align 2, 0
_0801AA38: .4byte 0x03000040
_0801AA3C: .4byte 0x00004870
_0801AA40: .4byte 0x00004859
_0801AA44:
	ldr r0, _0801AA74 @ =0x02017FB0
	mov r1, #0xC1
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	cmp r0, #0
	bge _0801A9AC
	ldr r0, _0801AA78 @ =0x0000F001
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	mov r0, #0
	bl sub_08022A9C
	ldr r0, _0801AA7C @ =0x03000040
	ldr r2, _0801AA80 @ =0x00004859
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801A9AC
_0801AA74: .4byte 0x02017FB0
_0801AA78: .4byte 0x0000F001
_0801AA7C: .4byte 0x03000040
_0801AA80: .4byte 0x00004859
_0801AA84:
	ldr r0, _0801AAAC @ =0x02017FB0
	ldr r1, _0801AAB0 @ =0x00000305
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	bge _0801A9AC
	ldr r0, _0801AAB4 @ =0x0000F012
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r0, _0801AAB8 @ =0x03000040
	ldr r2, _0801AABC @ =0x00004859
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801A9AC
_0801AAAC: .4byte 0x02017FB0
_0801AAB0: .4byte 0x00000305
_0801AAB4: .4byte 0x0000F012
_0801AAB8: .4byte 0x03000040
_0801AABC: .4byte 0x00004859
_0801AAC0:
	ldr r0, _0801AAE4 @ =0x02017FB0
	ldr r1, _0801AAE8 @ =0x00000305
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	cmp r0, #0
	blt _0801AAD0
	b _0801A9AC
_0801AAD0:
	mov r0, #0
	bl sub_08022BFC
	ldr r0, _0801AAEC @ =0x03000040
	ldr r2, _0801AAF0 @ =0x00004859
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801A9AC
_0801AAE4: .4byte 0x02017FB0
_0801AAE8: .4byte 0x00000305
_0801AAEC: .4byte 0x03000040
_0801AAF0: .4byte 0x00004859
_0801AAF4:
	ldr r0, _0801AB20 @ =0x02017FB0
	ldr r1, _0801AB24 @ =0x00000305
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	cmp r0, #0
	blt _0801AB04
	b _0801A9AC
_0801AB04:
	ldr r0, _0801AB28 @ =0x0000F014
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r0, _0801AB2C @ =0x03000040
	ldr r2, _0801AB30 @ =0x00004859
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801A9AC
	.align 2, 0
_0801AB20: .4byte 0x02017FB0
_0801AB24: .4byte 0x00000305
_0801AB28: .4byte 0x0000F014
_0801AB2C: .4byte 0x03000040
_0801AB30: .4byte 0x00004859
_0801AB34:
	ldr r0, _0801AB50 @ =0x02017FB0
	ldr r1, _0801AB54 @ =0x00000306
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	cmp r0, #0
	bne _0801AB44
	b _0801A9AC
_0801AB44:
	ldr r0, _0801AB58 @ =0x00004859
	add r1, r2, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0801A9AC
_0801AB50: .4byte 0x02017FB0
_0801AB54: .4byte 0x00000306
_0801AB58: .4byte 0x00004859
_0801AB5C:
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801AB6A
	b _0801A9AC
_0801AB6A:
	ldr r0, _0801AB78 @ =0x03000040
	ldr r1, _0801AB7C @ =0x00004859
	add r0, r0, r1
_0801AB70:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0801A9AC
_0801AB78: .4byte 0x03000040
_0801AB7C: .4byte 0x00004859
_0801AB80:
	ldr r1, _0801AB9C @ =0x02017FB0
	mov r2, #0xC1
	lsl r2, r2, #2
	add r1, r1, r2
	mov r0, #5
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801AB92:
	mov r0, #1
_0801AB94:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801AB9C: .4byte 0x02017FB0
	thumb_func_end sub_0801A8CC

