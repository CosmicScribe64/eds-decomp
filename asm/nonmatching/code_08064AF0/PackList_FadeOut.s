	thumb_func_start PackList_FadeOut
PackList_FadeOut: @ 0x08064DF8
	push {lr}
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08064E10
	mov r0, #0
	bl PackList_DebugNop
	mov r0, #0
	b _08064E1E
_08064E10:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08064E24 @ =0x0000EEFF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08064E1E:
	pop {r1}
	bx r1
	.align 2, 0
_08064E24: .4byte 0x0000EEFF
	thumb_func_end PackList_FadeOut

