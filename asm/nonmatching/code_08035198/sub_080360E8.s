	thumb_func_start sub_080360E8
sub_080360E8: @ 0x080360E8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	ldrb r7, [r4, #0xC]
	ldrh r0, [r4, #0xC]
	lsr r6, r0, #8
	ldrb r1, [r4, #0xE]
	mov r8, r1
	ldrh r0, [r4, #0xE]
	lsr r5, r0, #8
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08036142
	mov r0, #7
	ldrb r1, [r4, #0xA]
	and r0, r1
	cmp r0, #2
	bne _08036142
	ldrh r1, [r4, #0xC]
	add r0, r4, #0
	bl sub_0802C080
	cmp r0, #0
	beq _08036142
	add r0, r7, #0
	add r1, r6, #0
	mov r2, r8
	add r3, r5, #0
	bl sub_0800CCCC
	cmp r0, #0
	beq _08036142
	add r0, r7, #0
	add r1, r6, #0
	bl sub_0800CD68
	ldrh r1, [r4, #0xE]
	cmp r0, r1
	beq _08036142
	ldrh r0, [r4, #0xC]
	bl sub_08017C0C
_08036142:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080360E8
	.align 2, 0

