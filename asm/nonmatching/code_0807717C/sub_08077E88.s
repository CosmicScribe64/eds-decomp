	thumb_func_start sub_08077E88
sub_08077E88: @ 0x08077E88
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r0, r1, #0
	add r5, r2, #0
	add r4, r3, #0
	ldr r1, [sp, #0x1C]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl sub_0807A320
	ldrh r1, [r6]
	mov r2, #0xFF
	lsl r2, r2, #8
	and r2, r1
	add r1, r1, r4
	mov r3, #0xFF
	and r1, r3
	orr r2, r1
	strh r2, [r0]
	ldrh r1, [r6, #2]
	mov r2, #0xFE
	lsl r2, r2, #8
	and r2, r1
	add r1, r1, r5
	ldr r4, _08077ED0 @ =0x000001FF
	add r3, r4, #0
	and r1, r3
	orr r2, r1
	strh r2, [r0, #2]
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08077ED0: .4byte 0x000001FF
	thumb_func_end sub_08077E88

