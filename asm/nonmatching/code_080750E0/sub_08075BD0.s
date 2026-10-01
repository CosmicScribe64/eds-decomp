	thumb_func_start sub_08075BD0
sub_08075BD0: @ 0x08075BD0
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r1, _08075BF8 @ =0x03000040
	ldr r0, _08075BFC @ =0x00004832
	add r5, r1, r0
	ldrb r2, [r5]
	lsl r3, r2, #0x1A
	lsr r0, r3, #0x1A
	add r6, r1, #0
	cmp r0, r4
	ble _08075C00
	sub r0, r0, r4
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x40
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r5]
	b _08075C08
_08075BF8: .4byte 0x03000040
_08075BFC: .4byte 0x00004832
_08075C00:
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	strb r0, [r5]
_08075C08:
	ldr r1, _08075C20 @ =0x00004832
	add r0, r6, r1
	ldrb r2, [r0]
	mov r0, #0x3F
	and r0, r2
	cmp r0, #0
	bne _08075C24
	bl sub_080757F4
	mov r0, #1
	b _08075C36
	.align 2, 0
_08075C20: .4byte 0x00004832
_08075C24:
	ldr r1, _08075C3C @ =0x04000054
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1A
	strh r0, [r1]
	sub r1, #4
	ldr r2, _08075C40 @ =0x00003FBF
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0
_08075C36:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08075C3C: .4byte 0x04000054
_08075C40: .4byte 0x00003FBF
	thumb_func_end sub_08075BD0

