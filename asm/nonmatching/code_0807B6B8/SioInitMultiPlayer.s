	thumb_func_start SioInitMultiPlayer
SioInitMultiPlayer: @ 0x0807BE7C
	push {r4, r5, lr}
	lsl r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r2, _0807BEC8 @ =0x04000134
	mov r3, #0
	strh r3, [r2]
	ldr r4, _0807BECC @ =0x04000128
	mov r2, #0xC0
	lsl r2, r2, #0x12
	and r2, r0
	lsr r2, r2, #0x18
	mov r0, #1
	and r1, r0
	lsl r1, r1, #0xF
	mov r5, #0x80
	lsl r5, r5, #6
	add r0, r5, #0
	orr r1, r0
	orr r2, r1
	strh r2, [r4]
	ldr r0, _0807BED0 @ =0x0400012A
	strh r3, [r0]
	mov r1, #0
	ldr r3, _0807BED4 @ =0x04000120
	mov r2, #0
_0807BEB0:
	lsl r0, r1, #1
	add r0, r0, r3
	strh r2, [r0]
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	cmp r1, #3
	bls _0807BEB0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0807BEC8: .4byte 0x04000134
_0807BECC: .4byte 0x04000128
_0807BED0: .4byte 0x0400012A
_0807BED4: .4byte 0x04000120
	thumb_func_end SioInitMultiPlayer

