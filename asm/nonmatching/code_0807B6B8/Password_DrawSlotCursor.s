	thumb_func_start Password_DrawSlotCursor
Password_DrawSlotCursor: @ 0x0807BF9C
	push {r4, r5, lr}
	add r4, r0, #0
	lsl r4, r4, #3
	add r4, #0x18
	mov r0, #0x80
	lsl r0, r0, #0xD
	orr r4, r0
	ldr r5, _0807BFD4 @ =0x08087E7C
	ldr r0, _0807BFD8 @ =0x03000040
	ldr r1, _0807BFDC @ =0x0000485E
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r0, r0, #3
	mov r1, #6
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, r0, r5
	ldrh r2, [r0]
	add r0, r4, #0
	mov r1, #0
	bl AddSprite
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0807BFD4: .4byte gPasswordSlotCursorFrames
_0807BFD8: .4byte 0x03000040
_0807BFDC: .4byte 0x0000485E
	thumb_func_end Password_DrawSlotCursor

