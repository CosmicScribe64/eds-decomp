	thumb_func_start LinkSioMain
LinkSioMain: @ 0x080740BC
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	ldr r5, _080740D4 @ =0x03005B60
	ldr r1, _080740D8 @ =0x00000A1F
	add r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _080740DC
	cmp r0, #1
	beq _0807415E
	b _08074182
	.align 2, 0
_080740D4: .4byte 0x03005B60
_080740D8: .4byte 0x00000A1F
_080740DC:
	ldr r2, _080741A4 @ =0x00000B0C
	add r1, r5, r2
	ldr r6, _080741A8 @ =0x04000128
	ldr r0, [r6]
	str r0, [r1]
	ldrb r1, [r1]
	mov r4, #0x88
	and r4, r1
	cmp r4, #8
	bne _08074182
	mov r0, #4
	and r0, r1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #0
	bne _08074142
	ldr r3, _080741AC @ =0x00000A2C
	add r0, r5, r3
	ldr r0, [r0]
	cmp r0, #0xC
	bne _08074142
	ldr r3, _080741B0 @ =0x04000208
	strh r1, [r3]
	ldr r2, _080741B4 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _080741B8 @ =0x0000FF7F
	and r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
	mov r2, #1
	strh r2, [r3]
	ldrb r1, [r6, #1]
	mov r0, #0x41
	neg r0, r0
	and r0, r1
	strb r0, [r6, #1]
	ldr r1, _080741BC @ =0x04000202
	mov r0, #0xC0
	strh r0, [r1]
	sub r1, #0xF6
	ldr r0, _080741C0 @ =0x0000B1FC
	str r0, [r1]
	ldr r1, _080741C4 @ =0x00000A1E
	add r0, r5, r1
	strb r4, [r0]
	ldr r3, _080741C8 @ =0x00000A24
	add r0, r5, r3
	strb r2, [r0]
_08074142:
	ldr r1, _080741CC @ =0x03005B60
	mov r0, #0xA4
	lsl r0, r0, #4
	add r2, r1, r0
	ldrh r0, [r2]
	cmp r0, #0
	bne _08074156
	mov r0, #0x80
	lsl r0, r0, #5
	strh r0, [r2]
_08074156:
	ldr r2, _080741D0 @ =0x00000A1F
	add r1, r1, r2
	mov r0, #1
	strb r0, [r1]
_0807415E:
	add r0, r7, #0
	bl LinkSioCheckRecvData
	ldr r2, _080741CC @ =0x03005B60
	ldr r3, _080741D4 @ =0x00000B14
	add r1, r2, r3
	strh r0, [r1]
	mov r1, #3
	and r1, r0
	cmp r1, #0
	bne _08074182
	ldr r1, _080741C4 @ =0x00000A1E
	add r0, r2, r1
	ldrb r0, [r0]
	cmp r0, #8
	bne _08074182
	bl LinkSioStartTransfer
_08074182:
	ldr r0, _080741CC @ =0x03005B60
	ldr r3, _080741D4 @ =0x00000B14
	add r2, r0, r3
	ldrh r1, [r2]
	sub r3, #0xF6
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #8
	bne _08074198
	mov r0, #0x80
	orr r1, r0
_08074198:
	strh r1, [r2]
	ldrh r0, [r2]
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080741A4: .4byte 0x00000B0C
_080741A8: .4byte 0x04000128
_080741AC: .4byte 0x00000A2C
_080741B0: .4byte 0x04000208
_080741B4: .4byte 0x04000200
_080741B8: .4byte 0x0000FF7F
_080741BC: .4byte 0x04000202
_080741C0: .4byte 0x0000B1FC
_080741C4: .4byte 0x00000A1E
_080741C8: .4byte 0x00000A24
_080741CC: .4byte 0x03005B60
_080741D0: .4byte 0x00000A1F
_080741D4: .4byte 0x00000B14
	thumb_func_end LinkSioMain

