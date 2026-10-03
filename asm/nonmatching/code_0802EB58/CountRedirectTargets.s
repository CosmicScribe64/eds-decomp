	thumb_func_start CountRedirectTargets
CountRedirectTargets: @ 0x0802F808
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r1, #0
	mov r9, r2
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r0, #0
	mov r8, r0
	ldr r0, _0802F82C @ =0x00000431
	cmp r6, r0
	beq _0802F830
	add r0, #0xF4
	cmp r6, r0
	beq _0802F85A
	b _0802F878
	.align 2, 0
_0802F82C: .4byte 0x00000431
_0802F830:
	mov r5, #0
_0802F832:
	mov r4, #0
	add r7, r5, #1
_0802F836:
	add r0, r6, #0
	mov r1, r9
	add r2, r5, #0
	add r3, r4, #0
	bl CanRedirectEffectToZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802F84C
	mov r0, #1
	add r8, r0
_0802F84C:
	add r4, #1
	cmp r4, #4
	ble _0802F836
	add r5, r7, #0
	cmp r5, #1
	ble _0802F832
	b _0802F878
_0802F85A:
	mov r4, #0
_0802F85C:
	add r0, r6, #0
	mov r1, r9
	add r2, r5, #0
	add r3, r4, #0
	bl CanRedirectEffectToZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802F872
	mov r0, #1
	add r8, r0
_0802F872:
	add r4, #1
	cmp r4, #4
	ble _0802F85C
_0802F878:
	mov r0, r8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CountRedirectTargets
	.align 2, 0

