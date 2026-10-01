	thumb_func_start sub_08074D48
sub_08074D48: @ 0x08074D48
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r1
	add r5, r2, #0
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	lsl r3, r3, #0x10
	lsr r2, r3, #0x18
	lsl r3, r3, #8
	lsr r3, r3, #0x18
	mov r8, r3
	cmp r2, #0xA
	beq _08074DB4
	cmp r2, #0xA
	bgt _08074D70
	cmp r2, #8
	beq _08074D7A
	b _08074E0E
_08074D70:
	cmp r2, #0xC
	beq _08074DC4
	cmp r2, #0x10
	beq _08074DD4
	b _08074E0E
_08074D7A:
	lsl r0, r1, #3
	ldr r1, _08074DB0 @ =0x08228D00
	add r6, r0, r1
	mov r7, #3
_08074D82:
	ldrh r4, [r6]
	add r6, #2
	lsl r4, r4, #0x11
	lsl r0, r4, #8
	lsr r0, r0, #0x18
	add r2, r5, #0
	add r5, #1
	mov r1, r9
	mov r3, r8
	bl sub_08074B74
	lsr r4, r4, #0x18
	add r2, r5, #0
	add r5, #1
	add r0, r4, #0
	mov r1, r9
	mov r3, r8
	bl sub_08074B74
	sub r7, #1
	cmp r7, #0
	bge _08074D82
	b _08074E0E
_08074DB0: .4byte gUnk_08228D00
_08074DB4:
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
	ldr r1, _08074DC0 @ =0x08229500
	b _08074DD8
	.align 2, 0
_08074DC0: .4byte gUnk_08229500
_08074DC4:
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	ldr r1, _08074DD0 @ =0x08229F00
	b _08074DD8
	.align 2, 0
_08074DD0: .4byte gUnk_08229F00
_08074DD4:
	lsl r0, r1, #4
	ldr r1, _08074E1C @ =0x0822AB00
_08074DD8:
	add r6, r0, r1
	lsr r3, r2, #1
	cmp r3, #0
	beq _08074E0E
	add r7, r3, #0
_08074DE2:
	ldrh r4, [r6]
	add r6, #2
	lsl r4, r4, #0x11
	lsl r0, r4, #8
	lsr r0, r0, #0x18
	add r2, r5, #0
	add r5, #1
	mov r1, r9
	mov r3, r8
	bl sub_08074B74
	lsr r4, r4, #0x18
	add r2, r5, #0
	add r5, #1
	add r0, r4, #0
	mov r1, r9
	mov r3, r8
	bl sub_08074B74
	sub r7, #1
	cmp r7, #0
	bne _08074DE2
_08074E0E:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08074E1C: .4byte gUnk_0822AB00
	thumb_func_end sub_08074D48

