	thumb_func_start TurnOrder_FadeOutAndSetFirstPlayer
TurnOrder_FadeOutAndSetFirstPlayer: @ 0x08029750
	push {lr}
	ldr r3, _0802977C @ =0x02020310
	mov r0, #0xAD
	lsl r0, r0, #4
	add r1, r3, r0
	ldr r2, _08029780 @ =0x00000AD2
	add r0, r3, r2
	ldrh r2, [r1]
	ldrh r0, [r0]
	add r0, r2, r0
	strh r0, [r1]
	lsl r1, r0, #0x10
	mov r0, #0x80
	lsl r0, r0, #0x15
	cmp r1, r0
	bhi _08029784
	lsr r0, r1, #0x18
	bl SetBldY
	mov r0, #0
	b _080297A2
	.align 2, 0
_0802977C: .4byte 0x02020310
_08029780: .4byte 0x00000AD2
_08029784:
	ldr r2, _080297A8 @ =0x03000040
	ldr r1, _080297AC @ =0x00000ABF
	add r0, r3, r1
	ldr r3, _080297B0 @ =0x00004870
	add r2, r2, r3
	mov r1, #1
	ldrb r0, [r0]
	and r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	mov r0, #1
_080297A2:
	pop {r1}
	bx r1
	.align 2, 0
_080297A8: .4byte 0x03000040
_080297AC: .4byte 0x00000ABF
_080297B0: .4byte 0x00004870
	thumb_func_end TurnOrder_FadeOutAndSetFirstPlayer

