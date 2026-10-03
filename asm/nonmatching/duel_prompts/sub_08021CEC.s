	thumb_func_start sub_08021CEC
sub_08021CEC: @ 0x08021CEC
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	add r0, r5, #0
	bl GetCardIconObjTile
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r6, #0
	bne _08021D34
	ldr r2, _08021D28 @ =0x081A4424
	ldr r1, _08021D2C @ =0x03000040
	ldr r0, _08021D30 @ =0x0000485E
	add r1, r1, r0
	mov r0, #0x1E
	ldrh r1, [r1]
	and r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	b _08021D38
	.align 2, 0
_08021D28: .4byte gPulseScaleCurve
_08021D2C: .4byte 0x03000040
_08021D30: .4byte 0x0000485E
_08021D34:
	mov r3, #0x80
	lsl r3, r3, #0x11
_08021D38:
	ldr r0, _08021D5C @ =0x00400050
	mov r1, #0x80
	add r2, r4, #0
	bl AddAffineSprite
	cmp r7, #0
	beq _08021D60
	add r0, r5, #0
	bl GetCardIconObjTile
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	b _08021D62
	.align 2, 0
_08021D5C: .4byte 0x00400050
_08021D60:
	mov r4, #0x40
_08021D62:
	cmp r6, #0
	beq _08021D8C
	ldr r2, _08021D80 @ =0x081A4424
	ldr r1, _08021D84 @ =0x03000040
	ldr r0, _08021D88 @ =0x0000485E
	add r1, r1, r0
	mov r0, #0x1E
	ldrh r1, [r1]
	and r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	mov r0, #0x20
	orr r3, r0
	b _08021D8E
_08021D80: .4byte gPulseScaleCurve
_08021D84: .4byte 0x03000040
_08021D88: .4byte 0x0000485E
_08021D8C:
	ldr r3, _08021DA0 @ =0x01000020
_08021D8E:
	ldr r0, _08021DA4 @ =0x004000A0
	mov r1, #0x80
	add r2, r4, #0
	bl AddAffineSprite
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021DA0: .4byte 0x01000020
_08021DA4: .4byte 0x004000A0
	thumb_func_end sub_08021CEC

