	thumb_func_start EffectSolomonsLawbookResolve
EffectSolomonsLawbookResolve: @ 0x08036990
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _080369B8
	mov r0, #1
	ldrb r1, [r1, #2]
	and r0, r1
	mov r1, #0x45
	cmp r0, #0
	beq _080369AC
	ldr r1, _080369C0 @ =0x00008045
_080369AC:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080369B8:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_080369C0: .4byte 0x00008045
	thumb_func_end EffectSolomonsLawbookResolve

