	thumb_func_start sub_08060AAC
sub_08060AAC: @ 0x08060AAC
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	mov r3, #0x80
	lsl r3, r3, #0x13
	ldrh r2, [r3]
	ldr r1, _08060B0C @ =0x0000E0FF
	and r1, r2
	strh r1, [r3]
	ldr r5, _08060B10 @ =0x04000208
	mov r6, #0
	strh r6, [r5]
	ldr r3, _08060B14 @ =0x04000200
	ldrh r4, [r3]
	ldr r2, _08060B18 @ =0x0000FFFD
	add r1, r2, #0
	and r1, r4
	strh r1, [r3]
	mov r4, #1
	strh r4, [r5]
	strh r6, [r5]
	ldrh r1, [r3]
	and r2, r1
	strh r2, [r3]
	ldr r1, _08060B1C @ =0x03000000
	mov r2, #0
	str r2, [r1, #4]
	strh r4, [r5]
	ldr r1, _08060B20 @ =0x03000040
	ldr r3, _08060B24 @ =0x00000414
	add r1, r1, r3
	str r2, [r1]
	cmp r0, #0
	beq _08060B04
	bl sub_080759F4
	ldr r0, _08060B28 @ =0x0201CFB0
	mov r1, #5
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #3
	neg r2, r2
	and r1, r2
	strb r1, [r0]
_08060B04:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08060B0C: .4byte 0x0000E0FF
_08060B10: .4byte 0x04000208
_08060B14: .4byte 0x04000200
_08060B18: .4byte 0x0000FFFD
_08060B1C: .4byte 0x03000000
_08060B20: .4byte 0x03000040
_08060B24: .4byte 0x00000414
_08060B28: .4byte 0x0201CFB0
	thumb_func_end sub_08060AAC

