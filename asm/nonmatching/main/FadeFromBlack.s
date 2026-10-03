	thumb_func_start FadeFromBlack
FadeFromBlack: @ 0x08075AE4
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r1, _08075B0C @ =0x03000040
	ldr r0, _08075B10 @ =0x00004832
	add r5, r1, r0
	ldrb r2, [r5]
	lsl r3, r2, #0x1A
	lsr r0, r3, #0x1A
	add r6, r1, #0
	cmp r0, r4
	ble _08075B14
	sub r0, r0, r4
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x40
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r5]
	b _08075B1C
_08075B0C: .4byte 0x03000040
_08075B10: .4byte 0x00004832
_08075B14:
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	strb r0, [r5]
_08075B1C:
	ldr r1, _08075B34 @ =0x00004832
	add r0, r6, r1
	ldrb r2, [r0]
	mov r0, #0x3F
	and r0, r2
	cmp r0, #0
	bne _08075B38
	bl ClearBlend
	mov r0, #1
	b _08075B4A
	.align 2, 0
_08075B34: .4byte 0x00004832
_08075B38:
	ldr r1, _08075B50 @ =0x04000054
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1A
	strh r0, [r1]
	sub r1, #4
	ldr r2, _08075B54 @ =0x00003FFF
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0
_08075B4A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08075B50: .4byte 0x04000054
_08075B54: .4byte 0x00003FFF
	thumb_func_end FadeFromBlack

