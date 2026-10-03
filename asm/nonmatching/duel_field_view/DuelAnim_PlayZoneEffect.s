	thumb_func_start DuelAnim_PlayZoneEffect
DuelAnim_PlayZoneEffect: @ 0x08024380
	push {r4, r5, r6, r7, lr}
	ldr r4, _080243C0 @ =0x0201CFB0
	mov r5, #0x83
	lsl r5, r5, #4
	add r6, r4, r5
	ldr r7, _080243C4 @ =0x00000834
	add r5, r4, r7
	str r1, [r5]
	mov r5, #0x84
	lsl r5, r5, #4
	add r1, r4, r5
	ldr r0, [r0]
	str r0, [r1]
	add r7, #6
	add r0, r4, r7
	mov r1, #0
	strh r2, [r0]
	ldr r2, _080243C8 @ =0x0000083C
	add r0, r4, r2
	strh r3, [r0]
	sub r5, #8
	add r0, r4, r5
	strb r1, [r0]
	sub r7, #1
	add r4, r4, r7
	strb r1, [r4]
	mov r0, #0xB
	strb r0, [r6]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080243C0: .4byte 0x0201CFB0
_080243C4: .4byte 0x00000834
_080243C8: .4byte 0x0000083C
	thumb_func_end DuelAnim_PlayZoneEffect

