	thumb_func_start sub_08060E2C
sub_08060E2C: @ 0x08060E2C
	push {r4, lr}
	lsl r2, r2, #0x10
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0xB
	add r0, r0, r1
	lsl r0, r0, #1
	ldr r1, _08060EC8 @ =0x03001C5C
	add r3, r0, r1
	lsr r0, r2, #0x10
	mov r1, #0x80
	lsl r1, r1, #9
	add r2, r2, r1
	strh r0, [r3]
	lsr r0, r2, #0x10
	add r2, r2, r1
	strh r0, [r3, #2]
	lsr r0, r2, #0x10
	add r2, r2, r1
	strh r0, [r3, #4]
	lsr r0, r2, #0x10
	add r2, r2, r1
	strh r0, [r3, #6]
	add r1, r3, #0
	add r1, #0x40
	lsr r0, r2, #0x10
	mov r4, #0x80
	lsl r4, r4, #9
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #0x3A
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #0x3A
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	strh r0, [r1]
	add r1, #2
	lsr r0, r2, #0x10
	add r2, r2, r4
	lsr r2, r2, #0x10
	strh r0, [r1]
	add r0, r3, #0
	add r0, #0xC6
	strh r2, [r0]
	pop {r4}
	pop {r0}
	bx r0
_08060EC8: .4byte 0x03001C5C
	thumb_func_end sub_08060E2C

