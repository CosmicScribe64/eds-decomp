	thumb_func_start sub_0805F64C
sub_0805F64C: @ 0x0805F64C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	add r6, r1, #0
	mov r9, r2
	add r7, r3, #0
	mov r0, #0x20
	mov r1, #2
	bl sub_08074B08
	mov r0, r8
	asr r0, r0, #0x1F
	str r0, [sp, #0]
	mov r1, r8
	sub r4, r1, r0
	asr r4, r4, #1
	mov r1, #9
	sub r1, r1, r4
	mov r0, r8
	lsl r0, r0, #0x18
	mov sl, r0
	lsr r5, r0, #0x10
	mov r0, #0xD
	add r2, r5, #0
	orr r2, r0
	mov r0, #3
	add r3, r6, #0
	bl sub_0807501C
	mov r1, #8
	sub r1, r1, r4
	mov r0, #5
	orr r5, r0
	mov r0, #2
	add r2, r5, #0
	add r3, r6, #0
	bl sub_0807501C
	cmp r7, #0
	ble _0805F708
	mov r2, r9
	ldr r5, [sp, #0]
	mov r6, sl
	cmp r2, #9
	ble _0805F6D0
	ldr r1, _0805F720 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_0805F6BA:
	add r7, #1
	cmp r4, #0
	beq _0805F6C2
	add r7, #1
_0805F6C2:
	add r0, r2, #0
	mov r1, #0xA
	bl __divsi3
	add r2, r0, #0
	cmp r2, #9
	bgt _0805F6BA
_0805F6D0:
	mov r4, r8
	mul r4, r7
	lsr r0, r4, #0x1F
	add r4, r4, r0
	asr r4, r4, #1
	add r0, r4, #3
	mov r1, r8
	sub r5, r1, r5
	asr r5, r5, #1
	mov r1, #9
	sub r1, r1, r5
	lsr r6, r6, #0x10
	mov r3, #8
	add r2, r6, #0
	orr r2, r3
	mov r3, r9
	bl sub_080750E0
	add r4, #2
	mov r1, #8
	sub r1, r1, r5
	mov r0, #7
	orr r6, r0
	add r0, r4, #0
	add r2, r6, #0
	mov r3, r9
	bl sub_080750E0
_0805F708:
	ldr r0, _0805F724 @ =0x0201CFB8
	mov r1, #9
	bl sub_08075114
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0805F720: .4byte 0x02011C20
_0805F724: .4byte 0x0201CFB8
	thumb_func_end sub_0805F64C

