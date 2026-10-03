	thumb_func_start EffectShieldAndSwordResolve
EffectShieldAndSwordResolve: @ 0x08033DAC
	push {lr}
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _08033DE0
	mov r1, #1
	add r0, r1, #0
	ldrb r2, [r2, #2]
	and r0, r2
	mov r2, #0x1D
	cmp r0, #0
	beq _08033DCA
	ldr r2, _08033DE8 @ =0x0000801D
_08033DCA:
	ldr r0, _08033DEC @ =0x020192E0
	ldr r3, _08033DF0 @ =0x00001ACD
	add r0, r0, r3
	ldrb r0, [r0]
	lsr r0, r0, #6
	bic r1, r0
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08033DE0:
	mov r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_08033DE8: .4byte 0x0000801D
_08033DEC: .4byte 0x020192E0
_08033DF0: .4byte 0x00001ACD
	thumb_func_end EffectShieldAndSwordResolve

