	thumb_func_start sub_08060C68
sub_08060C68: @ 0x08060C68
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xE0
	lsl r3, r3, #3
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	ldr r1, _08060CA0 @ =0x03000040
	ldr r0, _08060CA4 @ =0x00004832
	add r4, r1, r0
	ldrb r2, [r4]
	lsl r3, r2, #0x1A
	lsr r0, r3, #0x1A
	add r6, r1, #0
	cmp r0, r5
	ble _08060CA8
	sub r0, r0, r5
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x40
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _08060CB0
_08060CA0: .4byte 0x03000040
_08060CA4: .4byte 0x00004832
_08060CA8:
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	strb r0, [r4]
_08060CB0:
	ldr r1, _08060CC8 @ =0x00004832
	add r0, r6, r1
	ldrb r2, [r0]
	mov r0, #0x3F
	and r0, r2
	cmp r0, #0
	bne _08060CCC
	bl sub_080757F4
	mov r0, #1
	b _08060CDE
	.align 2, 0
_08060CC8: .4byte 0x00004832
_08060CCC:
	ldr r1, _08060CE4 @ =0x04000054
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1A
	strh r0, [r1]
	sub r1, #4
	ldr r2, _08060CE8 @ =0x000027E7
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0
_08060CDE:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08060CE4: .4byte 0x04000054
_08060CE8: .4byte 0x000027E7
	thumb_func_end sub_08060C68

