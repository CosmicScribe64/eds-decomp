	thumb_func_start sub_08029F04
sub_08029F04: @ 0x08029F04
	push {r4, r5, lr}
	ldr r4, _08029FA0 @ =0x00000169
	cmp r0, #1
	bne _08029F0E
	sub r4, #0xF
_08029F0E:
	mov r1, #0x80
	lsl r1, r1, #7
	add r0, r4, r1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r5, _08029FA4 @ =0x03000040
	ldr r2, _08029FA8 @ =0x00000C9C
	add r3, r5, r2
	mov r2, #7
_08029F20:
	add r1, r4, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	strh r1, [r3]
	add r3, #2
	sub r2, #1
	cmp r2, #0
	bge _08029F20
	ldr r0, _08029FA4 @ =0x03000040
	ldr r3, _08029FAC @ =0x00000CAC
	add r0, r0, r3
	mov r2, #0x14
_08029F3A:
	strh r4, [r0]
	add r0, #2
	sub r2, #1
	cmp r2, #0
	bge _08029F3A
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r1, r4, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r2, r4, #0
	ldr r3, _08029FB0 @ =0x00000CD6
	add r0, r5, r3
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r3, r4, #0
	ldr r1, _08029FB4 @ =0x00000CDC
	add r0, r5, r1
	strh r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r1, r4, #0
	ldr r2, _08029FB8 @ =0x00000D16
	add r0, r5, r2
	strh r3, [r0]
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	ldr r3, _08029FBC @ =0x00000D1C
	add r0, r5, r3
	strh r1, [r0]
	ldr r1, _08029FC0 @ =0x00000D1E
	add r0, r5, r1
	mov r2, #0x1B
_08029F88:
	strh r4, [r0]
	add r0, #2
	sub r2, #1
	cmp r2, #0
	bge _08029F88
	add r1, r4, #1
	ldr r2, _08029FC4 @ =0x00000D56
	add r0, r5, r2
	strh r1, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
_08029FA0: .4byte 0x00000169
_08029FA4: .4byte 0x03000040
_08029FA8: .4byte 0x00000C9C
_08029FAC: .4byte 0x00000CAC
_08029FB0: .4byte 0x00000CD6
_08029FB4: .4byte 0x00000CDC
_08029FB8: .4byte 0x00000D16
_08029FBC: .4byte 0x00000D1C
_08029FC0: .4byte 0x00000D1E
_08029FC4: .4byte 0x00000D56
	thumb_func_end sub_08029F04

