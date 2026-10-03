	thumb_func_start SubtractLifePoints
SubtractLifePoints: @ 0x08007418
	add r3, r2, #0
	mov r2, #1
	and r2, r1
	ldr r1, _08007430 @ =0x00000D64
	mul r1, r2
	add r1, r0, r1
	ldrh r0, [r1]
	cmp r0, r3
	ble _08007434
	sub r0, r0, r3
	b _08007436
	.align 2, 0
_08007430: .4byte 0x00000D64
_08007434:
	mov r0, #0
_08007436:
	strh r0, [r1]
	bx lr
	thumb_func_end SubtractLifePoints
	.align 2, 0

