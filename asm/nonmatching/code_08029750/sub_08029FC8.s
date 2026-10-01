	thumb_func_start sub_08029FC8
sub_08029FC8: @ 0x08029FC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r5, r0, #0
	mov r8, r1
	add r6, r2, #0
	str r3, [sp, #0]
	ldr r1, _0802A038 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _0802A03C
	ldrb r0, [r6]
	cmp r0, #0
	beq _0802A08A
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	mov r9, r0
	mov r1, #9
	orr r0, r1
	lsl r0, r0, #0x10
	mov sl, r0
_08029FFC:
	ldrh r4, [r6]
	lsr r0, r4, #8
	lsl r4, r4, #0x18
	lsr r4, r4, #0x10
	orr r4, r0
	add r1, r5, #1
	add r0, r4, #0
	mov r2, r8
	add r2, #1
	mov r7, sl
	lsr r3, r7, #0x10
	bl sub_08074C80
	mov r0, #7
	mov r3, r9
	orr r3, r0
	lsl r3, r3, #0x10
	add r0, r4, #0
	add r1, r5, #0
	mov r2, r8
	lsr r3, r3, #0x10
	bl sub_08074C80
	ldr r0, [sp, #0]
	add r5, r5, r0
	add r6, #2
	ldrb r0, [r6]
	cmp r0, #0
	bne _08029FFC
	b _0802A08A
_0802A038: .4byte 0x02011C20
_0802A03C:
	add r4, r6, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _0802A08A
	ldr r1, [sp, #0]
	lsl r0, r1, #0x18
	lsr r0, r0, #0x10
	mov sl, r0
	mov r1, #9
	orr r0, r1
	lsl r0, r0, #0x10
	mov r9, r0
_0802A054:
	ldrb r0, [r4]
	add r1, r5, #1
	mov r2, r8
	add r2, #1
	mov r6, r9
	lsr r3, r6, #0x10
	bl sub_08074D48
	ldrb r0, [r4]
	mov r1, #7
	mov r3, sl
	orr r3, r1
	lsl r3, r3, #0x10
	add r1, r5, #0
	mov r2, r8
	lsr r3, r3, #0x10
	bl sub_08074D48
	ldr r7, [sp, #0]
	lsr r0, r7, #0x1F
	add r0, r7, r0
	asr r0, r0, #1
	add r5, r5, r0
	add r4, #1
	ldrb r0, [r4]
	cmp r0, #0
	bne _0802A054
_0802A08A:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08029FC8
	.align 2, 0

