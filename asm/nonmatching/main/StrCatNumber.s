	thumb_func_start StrCatNumber
StrCatNumber: @ 0x08075370
	push {r4, r5, r6, r7, lr}
	sub sp, #0xC
	add r7, r0, #0
	add r5, r1, #0
	mov r1, #0x30
	mov r0, sp
	add r0, #0xA
_0807537E:
	strb r1, [r0]
	sub r0, #1
	cmp r0, sp
	bge _0807537E
	mov r1, sp
	mov r0, #0
	strb r0, [r1, #0xB]
	mov r6, #0xA
	cmp r5, #0
	ble _080753B6
_08075392:
	mov r0, sp
	add r4, r0, r6
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r0, #0x30
	strb r0, [r4]
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	sub r6, #1
	cmp r5, #0
	ble _080753B6
	cmp r6, #0
	bgt _08075392
_080753B6:
	add r0, r6, #1
	mov r2, sp
	add r1, r2, r0
	add r0, r7, #0
	bl StrCat
	add sp, #0xC
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end StrCatNumber
	.align 2, 0

