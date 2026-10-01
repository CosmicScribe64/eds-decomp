	thumb_func_start sub_08064FF8
sub_08064FF8: @ 0x08064FF8
	push {lr}
	add r1, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r0, _0806502C @ =0x0201DB20
	ldr r2, _08065030 @ =0x00001C34
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1C
	add r0, r0, r1
	mov r1, #7
	bl __modsi3
	add r1, r0, #0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	mov r1, #0xC3
	lsl r1, r1, #2
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
_0806502C: .4byte 0x0201DB20
_08065030: .4byte 0x00001C34
	thumb_func_end sub_08064FF8

