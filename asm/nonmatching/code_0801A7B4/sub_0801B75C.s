	thumb_func_start sub_0801B75C
sub_0801B75C: @ 0x0801B75C
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r0, _0801B77C @ =0x03000040
	ldr r1, _0801B780 @ =0x0000488A
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xA
	bls _0801B772
	b _0801BCF2
_0801B772:
	lsl r0, r0, #2
	ldr r1, _0801B784 @ =0x0801B788
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0801B77C: .4byte 0x03000040
_0801B780: .4byte 0x0000488A
_0801B784: .4byte 0x0801B788
_0801B788:
	.4byte _0801B7B4
	.4byte _0801B7F8
	.4byte _0801BCF2
	.4byte _0801BCF2
	.4byte _0801BCF2
	.4byte _0801BCF2
	.4byte _0801BCF2
	.4byte _0801BC60
	.4byte _0801BCB8
	.4byte _0801BCC0
	.4byte _0801BCE8
_0801B7B4:
	bl sub_0800817C
	ldr r0, _0801B7D0 @ =0x020192E4
	ldrb r0, [r0, #3]
	cmp r0, #0x27
	bhi _0801B7DC
	ldr r0, _0801B7D4 @ =0x03000040
	ldr r2, _0801B7D8 @ =0x00004857
	add r0, r0, r2
	mov r1, #0xA
	strb r1, [r0]
_0801B7CA:
	mov r0, #0
	b _0801BCF4
	.align 2, 0
_0801B7D0: .4byte 0x020192E4
_0801B7D4: .4byte 0x03000040
_0801B7D8: .4byte 0x00004857
_0801B7DC:
	ldr r2, _0801B8AC @ =0x03000040
	ldr r3, _0801B8B0 @ =0x0000488A
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801B8B4 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0801B7F8:
	mov r0, sp
	bl sub_08004914
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl sub_080044E4
	add r4, r0, #0
	ldr r0, [sp, #0]
	ldr r1, _0801B8B8 @ =0x001FF000
	and r0, r1
	mov r1, #0xFE
	lsl r1, r1, #0xD
	cmp r0, r1
	bne _0801B830
	ldr r0, _0801B8BC @ =0x02011C20
	ldr r2, _0801B8C0 @ =0x0000215E
	add r1, r0, r2
	mov r2, #0
	strh r2, [r1]
	ldr r3, _0801B8C4 @ =0x00002160
	add r0, r0, r3
	strh r2, [r0]
_0801B830:
	ldr r6, _0801B8AC @ =0x03000040
	ldr r0, _0801B8C8 @ =0x00004888
	add r2, r6, r0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0xD
	neg r1, r1
	and r0, r1
	mov r1, #4
	orr r0, r1
	mov r1, #0x31
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r2, _0801B8CC @ =0x00004889
	add r1, r6, r2
	mov r0, #0
	strb r0, [r1]
	mov r5, #0x80
	lsl r5, r5, #0x11
	add r0, r4, #0
	and r0, r5
	cmp r0, #0
	beq _0801B8DC
	bl sub_08076F9C
	ldr r4, _0801B8D0 @ =0x08081A28
	mov r1, #5
	bl __modsi3
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r3, _0801B8D4 @ =0x00004870
	add r2, r6, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0xC9
	bl sub_08001C10
	mov r0, #0x31
	bl sub_08077B24
	ldr r1, _0801B8D8 @ =0x0000487C
	add r0, r6, r1
	str r5, [r0]
	ldr r3, _0801B8B0 @ =0x0000488A
	add r2, r6, r3
	ldr r0, _0801B8B4 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x90
	b _0801BC2C
_0801B8AC: .4byte 0x03000040
_0801B8B0: .4byte 0x0000488A
_0801B8B4: .4byte 0xFFFFF00F
_0801B8B8: .4byte 0x001FF000
_0801B8BC: .4byte 0x02011C20
_0801B8C0: .4byte 0x0000215E
_0801B8C4: .4byte 0x00002160
_0801B8C8: .4byte 0x00004888
_0801B8CC: .4byte 0x00004889
_0801B8D0: .4byte gUnk_08081A28
_0801B8D4: .4byte 0x00004870
_0801B8D8: .4byte 0x0000487C
_0801B8DC:
	mov r5, #0x80
	lsl r5, r5, #0x12
	add r0, r4, #0
	and r0, r5
	cmp r0, #0
	beq _0801B948
	bl sub_08076F9C
	ldr r4, _0801B934 @ =0x08081A28
	mov r1, #5
	bl __modsi3
	add r0, #5
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r3, _0801B938 @ =0x00004870
	add r2, r6, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0xCB
	bl sub_08001C10
	mov r0, #0x31
	bl sub_08077B24
	ldr r1, _0801B93C @ =0x0000487C
	add r0, r6, r1
	str r5, [r0]
	ldr r3, _0801B940 @ =0x0000488A
	add r2, r6, r3
	ldr r0, _0801B944 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x90
	b _0801BC2C
	.align 2, 0
_0801B934: .4byte gUnk_08081A28
_0801B938: .4byte 0x00004870
_0801B93C: .4byte 0x0000487C
_0801B940: .4byte 0x0000488A
_0801B944: .4byte 0xFFFFF00F
_0801B948:
	mov r5, #0x80
	lsl r5, r5, #0x13
	add r0, r4, #0
	and r0, r5
	cmp r0, #0
	beq _0801B9B4
	bl sub_08076F9C
	ldr r4, _0801B9A0 @ =0x08081A28
	mov r1, #5
	bl __modsi3
	add r0, #0xA
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r3, _0801B9A4 @ =0x00004870
	add r2, r6, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0xCD
	bl sub_08001C10
	mov r0, #0x31
	bl sub_08077B24
	ldr r1, _0801B9A8 @ =0x0000487C
	add r0, r6, r1
	str r5, [r0]
	ldr r3, _0801B9AC @ =0x0000488A
	add r2, r6, r3
	ldr r0, _0801B9B0 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x90
	b _0801BC2C
	.align 2, 0
_0801B9A0: .4byte gUnk_08081A28
_0801B9A4: .4byte 0x00004870
_0801B9A8: .4byte 0x0000487C
_0801B9AC: .4byte 0x0000488A
_0801B9B0: .4byte 0xFFFFF00F
_0801B9B4:
	mov r7, #0x80
	lsl r7, r7, #0x14
	add r5, r4, #0
	and r5, r7
	cmp r5, #0
	beq _0801BA20
	bl sub_08076F9C
	ldr r4, _0801BA0C @ =0x08081A28
	mov r1, #5
	bl __modsi3
	add r0, #0xF
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r3, _0801BA10 @ =0x00004870
	add r2, r6, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	ldr r1, _0801BA14 @ =0x0000487C
	add r0, r6, r1
	str r7, [r0]
	mov r0, #0xCF
	bl sub_08001C10
	ldr r3, _0801BA18 @ =0x0000488A
	add r2, r6, r3
	ldr r0, _0801BA1C @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x90
	orr r0, r1
	strh r0, [r2]
	mov r0, #0x32
	b _0801BB82
	.align 2, 0
_0801BA0C: .4byte gUnk_08081A28
_0801BA10: .4byte 0x00004870
_0801BA14: .4byte 0x0000487C
_0801BA18: .4byte 0x0000488A
_0801BA1C: .4byte 0xFFFFF00F
_0801BA20:
	mov r7, #0x80
	lsl r7, r7, #0x15
	add r0, r4, #0
	and r0, r7
	cmp r0, #0
	beq _0801BA9C
	bl sub_08076F9C
	ldr r2, _0801BA7C @ =0x08081A50
	mov r1, #3
	and r1, r0
	lsl r1, r1, #1
	add r1, r1, r2
	ldr r2, _0801BA80 @ =0x00004870
	add r3, r6, r2
	mov r2, #0x1F
	ldrb r1, [r1]
	and r2, r1
	lsl r2, r2, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	orr r0, r2
	strb r0, [r3]
	ldr r2, _0801BA84 @ =0x0000487C
	add r0, r6, r2
	str r7, [r0]
	ldr r0, _0801BA88 @ =0x000002BD
	bl sub_08001C10
	ldr r3, _0801BA8C @ =0x0000488A
	add r2, r6, r3
	ldr r0, _0801BA90 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x90
	orr r0, r1
	strh r0, [r2]
	ldr r0, _0801BA94 @ =0x02011C20
	ldr r2, _0801BA98 @ =0x00002160
	add r0, r0, r2
	strh r5, [r0]
	mov r0, #0x33
	b _0801BB82
	.align 2, 0
_0801BA7C: .4byte gUnk_08081A50
_0801BA80: .4byte 0x00004870
_0801BA84: .4byte 0x0000487C
_0801BA88: .4byte 0x000002BD
_0801BA8C: .4byte 0x0000488A
_0801BA90: .4byte 0xFFFFF00F
_0801BA94: .4byte 0x02011C20
_0801BA98: .4byte 0x00002160
_0801BA9C:
	mov r5, #0x80
	lsl r5, r5, #0x16
	add r0, r4, #0
	and r0, r5
	cmp r0, #0
	beq _0801BB08
	bl sub_08076F9C
	ldr r4, _0801BAF0 @ =0x08081A58
	mov r1, #5
	bl __umodsi3
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r3, _0801BAF4 @ =0x00004870
	add r2, r6, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	ldr r1, _0801BAF8 @ =0x0000487C
	add r0, r6, r1
	str r5, [r0]
	ldr r0, _0801BAFC @ =0x000002BE
	bl sub_08001C10
	ldr r3, _0801BB00 @ =0x0000488A
	add r2, r6, r3
	ldr r0, _0801BB04 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x90
	orr r0, r1
	strh r0, [r2]
	mov r0, #0x33
	b _0801BB82
_0801BAF0: .4byte gUnk_08081A58
_0801BAF4: .4byte 0x00004870
_0801BAF8: .4byte 0x0000487C
_0801BAFC: .4byte 0x000002BE
_0801BB00: .4byte 0x0000488A
_0801BB04: .4byte 0xFFFFF00F
_0801BB08:
	mov r7, #0x80
	lsl r7, r7, #0xF
	add r0, r4, #0
	and r0, r7
	cmp r0, #0
	beq _0801BBA0
	mov r6, #1
	bl sub_08063BAC
	cmp r0, #0
	beq _0801BB20
	mov r6, #2
_0801BB20:
	bl sub_08063C14
	cmp r0, #0
	beq _0801BB2A
	add r6, #1
_0801BB2A:
	bl sub_08063C7C
	cmp r0, #0
	beq _0801BB34
	add r6, #1
_0801BB34:
	lsl r0, r6, #2
	add r6, r0, r6
	bl sub_08076F9C
	ldr r4, _0801BB88 @ =0x03000040
	ldr r5, _0801BB8C @ =0x08081A28
	add r1, r6, #0
	bl __modsi3
	lsl r0, r0, #1
	add r0, r0, r5
	ldr r3, _0801BB90 @ =0x00004870
	add r2, r4, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0xFA
	lsl r0, r0, #1
	bl sub_08001C10
	ldr r1, _0801BB94 @ =0x0000487C
	add r0, r4, r1
	str r7, [r0]
	ldr r2, _0801BB98 @ =0x0000488A
	add r4, r4, r2
	ldr r0, _0801BB9C @ =0xFFFFF00F
	ldrh r3, [r4]
	and r0, r3
	mov r1, #0x90
	orr r0, r1
	strh r0, [r4]
	mov r0, #0x30
_0801BB82:
	bl sub_08077B24
	b _0801B7CA
_0801BB88: .4byte 0x03000040
_0801BB8C: .4byte gUnk_08081A28
_0801BB90: .4byte 0x00004870
_0801BB94: .4byte 0x0000487C
_0801BB98: .4byte 0x0000488A
_0801BB9C: .4byte 0xFFFFF00F
_0801BBA0:
	mov r0, #5
	bl sub_0801B640
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801BC00
	ldr r0, _0801BBEC @ =0x02011C20
	ldr r2, _0801BBF0 @ =0x00002150
	add r1, r0, r2
	ldrh r0, [r1]
	cmp r0, #0
	beq _0801BC00
	mov r1, #0x3C
	bl __umodsi3
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801BC00
	mov r0, #0xE1
	lsl r0, r0, #2
	bl sub_08001C10
	mov r0, #0x1A
	bl sub_08077B24
	ldr r3, _0801BBF4 @ =0x0000487C
	add r1, r6, r3
	mov r0, #0x80
	lsl r0, r0, #0x10
	str r0, [r1]
	ldr r0, _0801BBF8 @ =0x0000488A
	add r2, r6, r0
	ldr r0, _0801BBFC @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x70
	b _0801BC2C
	.align 2, 0
_0801BBEC: .4byte 0x02011C20
_0801BBF0: .4byte 0x00002150
_0801BBF4: .4byte 0x0000487C
_0801BBF8: .4byte 0x0000488A
_0801BBFC: .4byte 0xFFFFF00F
_0801BC00:
	ldr r0, _0801BC34 @ =0x02011C20
	ldr r3, _0801BC38 @ =0x00002164
	add r2, r0, r3
	ldrh r1, [r2]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _0801BC4C
	ldr r0, _0801BC3C @ =0x0000FFFE
	and r0, r1
	strh r0, [r2]
	mov r0, #0xAF
	lsl r0, r0, #1
	bl sub_08001C10
	ldr r2, _0801BC40 @ =0x03000040
	ldr r0, _0801BC44 @ =0x0000488A
	add r2, r2, r0
	ldr r0, _0801BC48 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xA0
_0801BC2C:
	orr r0, r1
	strh r0, [r2]
	b _0801B7CA
	.align 2, 0
_0801BC34: .4byte 0x02011C20
_0801BC38: .4byte 0x00002164
_0801BC3C: .4byte 0x0000FFFE
_0801BC40: .4byte 0x03000040
_0801BC44: .4byte 0x0000488A
_0801BC48: .4byte 0xFFFFF00F
_0801BC4C:
	ldr r0, _0801BC58 @ =0x03000040
	ldr r2, _0801BC5C @ =0x0000487C
	add r0, r0, r2
	str r4, [r0]
	b _0801BCF2
	.align 2, 0
_0801BC58: .4byte 0x03000040
_0801BC5C: .4byte 0x0000487C
_0801BC60:
	bl sub_08001AE4
	cmp r0, #0
	bne _0801BC6A
	b _0801B7CA
_0801BC6A:
	bl sub_08076F9C
	ldr r4, _0801BCA8 @ =0x03000040
	ldr r5, _0801BCAC @ =0x08081A62
	mov r1, #5
	bl __modsi3
	lsl r0, r0, #1
	add r0, r0, r5
	ldr r3, _0801BCB0 @ =0x00004870
	add r2, r4, r3
	mov r1, #0x1F
	ldrb r0, [r0]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x1B
	bl sub_08077B24
	ldr r0, _0801BCB4 @ =0x00004888
	add r4, r4, r0
	mov r0, #2
	ldrb r1, [r4]
	orr r0, r1
	strb r0, [r4]
	b _0801BCF2
_0801BCA8: .4byte 0x03000040
_0801BCAC: .4byte gUnk_08081A62
_0801BCB0: .4byte 0x00004870
_0801BCB4: .4byte 0x00004888
_0801BCB8:
	mov r0, #0
	bl sub_080754F8
	b _0801B7CA
_0801BCC0:
	bl sub_08001AE4
	cmp r0, #0
	bne _0801BCCA
	b _0801B7CA
_0801BCCA:
	ldr r0, _0801BCE0 @ =0x03000040
	ldr r2, _0801BCE4 @ =0x00004888
	add r0, r0, r2
	mov r1, #2
	ldrb r3, [r0]
	orr r1, r3
	mov r2, #0xC
	orr r1, r2
	strb r1, [r0]
	b _0801BCF2
	.align 2, 0
_0801BCE0: .4byte 0x03000040
_0801BCE4: .4byte 0x00004888
_0801BCE8:
	bl sub_08001AE4
	cmp r0, #0
	bne _0801BCF2
	b _0801B7CA
_0801BCF2:
	mov r0, #1
_0801BCF4:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0801B75C

