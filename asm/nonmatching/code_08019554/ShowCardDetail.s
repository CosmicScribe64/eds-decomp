	thumb_func_start ShowCardDetail
ShowCardDetail: @ 0x08019788
	push {r4, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, #0x70
	cmp r4, #0
	beq _08019798
	ldr r0, _080197B8 @ =0x00008070
_08019798:
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x12
	cmp r4, #0
	beq _080197A8
	ldr r0, _080197BC @ =0x00008012
_080197A8:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	pop {r4}
	pop {r0}
	bx r0
_080197B8: .4byte 0x00008070
_080197BC: .4byte 0x00008012
	thumb_func_end ShowCardDetail

