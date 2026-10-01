	thumb_func_start sub_08007B24
sub_08007B24: @ 0x08007B24
	push {r4, r5, r6, r7, lr}
	add r5, r2, #0
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	mov r2, #1
	and r2, r0
	ldr r0, _08007C38 @ =0x00000D64
	mul r2, r0
	ldr r0, _08007C3C @ =0x0201930C
	add r2, r2, r0
	mov r0, #0x94
	mul r0, r1
	mov r1, #0xB9
	lsl r1, r1, #2
	add r0, r0, r1
	add r4, r2, r0
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldr r0, _08007C40 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08007C44 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08007B74
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #2
	bne _08007B74
	mov r0, #0xB9
	lsl r0, r0, #3
	add r4, r2, r0
_08007B74:
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08007558
	ldr r1, _08007C48 @ =0x020192E0
	ldrh r2, [r1]
	add r0, r2, #1
	strh r0, [r1]
	strh r2, [r4, #4]
	mov r0, #2
	neg r0, r0
	ldrb r1, [r4, #6]
	and r0, r1
	mov r2, #1
	add r1, r7, #0
	and r1, r2
	lsl r1, r1, #1
	mov r2, #3
	neg r2, r2
	and r0, r2
	orr r0, r1
	mov r1, #0x3D
	neg r1, r1
	and r0, r1
	strb r0, [r4, #6]
	add r5, r4, #0
	add r5, #0x91
	mov r0, #5
	neg r0, r0
	ldrb r2, [r5]
	and r0, r2
	strb r0, [r5]
	add r2, r4, #0
	add r2, #0x90
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r1, #0
	ldrh r1, [r2]
	orr r0, r1
	strh r0, [r2]
	ldr r0, [r2]
	ldr r1, _08007C4C @ =0xFFFC1FFF
	and r0, r1
	str r0, [r2]
	mov r0, #9
	neg r0, r0
	ldrb r2, [r5]
	and r0, r2
	mov r1, #0x11
	neg r1, r1
	and r0, r1
	strb r0, [r5]
	add r1, r4, #0
	add r1, #0x92
	ldr r0, _08007C50 @ =0xFFFFFC03
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	cmp r7, #0
	bne _08007C32
	ldr r0, _08007C40 @ =0x000007FF
	and r6, r0
	lsl r0, r6, #2
	ldr r1, _08007C44 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08007C32
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #5
	beq _08007C32
	ldr r4, _08007C54 @ =0x0000049C
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _08007C32
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _08007C32
	mov r0, #4
	ldrb r2, [r5]
	orr r0, r2
	strb r0, [r5]
_08007C32:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08007C38: .4byte 0x00000D64
_08007C3C: .4byte 0x0201930C
_08007C40: .4byte 0x000007FF
_08007C44: .4byte gUnk_08621DE0
_08007C48: .4byte 0x020192E0
_08007C4C: .4byte 0xFFFC1FFF
_08007C50: .4byte 0xFFFFFC03
_08007C54: .4byte 0x0000049C
	thumb_func_end sub_08007B24

