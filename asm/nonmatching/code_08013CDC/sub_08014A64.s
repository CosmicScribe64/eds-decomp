	thumb_func_start sub_08014A64
sub_08014A64: @ 0x08014A64
	push {r4, lr}
	ldr r2, _08014A98 @ =0x020192E0
	ldr r3, _08014A9C @ =0x020185C0
	ldr r0, _08014AA0 @ =0x00001ACD
	add r2, r2, r0
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	lsl r1, r1, #6
	mov r0, #0x41
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08014AA4 @ =0x0000080D
	add r3, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	strb r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08014A98: .4byte 0x020192E0
_08014A9C: .4byte 0x020185C0
_08014AA0: .4byte 0x00001ACD
_08014AA4: .4byte 0x0000080D
	thumb_func_end sub_08014A64

