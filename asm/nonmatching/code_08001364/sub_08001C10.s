	thumb_func_start sub_08001C10
sub_08001C10: @ 0x08001C10
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r2, #0
	ldr r3, _08001C54 @ =0x030048C6
	mov r0, #0
	mov r8, r0
	sub r4, r3, #2
	ldr r1, _08001C58 @ =0x0813ADF4
	ldr r7, _08001C5C @ =0xFFFFF00F
	mov r0, #0x2D
	neg r0, r0
	add r0, r0, r3
	mov ip, r0
	mov r6, #0xC1
	lsl r6, r6, #2
_08001C34:
	ldrh r0, [r1]
	cmp r0, r5
	bne _08001C60
	strh r2, [r3]
	ldrb r1, [r1, #2]
	lsl r1, r1, #4
	add r0, r7, #0
	ldrh r2, [r4]
	and r0, r2
	orr r0, r1
	strh r0, [r4]
	mov r1, r8
	mov r0, ip
	strb r1, [r0]
	b _08001C6C
	.align 2, 0
_08001C54: .4byte 0x030048C6
_08001C58: .4byte gUnk_0813ADF4
_08001C5C: .4byte 0xFFFFF00F
_08001C60:
	add r1, r1, r6
	add r2, #1
	mov r0, #0xF5
	lsl r0, r0, #1
	cmp r2, r0
	bls _08001C34
_08001C6C:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08001C10
	.align 2, 0

