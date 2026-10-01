	thumb_func_start sub_0807BCFC
sub_0807BCFC: @ 0x0807BCFC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r2, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	add r4, r5, #0
	add r6, r5, #4
	add r7, r5, #0
	add r7, #8
	bl sub_0807BED8
	mov r1, #1
	eor r1, r0
	lsl r1, r1, #0x18
	lsr r2, r1, #0x18
	ldrb r0, [r5, #8]
	cmp r0, #5
	bhi _0807BE10
	lsl r0, r0, #2
	ldr r1, _0807BD38 @ =0x0807BD3C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0807BD38: .4byte 0x0807BD3C
_0807BD3C:
	.4byte _0807BD54
	.4byte _0807BD74
	.4byte _0807BD92
	.4byte _0807BDFE
	.4byte _0807BE10
	.4byte _0807BE04
_0807BD54:
	add r0, r4, #0
	add r1, r6, #0
	bl sub_0807BE34
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807BD68
	ldrb r0, [r7]
	add r0, #1
	strb r0, [r7]
_0807BD68:
	mov r1, #0
	mov r0, #0x96
	lsl r0, r0, #1
	strh r0, [r5, #0xA]
	strb r1, [r4, #1]
	b _0807BE10
_0807BD74:
	mov r0, r9
	strb r0, [r4]
	mov r0, r8
	strh r0, [r4, #2]
	mov r0, #1
	strb r0, [r4, #1]
	add r0, r4, #0
	mov r1, #4
	bl sub_08073784
	cmp r0, #0
	beq _0807BE10
	mov r0, #2
	strb r0, [r7]
	b _0807BE10
_0807BD92:
	add r0, r2, #0
	add r1, r6, #0
	mov r2, #4
	bl sub_08073F04
	cmp r0, #0
	beq _0807BE10
	ldrb r0, [r6, #1]
	cmp r0, #3
	beq _0807BDE6
	cmp r0, #3
	bgt _0807BDB0
	cmp r0, #1
	beq _0807BDB6
	b _0807BDF2
_0807BDB0:
	cmp r0, #4
	beq _0807BDD8
	b _0807BDF2
_0807BDB6:
	ldrb r0, [r6]
	cmp r0, r9
	beq _0807BDC0
	mov r0, #0
	b _0807BE20
_0807BDC0:
	mov r0, #4
	strb r0, [r4, #1]
	add r0, r4, #0
	mov r1, #4
	bl sub_08073784
	mov r0, #0x96
	lsl r0, r0, #1
	strh r0, [r5, #0xA]
	mov r0, #5
	strb r0, [r7]
	b _0807BE10
_0807BDD8:
	mov r0, #0x96
	lsl r0, r0, #1
	strh r0, [r5, #0xA]
	mov r0, #5
	strb r0, [r7]
	mov r0, #1
	b _0807BE24
_0807BDE6:
	mov r1, #0
	mov r0, #0x96
	lsl r0, r0, #1
	strh r0, [r5, #0xA]
	strb r1, [r7]
	b _0807BE10
_0807BDF2:
	mov r0, #0
	strb r0, [r7]
	mov r0, #0x96
	lsl r0, r0, #1
	strh r0, [r5, #0xA]
	b _0807BE10
_0807BDFE:
	mov r0, #1
	strb r0, [r7]
	b _0807BE10
_0807BE04:
	add r0, r4, #0
	add r1, r6, #0
	bl sub_0807BE60
	mov r0, #1
	b _0807BE24
_0807BE10:
	ldrh r0, [r5, #0xA]
	sub r0, #1
	strh r0, [r5, #0xA]
	lsl r0, r0, #0x10
	ldr r1, _0807BE30 @ =0xFFFF0000
	cmp r0, r1
	bne _0807BE22
	mov r0, #0
_0807BE20:
	strb r0, [r7]
_0807BE22:
	mov r0, #0
_0807BE24:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0807BE30: .4byte 0xFFFF0000
	thumb_func_end sub_0807BCFC

