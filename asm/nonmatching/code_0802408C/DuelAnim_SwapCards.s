	thumb_func_start DuelAnim_SwapCards
DuelAnim_SwapCards: @ 0x0802432C
	push {r4, r5, r6, r7, lr}
	ldr r4, _08024370 @ =0x0201CFB0
	mov r2, #0x83
	lsl r2, r2, #4
	add r5, r4, r2
	mov r6, #1
	mov r3, #1
	ldrb r7, [r5]
	and r3, r7
	mov r2, #8
	orr r3, r2
	strb r3, [r5]
	mov r7, #0x84
	lsl r7, r7, #4
	add r2, r4, r7
	ldr r0, [r0]
	str r0, [r2]
	ldr r0, _08024374 @ =0x00000844
	add r2, r4, r0
	ldr r0, [r1]
	str r0, [r2]
	ldr r1, _08024378 @ =0x00000838
	add r0, r4, r1
	mov r1, #0
	strb r1, [r0]
	ldr r2, _0802437C @ =0x00000839
	add r4, r4, r2
	strb r1, [r4]
	orr r3, r6
	strb r3, [r5]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08024370: .4byte 0x0201CFB0
_08024374: .4byte 0x00000844
_08024378: .4byte 0x00000838
_0802437C: .4byte 0x00000839
	thumb_func_end DuelAnim_SwapCards

