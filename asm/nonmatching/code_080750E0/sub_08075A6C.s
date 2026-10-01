	thumb_func_start sub_08075A6C
sub_08075A6C: @ 0x08075A6C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r1, _08075AC8 @ =0x04000050
	ldr r2, _08075ACC @ =0x00003FFF
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _08075AD0 @ =0x03000040
	ldr r0, _08075AD4 @ =0x00004832
	add r4, r1, r0
	ldrb r3, [r4]
	lsl r2, r3, #0x1A
	lsr r0, r2, #0x1A
	add r6, r1, #0
	cmp r0, #0x1E
	bhi _08075AAE
	add r1, r0, #0
	add r1, r1, r5
	mov r0, #0x3F
	and r1, r0
	mov r5, #0x40
	neg r5, r5
	add r2, r5, #0
	and r2, r3
	orr r2, r1
	strb r2, [r4]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1A
	cmp r0, #0x1F
	bls _08075AAE
	and r2, r5
	mov r0, #0x1F
	orr r2, r0
	strb r2, [r4]
_08075AAE:
	ldr r2, _08075AD8 @ =0x04000054
	ldr r1, _08075AD4 @ =0x00004832
	add r0, r6, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r1, r0, #0x1A
	strh r1, [r2]
	add r0, r1, #0
	cmp r0, #0x1E
	bls _08075ADC
	mov r0, #1
	b _08075ADE
	.align 2, 0
_08075AC8: .4byte 0x04000050
_08075ACC: .4byte 0x00003FFF
_08075AD0: .4byte 0x03000040
_08075AD4: .4byte 0x00004832
_08075AD8: .4byte 0x04000054
_08075ADC:
	mov r0, #0
_08075ADE:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08075A6C

