	thumb_func_start LinkSioSend
LinkSioSend: @ 0x08073784
	push {r4, r5, r6, r7, lr}
	mov ip, r0
	add r3, r1, #0
	ldr r0, _080737B0 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	neg r1, r0
	orr r1, r0
	lsr r1, r1, #0x1F
	ldr r4, _080737B4 @ =0x03005B60
	lsl r1, r1, #1
	ldr r2, _080737B8 @ =0x00000AF8
	add r0, r4, r2
	add r5, r1, r0
	ldrh r6, [r5]
	cmp r6, #0
	beq _080737BC
	mov r0, #0
	b _08073820
	.align 2, 0
_080737B0: .4byte 0x04000128
_080737B4: .4byte 0x03005B60
_080737B8: .4byte 0x00000AF8
_080737BC:
	sub r0, r3, #1
	cmp r0, #0xFF
	bhi _0807381E
	mov r2, #0xAF
	lsl r2, r2, #4
	add r0, r4, r2
	add r2, r1, r0
	add r0, r3, #0
	add r0, #0x10
	cmp r0, #0
	bge _080737D4
	add r0, #0xF
_080737D4:
	asr r0, r0, #4
	mov r7, #0
	strh r0, [r2]
	add r0, r3, #1
	strh r0, [r5]
	ldr r2, _080737F4 @ =0x00000AF4
	add r0, r4, r2
	add r0, r1, r0
	strh r6, [r0]
	cmp r3, #0xE
	bgt _080737F8
	mov r0, #0xC0
	lsl r0, r0, #6
	add r1, r0, #0
	b _080737FE
	.align 2, 0
_080737F4: .4byte 0x00000AF4
_080737F8:
	mov r2, #0x80
	lsl r2, r2, #6
	add r1, r2, #0
_080737FE:
	add r0, r3, #0
	orr r0, r1
	strh r0, [r4, #8]
	ldr r4, _08073828 @ =0x03005B6A
	lsr r2, r3, #0x1F
	add r2, r3, r2
	lsl r2, r2, #0xA
	lsr r2, r2, #0xB
	mov r0, ip
	add r1, r4, #0
	bl CpuSet
	sub r4, #2
	add r0, r4, #0
	bl LinkSioSetSendData
_0807381E:
	mov r0, #1
_08073820:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08073828: .4byte 0x03005B6A
	thumb_func_end LinkSioSend

