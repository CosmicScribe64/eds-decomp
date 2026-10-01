	thumb_func_start sub_08042AB0
sub_08042AB0: @ 0x08042AB0
	push {r4, r5, r6, lr}
	sub sp, #8
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	ldr r1, _08042AF4 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08042B04
	ldr r1, _08042AF8 @ =0x020192E0
	ldr r0, _08042AFC @ =0x00001B12
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042B04
	mov r1, sp
	mov r0, #1
	sub r0, r0, r6
	strh r0, [r1]
	mov r0, sp
	strh r4, [r0, #2]
	strh r2, [r0, #4]
	lsr r0, r2, #0x10
	strh r0, [r1, #6]
	ldr r0, _08042B00 @ =0x0000F059
	mov r2, #0xA
	bl sub_080229BC
	b _08042B4C
	.align 2, 0
_08042AF4: .4byte 0x02015EE8
_08042AF8: .4byte 0x020192E0
_08042AFC: .4byte 0x00001B12
_08042B00: .4byte 0x0000F059
_08042B04:
	ldr r3, _08042B54 @ =0x02017A40
	ldr r1, _08042B58 @ =0x0000048A
	add r0, r3, r1
	mov r5, #0
	strh r4, [r0]
	add r1, #2
	add r0, r3, r1
	str r2, [r0]
	ldr r0, _08042B5C @ =0x00000491
	add r4, r3, r0
	mov r0, #1
	add r2, r6, #0
	and r2, r0
	lsl r1, r2, #4
	mov r0, #0x11
	neg r0, r0
	ldrb r6, [r4]
	and r0, r6
	orr r0, r1
	lsl r2, r2, #5
	mov r1, #0x21
	neg r1, r1
	and r0, r1
	orr r0, r2
	mov r1, #0x92
	lsl r1, r1, #3
	add r3, r3, r1
	strb r5, [r3]
	mov r1, #0x10
	neg r1, r1
	and r0, r1
	mov r1, #0x40
	orr r0, r1
	mov r1, #0x7F
	and r0, r1
	strb r0, [r4]
_08042B4C:
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08042B54: .4byte 0x02017A40
_08042B58: .4byte 0x0000048A
_08042B5C: .4byte 0x00000491
	thumb_func_end sub_08042AB0

