	thumb_func_start sub_0807581C
sub_0807581C: @ 0x0807581C
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	ldr r1, _08075848 @ =0x03000040
	ldr r0, _0807584C @ =0x00004832
	add r4, r1, r0
	ldrb r2, [r4]
	lsl r3, r2, #0x1A
	lsr r0, r3, #0x1A
	add r5, r1, #0
	cmp r0, #2
	bls _08075850
	sub r0, #2
	mov r1, #0x3F
	and r0, r1
	mov r1, #0x40
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _08075858
	.align 2, 0
_08075848: .4byte 0x03000040
_0807584C: .4byte 0x00004832
_08075850:
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	strb r0, [r4]
_08075858:
	ldr r1, _08075870 @ =0x00004832
	add r0, r5, r1
	ldrb r1, [r0]
	mov r0, #0x3F
	and r0, r1
	cmp r0, #0
	bne _08075874
	bl sub_080757F4
	mov r0, #1
	b _08075882
	.align 2, 0
_08075870: .4byte 0x00004832
_08075874:
	ldr r0, _08075888 @ =0x04000054
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1A
	strh r1, [r0]
	sub r0, #4
	strh r6, [r0]
	mov r0, #0
_08075882:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08075888: .4byte 0x04000054
	thumb_func_end sub_0807581C

