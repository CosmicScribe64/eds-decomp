	thumb_func_start sub_0801FEA0
sub_0801FEA0: @ 0x0801FEA0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _0801FED4 @ =0xFFFFFE00
	add sp, r4
	add r7, r0, #0
	add r6, r1, #0
	ldr r5, _0801FED8 @ =0x02017A40
	mov r0, #0x92
	lsl r0, r0, #3
	add r4, r5, r0
	ldrb r3, [r4]
	add r0, r3, #0
	mov r8, r5
	cmp r0, #0xA
	bne _0801FEC2
	b _08020004
_0801FEC2:
	cmp r0, #0xA
	bgt _0801FEE2
	cmp r0, #1
	beq _0801FF2A
	cmp r0, #1
	bgt _0801FEDC
	cmp r0, #0
	beq _0801FF02
	b _0802031A
_0801FED4: .4byte 0xFFFFFE00
_0801FED8: .4byte 0x02017A40
_0801FEDC:
	cmp r0, #2
	beq _0801FFD4
	b _0802031A
_0801FEE2:
	cmp r0, #0x64
	bne _0801FEE8
	b _080202AC
_0801FEE8:
	cmp r0, #0x64
	bgt _0801FEF4
	cmp r0, #0xB
	bne _0801FEF2
	b _08020160
_0801FEF2:
	b _0802031A
_0801FEF4:
	cmp r0, #0x65
	bne _0801FEFA
	b _080202EC
_0801FEFA:
	cmp r0, #0xC8
	bne _0801FF00
	b _08020304
_0801FF00:
	b _0802031A
_0801FF02:
	mov r2, #0x91
	lsl r2, r2, #3
	add r1, r5, r2
	ldrb r2, [r1]
	mov r0, #1
	and r0, r2
	cmp r0, #0
	bne _0801FF26
	mov r0, #1
	orr r0, r2
	strb r0, [r1]
	mov r0, #0x12
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	b _080202D2
_0801FF26:
	add r0, r3, #1
	strb r0, [r4]
_0801FF2A:
	cmp r6, #0
	beq _0801FF4C
	ldr r1, _0801FF48 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	mov r1, #0xC8
	cmp r0, #0
	beq _0801FF3E
	mov r1, #0x64
_0801FF3E:
	mov r0, #0x92
	lsl r0, r0, #3
	add r0, r8
	b _0801FFB8
	.align 2, 0
_0801FF48: .4byte 0x02015EE8
_0801FF4C:
	ldrh r2, [r7]
	ldr r0, _0801FF78 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _0801FF7C @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0801FF88
	ldr r1, _0801FF80 @ =0x08081CB8
	lsl r2, r2, #6
	ldr r6, _0801FF84 @ =0x0822C720
	add r2, r2, r6
	mov r0, sp
	bl sub_080753F4
	b _0801FF96
	.align 2, 0
_0801FF78: .4byte 0x000007FF
_0801FF7C: .4byte gUnk_08621DE0
_0801FF80: .4byte gUnk_08081CB8
_0801FF84: .4byte gUnk_0822C720
_0801FF88:
	ldr r1, _0801FFC0 @ =0x08081CFC
	lsl r2, r2, #6
	ldr r0, _0801FFC4 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
_0801FF96:
	ldr r0, _0801FFC8 @ =0x00000206
	ldr r1, _0801FFCC @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r0, _0801FFD0 @ =0x02017A40
	mov r1, #0x92
	lsl r1, r1, #3
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
_0801FFB8:
	strb r1, [r0]
_0801FFBA:
	mov r0, #0
	b _0802031C
	.align 2, 0
_0801FFC0: .4byte gUnk_08081CFC
_0801FFC4: .4byte gUnk_0822C720
_0801FFC8: .4byte 0x00000206
_0801FFCC: .4byte 0x00000712
_0801FFD0: .4byte 0x02017A40
_0801FFD4:
	ldr r0, _0801FFF8 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0801FFDE
	b _0802031A
_0801FFDE:
	mov r0, #0xA
	strb r0, [r4]
	ldr r2, _0801FFFC @ =0x020192E0
	ldr r3, _08020000 @ =0x00001B2C
	add r2, r2, r3
	sub r0, #0xC
	ldrb r6, [r2]
	and r0, r6
	mov r1, #3
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	b _0801FFBA
_0801FFF8: .4byte 0x0201AE60
_0801FFFC: .4byte 0x020192E0
_08020000: .4byte 0x00001B2C
_08020004:
	ldr r6, _0802001C @ =0x020192E0
	ldr r1, _08020020 @ =0x00001B2C
	add r0, r6, r1
	ldrb r1, [r0]
	mov r5, #1
	add r0, r5, #0
	and r0, r1
	cmp r0, #0
	beq _08020024
	bl sub_0801DC04
	b _0801FFBA
_0802001C: .4byte 0x020192E0
_08020020: .4byte 0x00001B2C
_08020024:
	mov r2, #2
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _08020032
	add r0, r3, #1
	b _080202D6
_08020032:
	ldr r1, _08020044 @ =0x03000040
	add r0, r2, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08020048
	strb r5, [r4]
	b _0801FFBA
	.align 2, 0
_08020044: .4byte 0x03000040
_08020048:
	ldr r3, _08020090 @ =0x00001B12
	add r1, r6, r3
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	mov r1, #0xEE
	cmp r0, #0
	bne _0802005A
	mov r1, #0xF
_0802005A:
	add r0, r1, #0
	bl sub_08052F38
	cmp r0, #0
	beq _0801FFBA
	ldr r0, _08020094 @ =0x0201CFB0
	ldr r6, _08020098 @ =0x00000824
	add r1, r0, r6
	ldr r6, [r1]
	ldr r2, _0802009C @ =0x00000828
	add r1, r0, r2
	ldr r5, [r1]
	ldr r3, _080200A0 @ =0x0000082C
	add r0, r0, r3
	ldr r0, [r0]
	mov r8, r0
	bl sub_0805ECFC
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r5, #0xF
	bhi _0801FFBA
	lsl r0, r5, #2
	ldr r1, _080200A4 @ =0x080200A8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08020090: .4byte 0x00001B12
_08020094: .4byte 0x0201CFB0
_08020098: .4byte 0x00000824
_0802009C: .4byte 0x00000828
_080200A0: .4byte 0x0000082C
_080200A4: .4byte 0x080200A8
_080200A8:
	.4byte _080200E8
	.4byte _0801FFBA
	.4byte _0801FFBA
	.4byte _0801FFBA
	.4byte _0801FFBA
	.4byte _080200E8
	.4byte _0801FFBA
	.4byte _0801FFBA
	.4byte _0801FFBA
	.4byte _0801FFBA
	.4byte _080200E8
	.4byte _080200E8
	.4byte _0802014C
	.4byte _08020148
	.4byte _0802014C
	.4byte _0802014C
_080200E8:
	cmp r2, #0
	beq _08020144
	ldr r1, _08020130 @ =0x020192E0
	ldr r0, _08020134 @ =0x00001B2C
	add r4, r1, r0
	mov r0, #1
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	ldr r3, _08020138 @ =0x00001B2F
	add r2, r1, r3
	mov r0, #3
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	ldr r0, _0802013C @ =0x00001B30
	add r1, r1, r0
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, r8
	bl sub_0801FD68
	lsl r0, r0, #0x10
	lsr r0, r0, #6
	ldr r1, [r4]
	ldr r2, _08020140 @ =0xFC0003FF
	and r1, r2
	orr r1, r0
	str r1, [r4]
	b _0801FFBA
_08020130: .4byte 0x020192E0
_08020134: .4byte 0x00001B2C
_08020138: .4byte 0x00001B2F
_0802013C: .4byte 0x00001B30
_08020140: .4byte 0xFC0003FF
_08020144:
	mov r0, #3
	b _0802015A
_08020148:
	mov r0, #3
	b _0802015A
_0802014C:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802AF34
	mov r0, #1
_0802015A:
	bl sub_08077AEC
	b _0801FFBA
_08020160:
	ldr r4, _08020188 @ =0x020192E0
	ldr r3, _0802018C @ =0x00001B33
	add r0, r4, r3
	ldrb r0, [r0]
	lsr r2, r0, #2
	ldr r6, _08020190 @ =0x00001B34
	add r1, r4, r6
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #6
	orr r0, r2
	cmp r0, #5
	beq _080201B8
	cmp r0, #5
	bgt _08020194
	cmp r0, #0
	beq _080201B8
	b _08020272
	.align 2, 0
_08020188: .4byte 0x020192E0
_0802018C: .4byte 0x00001B33
_08020190: .4byte 0x00001B34
_08020194:
	cmp r0, #0xB
	bne _08020272
	mov r0, #1
	mov r1, #1
	add r2, r7, #0
	bl sub_08049048
	ldr r0, _080201B4 @ =0x00001B2C
	add r1, r4, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08020272
	b _0801FFBA
	.align 2, 0
_080201B4: .4byte 0x00001B2C
_080201B8:
	ldr r4, _08020288 @ =0x020192E0
	ldr r2, _0802028C @ =0x00001B2C
	add r1, r4, r2
	mov r0, #3
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	ldr r6, _08020290 @ =0x00001B33
	add r0, r4, r6
	ldrb r6, [r0]
	lsl r5, r6, #0x1E
	mov r0, #1
	lsr r5, r5, #0x1F
	ldr r1, _08020294 @ =0x00001B34
	add r2, r4, r1
	ldrh r3, [r2]
	lsl r3, r3, #0x17
	mov ip, r3
	lsr r1, r3, #0x18
	lsr r3, r6, #2
	ldrb r2, [r2]
	and r0, r2
	lsl r2, r0, #6
	orr r2, r3
	add r1, r1, r2
	mov r0, #0x94
	mul r1, r0
	ldr r0, _08020298 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	add r4, #0x2C
	add r1, r1, r4
	mov r3, #2
	add r0, r3, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08020222
	add r0, r3, #0
	and r0, r6
	mov r3, #0x7F
	cmp r0, #0
	beq _08020212
	ldr r3, _0802029C @ =0x0000807F
_08020212:
	mov r6, ip
	lsr r1, r6, #0x18
	add r1, r1, r2
	add r0, r3, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08020222:
	ldr r5, _08020288 @ =0x020192E0
	ldr r1, _08020290 @ =0x00001B33
	add r0, r5, r1
	ldrb r4, [r0]
	lsl r0, r4, #0x1E
	mov r2, #1
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldrb r3, [r7, #3]
	lsr r1, r3, #2
	lsl r1, r1, #0x19
	orr r0, r1
	ldr r6, _08020294 @ =0x00001B34
	add r3, r5, r6
	ldrh r6, [r3]
	lsl r1, r6, #0x17
	lsr r1, r1, #0x18
	lsr r4, r4, #2
	ldrb r3, [r3]
	and r2, r3
	lsl r2, r2, #6
	orr r2, r4
	add r1, r1, r2
	mov r2, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	orr r1, r2
	orr r0, r1
	ldr r1, _080202A0 @ =0x00001B28
	add r5, r5, r1
	ldrh r5, [r5]
	orr r0, r5
	ldrh r2, [r7, #8]
	lsl r1, r2, #0x10
	ldrh r7, [r7, #6]
	orr r1, r7
	bl sub_0801FBE0
_08020272:
	ldr r0, _080202A4 @ =0x02017A40
	ldr r3, _080202A8 @ =0x00000491
	add r0, r0, r3
	mov r1, #0x41
	neg r1, r1
	ldrb r6, [r0]
	and r1, r6
	mov r2, #0x80
	orr r1, r2
	strb r1, [r0]
	b _0802031A
_08020288: .4byte 0x020192E0
_0802028C: .4byte 0x00001B2C
_08020290: .4byte 0x00001B33
_08020294: .4byte 0x00001B34
_08020298: .4byte 0x00000D64
_0802029C: .4byte 0x0000807F
_080202A0: .4byte 0x00001B28
_080202A4: .4byte 0x02017A40
_080202A8: .4byte 0x00000491
_080202AC:
	ldr r0, _080202DC @ =0x0000F054
	add r1, r7, #0
	mov r2, #0x14
	bl sub_080229BC
	ldr r0, _080202E0 @ =0x00000491
	add r1, r5, r0
	mov r0, #0x7F
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r1, _080202E4 @ =0x02017FB0
	ldr r3, _080202E8 @ =0x00000307
	add r1, r1, r3
	mov r0, #9
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
_080202D2:
	ldrb r0, [r4]
	add r0, #1
_080202D6:
	strb r0, [r4]
	b _0801FFBA
	.align 2, 0
_080202DC: .4byte 0x0000F054
_080202E0: .4byte 0x00000491
_080202E4: .4byte 0x02017FB0
_080202E8: .4byte 0x00000307
_080202EC:
	ldr r0, _080202FC @ =0x02017FB0
	ldr r1, _08020300 @ =0x00000307
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1F
	b _0802031C
	.align 2, 0
_080202FC: .4byte 0x02017FB0
_08020300: .4byte 0x00000307
_08020304:
	add r0, r7, #0
	bl sub_080589C8
	cmp r0, #0
	beq _0802031A
	ldr r2, _0802032C @ =0x00000491
	add r1, r5, r2
	mov r0, #0x80
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_0802031A:
	mov r0, #1
_0802031C:
	mov r3, #0x80
	lsl r3, r3, #2
	add sp, r3
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802032C: .4byte 0x00000491
	thumb_func_end sub_0801FEA0

