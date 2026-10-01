	thumb_func_start sub_0801ABCC
sub_0801ABCC: @ 0x0801ABCC
	push {r4, r5, lr}
	mov r0, #0xEE
	lsl r0, r0, #8
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801ABEC
	mov r0, #0
	b _0801AC28
_0801ABEC:
	bl sub_08077BCC
	ldr r4, _0801AC30 @ =0x04000208
	mov r5, #0
	strh r5, [r4]
	ldr r2, _0801AC34 @ =0x04000200
	ldrh r3, [r2]
	ldr r1, _0801AC38 @ =0x0000FFFD
	add r0, r1, #0
	and r0, r3
	strh r0, [r2]
	mov r3, #1
	strh r3, [r4]
	strh r5, [r4]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r0, _0801AC3C @ =0x03000000
	mov r1, #0
	str r1, [r0, #4]
	strh r3, [r4]
	ldr r0, _0801AC40 @ =0x03000040
	ldr r2, _0801AC44 @ =0x00000414
	add r0, r0, r2
	str r1, [r0]
	mov r0, #0xA0
	lsl r0, r0, #1
	bl sub_08001C10
	mov r0, #1
_0801AC28:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0801AC30: .4byte 0x04000208
_0801AC34: .4byte 0x04000200
_0801AC38: .4byte 0x0000FFFD
_0801AC3C: .4byte 0x03000000
_0801AC40: .4byte 0x03000040
_0801AC44: .4byte 0x00000414
	thumb_func_end sub_0801ABCC

