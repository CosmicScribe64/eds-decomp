	thumb_func_start sub_08070A1C
sub_08070A1C: @ 0x08070A1C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, _08070C5C @ =0x0201DB20
	ldr r1, _08070C60 @ =0x00001C5C
	add r0, r5, #0
	bl sub_08075278
	ldr r0, _08070C64 @ =0x03000040
	ldr r1, _08070C68 @ =0x0000040E
	add r0, r0, r1
	mov r4, #0
	mov r1, #1
	strh r1, [r0]
	ldr r0, _08070C6C @ =0x04000012
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #0xC
	strh r4, [r0]
	add r0, #2
	strh r4, [r0]
	add r0, #0x12
	strh r4, [r0]
	add r0, #2
	strh r4, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08070C70 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl sub_0807A2EC
	ldr r2, _08070C74 @ =0x00000632
	add r0, r5, r2
	strh r4, [r0]
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r5, r3
	strh r4, [r0]
	ldr r1, _08070C78 @ =0x0000063A
	add r0, r5, r1
	strh r4, [r0]
	add r2, #6
	add r0, r5, r2
	strh r4, [r0]
	add r3, #0xE
	add r0, r5, r3
	strh r4, [r0]
	add r1, #2
	add r0, r5, r1
	strh r4, [r0]
	mov r6, #0
	sub r2, #0x18
	add r2, r2, r5
	mov r9, r2
	mov r3, #0
	mov r8, r3
	mov r0, #0xA5
	lsl r0, r0, #5
	add r0, r0, r5
	mov ip, r0
	ldr r1, _08070C7C @ =0x00001494
	add r7, r5, r1
_08070ABC:
	lsl r0, r6, #1
	mov r2, r9
	add r1, r0, r2
	strh r4, [r1]
	mov r3, ip
	add r1, r6, r3
	mov r2, r8
	strb r2, [r1]
	mov r2, #0
	add r1, r6, #1
	add r3, r0, #0
_08070AD2:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r3, r0
	add r0, r0, r7
	strh r4, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, #1
	bls _08070AD2
	lsl r0, r1, #0x10
	lsr r6, r0, #0x10
	cmp r6, #2
	bls _08070ABC
	ldr r3, _08070C80 @ =0x00001C1D
	add r0, r5, r3
	mov r1, #0
	strb r1, [r0]
	ldr r2, _08070C84 @ =0x00001C1C
	add r0, r5, r2
	strb r1, [r0]
	ldr r3, _08070C88 @ =0x00000634
	add r0, r5, r3
	strb r1, [r0]
	ldr r2, _08070C8C @ =0x00000635
	add r0, r5, r2
	strb r1, [r0]
	ldr r3, _08070C90 @ =0x00001710
	add r1, r5, r3
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _08070C94 @ =0x000018AC
	add r1, r5, r3
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r0, #0xC5
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl sub_0807B0EC
	mov r1, #0xC8
	lsl r1, r1, #3
	add r0, r5, r1
	bl sub_080788A0
	ldr r2, _08070C98 @ =0x000018B0
	add r0, r5, r2
	bl sub_0807B534
	ldr r0, _08070C64 @ =0x03000040
	ldr r3, _08070C9C @ =0x00004874
	add r0, r0, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08070B56
	b _08070CCC
_08070B56:
	cmp r0, #1
	beq _08070B5C
	b _08070DB8
_08070B5C:
	mov r6, #1
	ldr r0, _08070CA0 @ =0x000007FF
	add r3, r0, #0
	ldr r2, _08070CA4 @ =0x08622AB4
	ldrh r1, [r2, #2]
	ldr r0, _08070CA8 @ =0x0000FFFF
	cmp r1, r0
	bne _08070B6E
	b _08070DB8
_08070B6E:
	ldr r1, _08070C7C @ =0x00001494
	add r1, r1, r5
	mov sl, r1
	ldr r0, _08070CAC @ =0x00001496
	add r0, r0, r5
	mov r9, r0
	ldr r1, _08070CB0 @ =0x00001498
	add r1, r1, r5
	mov r8, r1
_08070B80:
	add r0, r6, #0
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r3, _08070CB4 @ =0xFFFFFB46
	add r0, r2, r3
	lsl r0, r0, #0x10
	ldr r1, _08070CB8 @ =0x03150000
	cmp r0, r1
	bhi _08070BA2
	ldr r1, _08070CBC @ =0xFFFFF894
	add r0, r2, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x13
	bhi _08070C32
_08070BA2:
	ldr r0, _08070CC0 @ =0x02011C20
	lsl r1, r6, #2
	add r4, r1, r0
	ldrh r2, [r4, #8]
	lsl r0, r2, #0x16
	add r7, r1, #0
	cmp r0, #0
	beq _08070BD4
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r5, r3
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, sl
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #0
	bl sub_08068DA4
_08070BD4:
	ldrb r1, [r4, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08070BE4
	lsr r0, r1, #6
	cmp r0, #0
	beq _08070C04
_08070BE4:
	ldr r1, _08070CC4 @ =0x000014A1
	add r0, r5, r1
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r9
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #1
	bl sub_08068DA4
_08070C04:
	ldr r0, _08070CC0 @ =0x02011C20
	add r0, r7, r0
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08070C32
	ldr r2, _08070CC8 @ =0x000014A2
	add r0, r5, r2
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r8
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #2
	bl sub_08068DA4
_08070C32:
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r6, r0
	bls _08070C42
	b _08070DB8
_08070C42:
	ldr r0, _08070CA0 @ =0x000007FF
	add r3, r0, #0
	add r0, r6, #0
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _08070CA4 @ =0x08622AB4
	add r0, r0, r2
	ldr r1, _08070CA8 @ =0x0000FFFF
	ldrh r0, [r0]
	cmp r0, r1
	bne _08070B80
	b _08070DB8
	.align 2, 0
_08070C5C: .4byte 0x0201DB20
_08070C60: .4byte 0x00001C5C
_08070C64: .4byte 0x03000040
_08070C68: .4byte 0x0000040E
_08070C6C: .4byte 0x04000012
_08070C70: .4byte 0x0000E0FF
_08070C74: .4byte 0x00000632
_08070C78: .4byte 0x0000063A
_08070C7C: .4byte 0x00001494
_08070C80: .4byte 0x00001C1D
_08070C84: .4byte 0x00001C1C
_08070C88: .4byte 0x00000634
_08070C8C: .4byte 0x00000635
_08070C90: .4byte 0x00001710
_08070C94: .4byte 0x000018AC
_08070C98: .4byte 0x000018B0
_08070C9C: .4byte 0x00004874
_08070CA0: .4byte 0x000007FF
_08070CA4: .4byte gUnk_08622AB4
_08070CA8: .4byte 0x0000FFFF
_08070CAC: .4byte 0x00001496
_08070CB0: .4byte 0x00001498
_08070CB4: .4byte 0xFFFFFB46
_08070CB8: .4byte 0x03150000
_08070CBC: .4byte 0xFFFFF894
_08070CC0: .4byte 0x02011C20
_08070CC4: .4byte 0x000014A1
_08070CC8: .4byte 0x000014A2
_08070CCC:
	mov r6, #1
	ldr r1, _08070E28 @ =0x000007FF
	add r3, r1, #0
	ldr r2, _08070E2C @ =0x08622AB4
	ldrh r1, [r2, #2]
	ldr r0, _08070E30 @ =0x0000FFFF
	cmp r1, r0
	beq _08070DB8
	ldr r0, _08070E34 @ =0x00001494
	add r0, r0, r5
	mov sl, r0
	ldr r1, _08070E38 @ =0x00001496
	add r1, r1, r5
	mov r9, r1
	ldr r0, _08070E3C @ =0x00001498
	add r0, r0, r5
	mov r8, r0
_08070CEE:
	add r0, r6, #0
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r1, _08070E40 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08070D94
	ldr r0, _08070E44 @ =0x02011C20
	lsl r1, r6, #2
	add r4, r1, r0
	ldrh r2, [r4, #8]
	lsl r0, r2, #0x16
	add r7, r1, #0
	cmp r0, #0
	beq _08070D36
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r5, r3
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, sl
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #0
	bl sub_08068DA4
_08070D36:
	ldrb r1, [r4, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08070D46
	lsr r0, r1, #6
	cmp r0, #0
	beq _08070D66
_08070D46:
	ldr r1, _08070E48 @ =0x000014A1
	add r0, r5, r1
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r9
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #1
	bl sub_08068DA4
_08070D66:
	ldr r0, _08070E44 @ =0x02011C20
	add r0, r7, r0
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _08070D94
	ldr r2, _08070E4C @ =0x000014A2
	add r0, r5, r2
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r8
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #2
	bl sub_08068DA4
_08070D94:
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r6, r0
	bhi _08070DB8
	ldr r0, _08070E28 @ =0x000007FF
	add r3, r0, #0
	add r0, r6, #0
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _08070E2C @ =0x08622AB4
	add r0, r0, r2
	ldr r1, _08070E30 @ =0x0000FFFF
	ldrh r0, [r0]
	cmp r0, r1
	bne _08070CEE
_08070DB8:
	bl sub_0806710C
	ldr r7, _08070E50 @ =0x0201DB20
	ldr r1, _08070E54 @ =0x00001C1C
	add r6, r7, r1
	ldrb r1, [r6]
	lsl r2, r1, #1
	mov r3, #0xA5
	lsl r3, r3, #5
	add r4, r7, r3
	add r1, r1, r4
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r1, _08070E34 @ =0x00001494
	add r5, r7, r1
	add r0, r0, r5
	ldrh r0, [r0]
	mov r3, #0xC4
	lsl r3, r3, #3
	add r1, r7, r3
	add r2, r2, r1
	ldrh r1, [r2]
	ldr r3, _08070E58 @ =0x00001BB0
	add r2, r7, r3
	bl sub_08065F34
	ldr r0, _08070E5C @ =0x00001BB4
	add r2, r7, r0
	mov r0, #1
	ldrb r1, [r2]
	orr r1, r0
	strb r1, [r2]
	ldr r2, _08070E60 @ =0x00001BB6
	add r1, r7, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldrb r1, [r6]
	add r4, r1, r4
	ldrb r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r2
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, #5
	bls _08070E68
	ldr r3, _08070E64 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #1
	b _08070E6E
	.align 2, 0
_08070E28: .4byte 0x000007FF
_08070E2C: .4byte gUnk_08622AB4
_08070E30: .4byte 0x0000FFFF
_08070E34: .4byte 0x00001494
_08070E38: .4byte 0x00001496
_08070E3C: .4byte 0x00001498
_08070E40: .4byte 0xFFFFF880
_08070E44: .4byte 0x02011C20
_08070E48: .4byte 0x000014A1
_08070E4C: .4byte 0x000014A2
_08070E50: .4byte 0x0201DB20
_08070E54: .4byte 0x00001C1C
_08070E58: .4byte 0x00001BB0
_08070E5C: .4byte 0x00001BB4
_08070E60: .4byte 0x00001BB6
_08070E64: .4byte 0x00001BB5
_08070E68:
	ldr r3, _08070EF4 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #0
_08070E6E:
	strb r1, [r0]
	ldr r2, _08070EF8 @ =0x00001BB7
	add r0, r7, r2
	strb r1, [r0]
	ldr r4, _08070EFC @ =0x0201F6D8
	add r0, r4, #0
	bl sub_08066244
	add r0, r4, #0
	add r0, #0x68
	bl sub_080666AC
	add r2, r4, #0
	add r2, #0x7C
	mov r0, #2
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	add r3, r4, #0
	add r3, #0xA2
	mov r0, #4
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	mov r1, #2
	orr r0, r1
	strb r0, [r3]
	add r2, #8
	ldr r0, [r2]
	ldr r1, _08070F00 @ =0xFFFC7FFF
	and r0, r1
	str r0, [r2]
	add r2, #1
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x1D
	neg r0, r0
	ldrb r2, [r3]
	and r0, r2
	strb r0, [r3]
	ldr r0, _08070F04 @ =0x081A70FC
	ldr r3, _08070F08 @ =0xFFFFFB60
	add r1, r4, r3
	bl sub_08078670
	ldr r0, _08070F0C @ =0x03000040
	ldr r1, _08070F10 @ =0x00004872
	add r0, r0, r1
	mov r1, #0
	strh r1, [r0]
	mov r0, #1
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08070EF4: .4byte 0x00001BB5
_08070EF8: .4byte 0x00001BB7
_08070EFC: .4byte 0x0201F6D8
_08070F00: .4byte 0xFFFC7FFF
_08070F04: .4byte gUnk_081A70FC
_08070F08: .4byte 0xFFFFFB60
_08070F0C: .4byte 0x03000040
_08070F10: .4byte 0x00004872
	thumb_func_end sub_08070A1C

