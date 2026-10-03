	thumb_func_start DuelPrompt_PostRandomDiscard
DuelPrompt_PostRandomDiscard: @ 0x08022784
	push {r4, r5, lr}
	add r5, r0, #0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	ldr r3, _080227A8 @ =0x020192E0
	ldr r0, _080227AC @ =0x00001B50
	add r1, r3, r0
	ldr r0, _080227B0 @ =0x000003F2
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0x32
	bne _080227B8
	ldr r0, _080227B4 @ =0x00001B54
	add r1, r3, r0
	ldrh r3, [r1]
	add r0, r3, r2
	strh r0, [r1]
	b _080227C6
_080227A8: .4byte 0x020192E0
_080227AC: .4byte 0x00001B50
_080227B0: .4byte 0x000003F2
_080227B4: .4byte 0x00001B54
_080227B8:
	lsl r3, r2, #0x10
	lsr r3, r3, #0x10
	add r0, r5, #0
	mov r1, #3
	add r2, r4, #0
	bl DuelPrompt_Post
_080227C6:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end DuelPrompt_PostRandomDiscard

