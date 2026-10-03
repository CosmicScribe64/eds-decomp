	thumb_func_start DuelCursor_MoveTo
DuelCursor_MoveTo: @ 0x080240D4
	push {r4, r5, lr}
	ldr r3, _08024118 @ =0x0201CFB0
	ldr r2, _0802411C @ =0x00000814
	add r4, r3, r2
	ldr r5, _08024120 @ =0x0000080C
	add r2, r3, r5
	ldr r2, [r2]
	str r2, [r4]
	ldr r2, _08024124 @ =0x00000818
	add r4, r3, r2
	add r5, #4
	add r2, r3, r5
	ldr r2, [r2]
	str r2, [r4]
	ldr r4, _08024128 @ =0x0000081C
	add r2, r3, r4
	str r0, [r2]
	add r5, #0x10
	add r0, r3, r5
	str r1, [r0]
	ldr r0, _0802412C @ =0x00000808
	add r3, r3, r0
	ldr r0, _08024130 @ =0xFFFFFC3F
	ldrh r1, [r3]
	and r0, r1
	mov r2, #0x80
	lsl r2, r2, #1
	add r1, r2, #0
	orr r0, r1
	strh r0, [r3]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08024118: .4byte 0x0201CFB0
_0802411C: .4byte 0x00000814
_08024120: .4byte 0x0000080C
_08024124: .4byte 0x00000818
_08024128: .4byte 0x0000081C
_0802412C: .4byte 0x00000808
_08024130: .4byte 0xFFFFFC3F
	thumb_func_end DuelCursor_MoveTo

