	thumb_func_start sub_0800A480
sub_0800A480: @ 0x0800A480
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	str r0, [sp, #0x14]
	mov r8, r1
	mov r0, #0
	mov r9, r0
	mov r2, #1
	ldr r1, [sp, #0x14]
	and r2, r1
	mov r0, #0x94
	mov r7, r8
	mul r7, r0
	add r0, r7, #0
	ldr r1, _0800A544 @ =0x00000D64
	mov ip, r1
	mov r1, ip
	mul r1, r2
	add r0, r0, r1
	ldr r7, _0800A548 @ =0x0201930C
	add r0, r0, r7
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r9, r0
	blt _0800A4BA
	b _0800A646
_0800A4BA:
	mov sl, r2
	mov r4, sp
_0800A4BE:
	mov r2, #0x94
	mov r1, r8
	mul r1, r2
	mov r2, ip
	mov r0, sl
	mul r0, r2
	add r1, r1, r0
	add r1, r1, r7
	mov r0, r9
	lsl r2, r0, #1
	add r0, r1, #0
	add r0, #0xA
	add r0, r0, r2
	ldrh r3, [r0]
	add r1, #0x4A
	add r1, r1, r2
	ldrb r1, [r1]
	cmp r1, #1
	beq _0800A4E6
	b _0800A624
_0800A4E6:
	lsl r0, r3, #0x18
	lsr r6, r0, #0x18
	lsr r5, r3, #8
	and r1, r6
	mov r2, #0x94
	add r0, r5, #0
	mul r0, r2
	mov r2, ip
	mul r2, r1
	add r1, r2, #0
	add r0, r0, r1
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldr r7, _0800A54C @ =0x000007FF
	add r1, r7, #0
	add r0, r3, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0800A550 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0800A554 @ =0x0000028D
	cmp r1, r0
	beq _0800A5DC
	cmp r1, r0
	bgt _0800A57C
	mov r0, #0x9E
	lsl r0, r0, #1
	cmp r1, r0
	bgt _0800A560
	sub r0, #2
	cmp r1, r0
	bge _0800A5DC
	sub r0, #5
	cmp r1, r0
	bgt _0800A558
	sub r0, #5
	cmp r1, r0
	bge _0800A5DC
	sub r0, #2
	cmp r1, r0
	bgt _0800A624
	sub r0, #2
	b _0800A5A8
	.align 2, 0
_0800A544: .4byte 0x00000D64
_0800A548: .4byte 0x0201930C
_0800A54C: .4byte 0x000007FF
_0800A550: .4byte gUnk_08622AB4
_0800A554: .4byte 0x0000028D
_0800A558:
	ldr r0, _0800A55C @ =0x00000137
	b _0800A5C6
_0800A55C: .4byte 0x00000137
_0800A560:
	ldr r0, _0800A570 @ =0x00000147
	cmp r1, r0
	bgt _0800A574
	sub r0, #6
	cmp r1, r0
	bge _0800A5DC
	sub r0, #3
	b _0800A5C6
_0800A570: .4byte 0x00000147
_0800A574:
	ldr r0, _0800A578 @ =0x0000028B
	b _0800A5C6
_0800A578: .4byte 0x0000028B
_0800A57C:
	ldr r0, _0800A598 @ =0x00000417
	cmp r1, r0
	bgt _0800A5B4
	sub r0, #1
	cmp r1, r0
	bge _0800A5DC
	sub r0, #0x54
	cmp r1, r0
	beq _0800A5DC
	cmp r1, r0
	bgt _0800A5A0
	ldr r0, _0800A59C @ =0x0000029B
	b _0800A5C6
	.align 2, 0
_0800A598: .4byte 0x00000417
_0800A59C: .4byte 0x0000029B
_0800A5A0:
	ldr r0, _0800A5B0 @ =0x000003F5
	cmp r1, r0
	bgt _0800A624
	sub r0, #1
_0800A5A8:
	cmp r1, r0
	blt _0800A624
	b _0800A5DC
	.align 2, 0
_0800A5B0: .4byte 0x000003F5
_0800A5B4:
	ldr r0, _0800A5CC @ =0x0000058E
	cmp r1, r0
	beq _0800A5DC
	cmp r1, r0
	bgt _0800A5D0
	sub r0, #0xF0
	cmp r1, r0
	beq _0800A5DC
	add r0, #0x84
_0800A5C6:
	cmp r1, r0
	beq _0800A5DC
	b _0800A624
_0800A5CC: .4byte 0x0000058E
_0800A5D0:
	ldr r0, _0800A658 @ =0x00000604
	cmp r1, r0
	beq _0800A5DC
	add r0, #0xA
	cmp r1, r0
	bne _0800A624
_0800A5DC:
	add r2, r6, #0
	mov r7, #1
	and r2, r7
	ldrb r0, [r4, #2]
	sub r7, #3
	add r1, r7, #0
	and r0, r1
	orr r0, r2
	strb r0, [r4, #2]
	mov r2, #0x3F
	and r2, r5
	lsl r2, r2, #4
	ldrh r0, [r4, #2]
	ldr r7, _0800A65C @ =0xFFFFFC0F
	add r1, r7, #0
	and r0, r1
	orr r0, r2
	strh r0, [r4, #2]
	strh r3, [r4]
	ldr r0, [sp, #0x14]
	lsl r1, r0, #0x18
	mov r2, r8
	lsl r0, r2, #0x18
	lsr r1, r1, #8
	orr r1, r0
	mov r0, sp
	lsr r1, r1, #0x10
	bl sub_0802B558
	cmp r0, #0
	bne _0800A624
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #1
	bl sub_08018544
_0800A624:
	mov r7, #1
	add r9, r7
	mov r1, #0x94
	mov r0, r8
	mul r0, r1
	ldr r2, _0800A660 @ =0x00000D64
	mov ip, r2
	mov r1, sl
	mul r1, r2
	add r0, r0, r1
	ldr r7, _0800A664 @ =0x0201930C
	add r0, r0, r7
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r9, r0
	bge _0800A646
	b _0800A4BE
_0800A646:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800A658: .4byte 0x00000604
_0800A65C: .4byte 0xFFFFFC0F
_0800A660: .4byte 0x00000D64
_0800A664: .4byte 0x0201930C
	thumb_func_end sub_0800A480

