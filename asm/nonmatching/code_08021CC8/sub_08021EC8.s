	thumb_func_start sub_08021EC8
sub_08021EC8: @ 0x08021EC8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r2, r0, #0
	ldr r6, _08021EF8 @ =0x020192E0
	ldr r0, _08021EFC @ =0x00001B62
	add r4, r6, r0
	ldrb r1, [r4]
	mov r8, r6
	cmp r1, #0
	beq _08021F04
	cmp r1, #1
	bne _08021EE4
	b _08022014
_08021EE4:
	ldr r1, _08021F00 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08021EF2
	b _0802204C
_08021EF2:
	mov r0, #0
	strb r0, [r4]
	b _080220CA
_08021EF8: .4byte 0x020192E0
_08021EFC: .4byte 0x00001B62
_08021F00: .4byte 0x03000040
_08021F04:
	ldr r3, _08021FA4 @ =0x00001B64
	add r0, r6, r3
	strh r1, [r0]
	mov r4, #0
	mov r0, #1
	and r0, r2
	ldr r3, _08021FA8 @ =0x00000D64
	mul r0, r3
	add r0, r6, r0
	ldrb r0, [r0, #6]
	cmp r4, r0
	bge _08021F9E
	mov r7, #1
	mov r0, #0xD1
	lsl r0, r0, #3
	add r0, r0, r6
	mov ip, r0
	ldr r1, _08021FAC @ =0x00001B62
	add r5, r6, r1
	ldr r0, _08021FB0 @ =0x00000D6A
	add r6, r6, r0
_08021F2E:
	add r0, r2, #0
	and r0, r7
	mul r0, r3
	add r0, ip
	lsl r1, r4, #2
	add r0, r0, r1
	ldr r1, [r0]
	lsl r0, r1, #0x15
	lsr r0, r0, #0x13
	ldr r3, _08021FB4 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r3, #0xF8
	lsl r3, r3, #0x11
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08021FF4
	lsl r0, r1, #0xD
	cmp r0, #0
	blt _08021FF4
	cmp r2, #0
	beq _08021FC8
	ldr r0, _08021FB8 @ =0x000014DC
	add r0, ip
	strh r7, [r0]
	mov r4, #0
	ldrb r6, [r6]
	cmp r4, r6
	bge _08021F96
	ldr r1, _08021FBC @ =0x0201A6CC
	ldr r5, _08021FC0 @ =0x000007FF
	ldr r6, _08021FC4 @ =0xFFFFF97E
	add r0, r1, r6
	ldrb r2, [r0]
_08021F74:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #2
	ldr r6, _08021FB4 @ =0x08621DE0
	add r0, r0, r6
	ldr r0, [r0]
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08021F8E
	b _080220B4
_08021F8E:
	add r1, #4
	add r4, #1
	cmp r4, r2
	blt _08021F74
_08021F96:
	ldr r1, _08021FA4 @ =0x00001B64
	add r1, r8
	mov r0, #0
	strh r0, [r1]
_08021F9E:
	mov r0, #1
	b _080220CA
	.align 2, 0
_08021FA4: .4byte 0x00001B64
_08021FA8: .4byte 0x00000D64
_08021FAC: .4byte 0x00001B62
_08021FB0: .4byte 0x00000D6A
_08021FB4: .4byte gUnk_08621DE0
_08021FB8: .4byte 0x000014DC
_08021FBC: .4byte 0x0201A6CC
_08021FC0: .4byte 0x000007FF
_08021FC4: .4byte 0xFFFFF97E
_08021FC8:
	ldr r0, _08021FE8 @ =0x00000206
	ldr r1, _08021FEC @ =0x00000712
	mov r2, #0xB
	ldr r3, _08021FF0 @ =0x08081DB8
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _080220C8
	.align 2, 0
_08021FE8: .4byte 0x00000206
_08021FEC: .4byte 0x00000712
_08021FF0: .4byte gUnk_08081DB8
_08021FF4:
	add r4, #1
	ldr r1, _0802200C @ =0x020192E4
	add r0, r2, #0
	and r0, r7
	ldr r3, _08022010 @ =0x00000D64
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r4, r0
	blt _08021F2E
	b _08021F9E
	.align 2, 0
_0802200C: .4byte 0x020192E4
_08022010: .4byte 0x00000D64
_08022014:
	ldr r0, _08022024 @ =0x0201AE60
	ldrh r2, [r0, #0x14]
	cmp r2, #0
	bne _0802202C
	ldr r1, _08022028 @ =0x00001B64
	add r0, r6, r1
	strh r2, [r0]
	b _08021F9E
_08022024: .4byte 0x0201AE60
_08022028: .4byte 0x00001B64
_0802202C:
	ldr r0, _08022040 @ =0x00000206
	ldr r1, _08022044 @ =0x00000712
	ldr r3, _08022048 @ =0x08081DF0
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _080220C8
_08022040: .4byte 0x00000206
_08022044: .4byte 0x00000712
_08022048: .4byte gUnk_08081DF0
_0802204C:
	mov r0, #1
	bl sub_08052F38
	cmp r0, #0
	beq _080220C8
	ldr r0, _080220A4 @ =0x0201CFB0
	ldr r2, _080220A8 @ =0x0000082C
	add r0, r0, r2
	ldr r3, [r0]
	lsl r0, r3, #2
	mov r2, #0xD1
	lsl r2, r2, #3
	add r1, r6, r2
	add r0, r0, r1
	ldr r2, [r0]
	lsl r0, r2, #0x15
	lsr r0, r0, #0x13
	ldr r1, _080220AC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _080220C2
	mov r1, #1
	lsl r0, r2, #0xE
	cmp r0, #0
	bge _0802208A
	mov r1, #0
_0802208A:
	lsl r0, r2, #0xD
	cmp r0, #0
	bge _08022092
	mov r1, #0
_08022092:
	cmp r1, #0
	beq _080220C2
	ldr r2, _080220B0 @ =0x00001B64
	add r1, r6, r2
	mov r0, #1
	strh r0, [r1]
	mov r0, #0
	add r1, r3, #0
	b _080220B8
_080220A4: .4byte 0x0201CFB0
_080220A8: .4byte 0x0000082C
_080220AC: .4byte gUnk_08621DE0
_080220B0: .4byte 0x00001B64
_080220B4:
	mov r0, #1
	add r1, r4, #0
_080220B8:
	mov r2, #1
	mov r3, #1
	bl sub_080193D4
	b _08021F9E
_080220C2:
	mov r0, #3
	bl sub_08077AEC
_080220C8:
	mov r0, #0
_080220CA:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08021EC8

