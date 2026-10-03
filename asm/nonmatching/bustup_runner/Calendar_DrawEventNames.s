	thumb_func_start Calendar_DrawEventNames
Calendar_DrawEventNames: @ 0x08001FB0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	mov r8, r0
	mov r0, #0x20
	mov r9, r0
	mov r3, #0x80
	mov r7, #0
	mov r6, #0
	ldr r0, _08002004 @ =0x081980D4
	add r5, r0, #4
	add r4, r0, #0
_08001FCC:
	ldr r0, [r4]
	mov r1, r8
	and r0, r1
	cmp r0, #0
	beq _08001FEC
	mov r0, r9
	add r1, r3, #0
	add r2, r5, #0
	str r3, [sp, #0]
	bl Calendar_DrawStringShadow
	ldr r3, [sp, #0]
	add r3, #0xC
	add r7, #1
	cmp r7, #2
	beq _08001FF6
_08001FEC:
	add r5, #0x44
	add r4, #0x44
	add r6, #1
	cmp r6, #8
	bls _08001FCC
_08001FF6:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08002004: .4byte gCalendarEvents
	thumb_func_end Calendar_DrawEventNames

