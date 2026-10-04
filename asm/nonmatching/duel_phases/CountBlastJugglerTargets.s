	thumb_func_start CountBlastJugglerTargets
CountBlastJugglerTargets: @ 0x0804F6A8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r8, r1
	mov r6, #0
	mov r5, #0
_0804F6B8:
	mov r4, #0
	add r7, r5, #1
_0804F6BC:
	cmp r5, r9
	bne _0804F6C4
	cmp r4, r8
	beq _0804F6D2
_0804F6C4:
	add r0, r5, #0
	add r1, r4, #0
	bl EffectBlastJugglerCheck
	cmp r0, #0
	beq _0804F6D2
	add r6, #1
_0804F6D2:
	add r4, #1
	cmp r4, #4
	ble _0804F6BC
	add r5, r7, #0
	cmp r5, #1
	ble _0804F6B8
	add r0, r6, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CountBlastJugglerTargets

