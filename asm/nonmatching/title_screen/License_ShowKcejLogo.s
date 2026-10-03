	thumb_func_start License_ShowKcejLogo
License_ShowKcejLogo: @ 0x08004D60
	push {r4, r5, r6, r7, lr}
	ldr r0, _08004D7C @ =0x03000040
	ldr r1, _08004D80 @ =0x00004858
	add r4, r0, r1
	ldrb r2, [r4]
	add r7, r0, #0
	cmp r2, #1
	beq _08004DAC
	cmp r2, #1
	bgt _08004D84
	cmp r2, #0
	beq _08004D8E
	b _08004E18
	.align 2, 0
_08004D7C: .4byte 0x03000040
_08004D80: .4byte 0x00004858
_08004D84:
	cmp r2, #2
	beq _08004DD0
	cmp r2, #3
	beq _08004DF4
	b _08004E18
_08004D8E:
	bl ClearBgMapBuffers
	ldr r3, _08004DA8 @ =0x087D292C
	mov r0, #0
	mov r1, #0
	mov r2, #0x20
	bl LoadBgImage
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08004E86
	.align 2, 0
_08004DA8: .4byte gKcejLogoImage
_08004DAC:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x80
	lsl r3, r3, #1
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #1
	bl FadeFromWhite
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004E86
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08004E86
_08004DD0:
	ldr r0, _08004DF0 @ =0x00004859
	add r2, r7, r0
	ldrb r0, [r2]
	add r1, r0, #1
	strb r1, [r2]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x77
	bls _08004E86
	mov r0, #0
	strb r0, [r2]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08004E86
	.align 2, 0
_08004DF0: .4byte 0x00004859
_08004DF4:
	mov r0, #1
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004E86
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08004E14 @ =0x0000FEFF
	and r0, r1
	strh r0, [r2]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08004E86
_08004E14: .4byte 0x0000FEFF
_08004E18:
	mov r1, #0x83
	lsl r1, r1, #3
	add r0, r7, r1
	mov r5, #0
	str r5, [r0]
	ldr r3, _08004E90 @ =0x00000414
	add r0, r7, r3
	str r5, [r0]
	ldr r6, _08004E94 @ =0x04000208
	mov r3, #0
	strh r5, [r6]
	ldr r2, _08004E98 @ =0x04000200
	ldrh r4, [r2]
	ldr r1, _08004E9C @ =0x0000FFFD
	add r0, r1, #0
	and r0, r4
	strh r0, [r2]
	mov r4, #1
	strh r4, [r6]
	strh r5, [r6]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r0, _08004EA0 @ =0x03000000
	str r5, [r0, #4]
	strh r4, [r6]
	ldr r1, _08004EA4 @ =0x00004878
	add r0, r7, r1
	strb r3, [r0]
	add r1, #1
	add r0, r7, r1
	strb r3, [r0]
	add r1, #1
	add r0, r7, r1
	strb r3, [r0]
	sub r1, #0x23
	add r0, r7, r1
	strb r3, [r0]
	add r1, #1
	add r0, r7, r1
	strb r3, [r0]
	add r1, #1
	add r0, r7, r1
	strb r3, [r0]
	add r1, #1
	add r0, r7, r1
	strb r3, [r0]
	add r1, #1
	add r0, r7, r1
	strb r3, [r0]
	mov r3, #0x82
	lsl r3, r3, #3
	add r1, r7, r3
	ldr r0, _08004EA8 @ =0x080057BD
	str r0, [r1]
_08004E86:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08004E90: .4byte 0x00000414
_08004E94: .4byte 0x04000208
_08004E98: .4byte 0x04000200
_08004E9C: .4byte 0x0000FFFD
_08004EA0: .4byte 0x03000000
_08004EA4: .4byte 0x00004878
_08004EA8: .4byte CB_Title
	thumb_func_end License_ShowKcejLogo

