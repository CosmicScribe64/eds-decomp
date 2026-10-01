	thumb_func_start sub_08016A18
sub_08016A18: @ 0x08016A18
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r1, _08016A40 @ =0x020185C0
	ldrh r0, [r1]
	lsr r0, r0, #0xF
	mov r8, r0
	ldr r2, _08016A44 @ =0x0000080A
	add r6, r1, r2
	ldrb r3, [r6]
	lsl r0, r3, #0x19
	lsr r5, r0, #0x19
	add r7, r1, #0
	cmp r5, #1
	beq _08016AD0
	cmp r5, #1
	bgt _08016A48
	cmp r5, #0
	beq _08016A56
	b _08016D08
_08016A40: .4byte 0x020185C0
_08016A44: .4byte 0x0000080A
_08016A48:
	cmp r5, #2
	bne _08016A4E
	b _08016B90
_08016A4E:
	cmp r5, #3
	bne _08016A54
	b _08016C50
_08016A54:
	b _08016D08
_08016A56:
	ldr r4, _08016AAC @ =0x0201CFB0
	ldr r0, _08016AB0 @ =0x00000808
	add r1, r4, r0
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _08016AB4 @ =0x050003E0
	ldr r1, _08016AB8 @ =0x08688FBC
	mov r2, #0x20
	bl sub_080752B0
	ldr r0, _08016ABC @ =0x06016C80
	ldr r1, _08016AC0 @ =0x08688FD8
	mov r2, #0x80
	lsl r2, r2, #3
	bl sub_080752B0
	ldr r0, _08016AC4 @ =0x0000085C
	add r4, r4, r0
	str r5, [r4]
	ldr r2, _08016AC8 @ =0x0000080C
	add r1, r7, r2
	ldr r0, _08016ACC @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldrb r2, [r6]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	mov r0, #0x16
	bl sub_08077AEC
	b _08016D16
_08016AAC: .4byte 0x0201CFB0
_08016AB0: .4byte 0x00000808
_08016AB4: .4byte 0x050003E0
_08016AB8: .4byte gUnk_08688FBC
_08016ABC: .4byte 0x06016C80
_08016AC0: .4byte gUnk_08688FD8
_08016AC4: .4byte 0x0000085C
_08016AC8: .4byte 0x0000080C
_08016ACC: .4byte 0xFFFFF01F
_08016AD0:
	ldr r0, _08016AF0 @ =0x0000080C
	add r1, r7, r0
	ldrh r4, [r1]
	lsl r0, r4, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0xF
	bgt _08016B78
	mov r1, r8
	cmp r1, #0
	beq _08016AF8
	ldr r1, _08016AF4 @ =0x081A44FC
	lsl r0, r2, #1
	add r0, r0, r1
	ldrh r0, [r0]
	b _08016B04
	.align 2, 0
_08016AF0: .4byte 0x0000080C
_08016AF4: .4byte gUnk_081A44FC
_08016AF8:
	ldr r0, _08016B58 @ =0x081A44FC
	lsl r1, r2, #1
	add r1, r1, r0
	mov r0, #0x80
	ldrh r1, [r1]
	sub r0, r0, r1
_08016B04:
	lsl r0, r0, #0x10
	mov r1, #0x58
	orr r0, r1
	ldr r1, _08016B5C @ =0x000040C0
	ldr r2, _08016B60 @ =0x0000F364
	bl sub_080761F0
	ldr r0, _08016B64 @ =0x020185C0
	ldr r2, _08016B68 @ =0x0000080C
	add r3, r0, r2
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08016B6C @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _08016B70 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016B4A
	ldr r1, _08016B74 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08016B4A
	b _08016D16
_08016B4A:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0xB
	ble _08016B54
	b _08016D16
_08016B54:
	add r0, #3
	b _08016CDC
_08016B58: .4byte gUnk_081A44FC
_08016B5C: .4byte 0x000040C0
_08016B60: .4byte 0x0000F364
_08016B64: .4byte 0x020185C0
_08016B68: .4byte 0x0000080C
_08016B6C: .4byte 0xFFFFF01F
_08016B70: .4byte 0x03000040
_08016B74: .4byte 0x0201CFB0
_08016B78:
	ldr r0, _08016C04 @ =0xFFFFF01F
	and r0, r4
	strh r0, [r1]
	mov r1, #2
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r6]
	mov r0, #0x16
	bl sub_08077AEC
_08016B90:
	ldr r1, _08016C08 @ =0x020185C0
	ldr r0, _08016C0C @ =0x0000080C
	add r6, r1, r0
	ldrh r2, [r6]
	lsl r0, r2, #0x14
	lsr r5, r0, #0x19
	add r7, r1, #0
	cmp r5, #0x3F
	bgt _08016C28
	ldr r0, _08016C10 @ =0x00400058
	ldr r1, _08016C14 @ =0x000040C0
	ldr r2, _08016C18 @ =0x0000F364
	ldr r4, _08016C1C @ =0x081A4424
	mov r3, #0xF
	and r5, r3
	lsl r3, r5, #1
	add r3, r3, r4
	ldrh r3, [r3]
	lsl r3, r3, #0x10
	bl sub_08076714
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	add r0, #1
	mov r4, #0x7F
	and r0, r4
	lsl r0, r0, #5
	ldr r3, _08016C04 @ =0xFFFFF01F
	add r2, r3, #0
	and r2, r1
	orr r2, r0
	strh r2, [r6]
	ldr r1, _08016C20 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016BEC
	ldr r1, _08016C24 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08016BEC
	b _08016D16
_08016BEC:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #0x37
	ble _08016BF6
	b _08016D16
_08016BF6:
	add r0, #7
	and r0, r4
	lsl r0, r0, #5
	and r2, r3
	orr r2, r0
	strh r2, [r6]
	b _08016D16
_08016C04: .4byte 0xFFFFF01F
_08016C08: .4byte 0x020185C0
_08016C0C: .4byte 0x0000080C
_08016C10: .4byte 0x00400058
_08016C14: .4byte 0x000040C0
_08016C18: .4byte 0x0000F364
_08016C1C: .4byte gUnk_081A4424
_08016C20: .4byte 0x03000040
_08016C24: .4byte 0x0201CFB0
_08016C28:
	ldr r0, _08016C70 @ =0xFFFFF01F
	and r0, r2
	mov r2, #0x80
	lsl r2, r2, #2
	add r1, r2, #0
	orr r0, r1
	strh r0, [r6]
	ldr r0, _08016C74 @ =0x0000080A
	add r3, r7, r0
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
_08016C50:
	ldr r1, _08016C78 @ =0x0000080C
	add r0, r7, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x19
	cmp r2, #0
	beq _08016D08
	mov r0, r8
	cmp r0, #0
	beq _08016C80
	ldr r1, _08016C7C @ =0x081A44FC
	sub r0, r2, #1
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	b _08016C8E
_08016C70: .4byte 0xFFFFF01F
_08016C74: .4byte 0x0000080A
_08016C78: .4byte 0x0000080C
_08016C7C: .4byte gUnk_081A44FC
_08016C80:
	ldr r0, _08016CE8 @ =0x081A44FC
	sub r1, r2, #1
	lsl r1, r1, #1
	add r1, r1, r0
	mov r0, #0x80
	ldrh r1, [r1]
	sub r0, r0, r1
_08016C8E:
	lsl r0, r0, #0x10
	mov r1, #0x58
	orr r0, r1
	ldr r1, _08016CEC @ =0x000040C0
	ldr r2, _08016CF0 @ =0x0000F364
	bl sub_080761F0
	ldr r0, _08016CF4 @ =0x020185C0
	ldr r1, _08016CF8 @ =0x0000080C
	add r3, r0, r1
	ldrh r1, [r3]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	sub r0, #1
	mov r5, #0x7F
	and r0, r5
	lsl r0, r0, #5
	ldr r4, _08016CFC @ =0xFFFFF01F
	add r2, r4, #0
	and r2, r1
	orr r2, r0
	strh r2, [r3]
	ldr r1, _08016D00 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08016CD2
	ldr r1, _08016D04 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08016D16
_08016CD2:
	lsl r0, r2, #0x14
	lsr r0, r0, #0x19
	cmp r0, #4
	ble _08016D16
	sub r0, #3
_08016CDC:
	and r0, r5
	lsl r0, r0, #5
	and r2, r4
	orr r2, r0
	strh r2, [r3]
	b _08016D16
_08016CE8: .4byte gUnk_081A44FC
_08016CEC: .4byte 0x000040C0
_08016CF0: .4byte 0x0000F364
_08016CF4: .4byte 0x020185C0
_08016CF8: .4byte 0x0000080C
_08016CFC: .4byte 0xFFFFF01F
_08016D00: .4byte 0x03000040
_08016D04: .4byte 0x0201CFB0
_08016D08:
	ldr r2, _08016D20 @ =0x0000080D
	add r1, r7, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08016D16:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08016D20: .4byte 0x0000080D
	thumb_func_end sub_08016A18

