	thumb_func_start TextPlotRow8
TextPlotRow8: @ 0x08074B74
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r8, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov ip, r0
	mov r0, #7
	add r3, r1, #0
	and r3, r0
	add r4, r2, #0
	and r4, r0
	str r4, [sp, #4]
	asr r4, r1, #3
	asr r2, r2, #3
	str r2, [sp, #0]
	mov r2, #0
	mov r5, #0x80
	lsl r5, r5, #9
	ldr r6, _08074BF4 @ =0x02000000
	add r5, r5, r6
	mov sl, r5
	mov r7, #0x80
	mov r9, r7
_08074BAA:
	mov r0, r9
	asr r0, r2
	mov r1, ip
	and r0, r1
	cmp r0, #0
	beq _08074BD4
	ldr r5, [sp, #4]
	lsl r1, r5, #3
	add r1, r3, r1
	mov r6, sl
	ldrb r6, [r6]
	ldr r7, [sp, #0]
	add r0, r6, #0
	mul r0, r7
	add r0, r4, r0
	lsl r0, r0, #6
	add r1, r1, r0
	ldr r0, _08074BF4 @ =0x02000000
	add r1, r1, r0
	mov r5, r8
	strb r5, [r1]
_08074BD4:
	add r3, #1
	cmp r3, #7
	ble _08074BDE
	mov r3, #0
	add r4, #1
_08074BDE:
	add r2, #1
	cmp r2, #7
	ble _08074BAA
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08074BF4: .4byte 0x02000000
	thumb_func_end TextPlotRow8

