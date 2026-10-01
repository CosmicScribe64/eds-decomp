	thumb_func_start sub_08055AB4
sub_08055AB4: @ 0x08055AB4
	ldr r1, _08055B18 @ =0x0201CF90
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _08055B1C @ =0xFFFFF01F
	ldrh r3, [r1, #0xE]
	and r0, r3
	strh r0, [r1, #0xE]
	mov r0, #0xF
	ldrb r2, [r1, #0xF]
	and r0, r2
	strb r0, [r1, #0xF]
	mov r0, #8
	neg r0, r0
	ldrb r3, [r1, #0x10]
	and r0, r3
	strb r0, [r1, #0x10]
	ldr r0, _08055B20 @ =0xFFFFF807
	ldrh r2, [r1, #0x10]
	and r0, r2
	strh r0, [r1, #0x10]
	ldr r0, [r1, #0x10]
	ldr r2, _08055B24 @ =0xFFF807FF
	and r0, r2
	str r0, [r1, #0x10]
	mov r3, #1
	ldrb r0, [r1, #0xE]
	orr r0, r3
	mov r2, #2
	orr r0, r2
	strb r0, [r1, #0xE]
	add r2, r1, #0
	add r2, #8
	ldr r1, [r1, #8]
	lsl r1, r1, #0x13
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	and r0, r3
	lsl r0, r0, #4
	mov r1, #0x11
	neg r1, r1
	ldrb r3, [r2, #1]
	and r1, r3
	orr r1, r0
	strb r1, [r2, #1]
	bx lr
	.align 2, 0
_08055B18: .4byte 0x0201CF90
_08055B1C: .4byte 0xFFFFF01F
_08055B20: .4byte 0xFFFFF807
_08055B24: .4byte 0xFFF807FF
	thumb_func_end sub_08055AB4

