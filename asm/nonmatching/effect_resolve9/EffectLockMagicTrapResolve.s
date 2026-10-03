	thumb_func_start EffectLockMagicTrapResolve
EffectLockMagicTrapResolve: @ 0x0803A328
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803A36C
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r1, #0x4A
	cmp r0, #0
	beq _0803A346
	ldr r1, _0803A374 @ =0x0000804A
_0803A346:
	add r0, r1, #0
	mov r1, #2
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x4A
	cmp r0, #0
	bne _0803A360
	ldr r1, _0803A374 @ =0x0000804A
_0803A360:
	add r0, r1, #0
	mov r1, #2
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0803A36C:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
_0803A374: .4byte 0x0000804A
	thumb_func_end EffectLockMagicTrapResolve

