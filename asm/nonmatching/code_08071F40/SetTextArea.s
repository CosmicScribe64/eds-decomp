	thumb_func_start SetTextArea
SetTextArea: @ 0x0807289C
	push {r4, lr}
	ldr r2, _080728B4 @ =0x03000040
	ldr r4, _080728B8 @ =0x0000441C
	add r3, r2, r4
	strh r0, [r3]
	ldr r0, _080728BC @ =0x0000441E
	add r2, r2, r0
	strh r1, [r2]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080728B4: .4byte 0x03000040
_080728B8: .4byte 0x0000441C
_080728BC: .4byte 0x0000441E
	thumb_func_end SetTextArea

