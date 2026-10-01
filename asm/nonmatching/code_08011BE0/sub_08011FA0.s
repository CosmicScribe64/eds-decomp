	thumb_func_start sub_08011FA0
sub_08011FA0: @ 0x08011FA0
	push {r4, lr}
	ldr r4, _08011FEC @ =0x020185C0
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	mov r0, #0x94
	ldrh r3, [r4, #2]
	add r1, r3, #0
	mul r1, r0
	ldr r0, _08011FF0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08011FF4 @ =0x0201930C
	add r3, r1, r0
	ldrb r2, [r3, #6]
	lsl r1, r2, #0x1A
	lsr r0, r1, #0x1C
	cmp r0, #0xE
	bhi _08011FD6
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #2
	mov r1, #0x3D
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3, #6]
_08011FD6:
	ldr r0, _08011FF8 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08011FEC: .4byte 0x020185C0
_08011FF0: .4byte 0x00000D64
_08011FF4: .4byte 0x0201930C
_08011FF8: .4byte 0x0000080D
	thumb_func_end sub_08011FA0

