	thumb_func_start Bustup_ChangeSpeaker
Bustup_ChangeSpeaker: @ 0x08001770
	push {r4, r5, lr}
	sub sp, #4
	mov r5, #1
	ldr r1, _080017CC @ =0xFFFFFE80
	ldr r4, _080017D0 @ =0x020150CC
	mov r0, #0
	mov r2, #0
	add r3, r4, #0
	bl FadeStart
	sub r0, r4, #1
	ldrb r0, [r0]
	bl GetSceneSet
	add r1, r4, #0
	add r1, #0x80
	ldrh r2, [r1]
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #6
	add r1, r1, r2
	lsl r1, r1, #2
	ldr r2, _080017D4 @ =0x0813ADF8
	add r1, r1, r2
	ldr r3, _080017D8 @ =0xFFFFF6C0
	add r2, r4, r3
	ldr r3, _080017DC @ =0xFFFFFDD4
	add r4, r4, r3
	str r5, [sp, #0]
	add r3, r4, #0
	bl Bustup_ChangeSceneSet
	bl Bustup_ResetBlink
	ldr r0, _080017E0 @ =0x03000040
	ldr r1, _080017E4 @ =0x00004859
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #4
	strb r1, [r0]
	mov r0, #0
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_080017CC: .4byte 0xFFFFFE80
_080017D0: .4byte 0x020150CC
_080017D4: .4byte gUnk_0813ADF8
_080017D8: .4byte 0xFFFFF6C0
_080017DC: .4byte 0xFFFFFDD4
_080017E0: .4byte 0x03000040
_080017E4: .4byte 0x00004859
	thumb_func_end Bustup_ChangeSpeaker

