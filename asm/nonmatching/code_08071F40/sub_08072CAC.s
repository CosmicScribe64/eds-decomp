	thumb_func_start sub_08072CAC
sub_08072CAC: @ 0x08072CAC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	add r4, r0, #0
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsr r3, r1, #0x10
	sub r0, r3, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #7
	bhi _08072D1C
	mov r0, sp
	add r1, r0, r3
	mov r0, #0
	strb r0, [r1]
	cmp r2, #0
	bge _08072CD6
	neg r2, r2
_08072CD6:
	lsl r5, r4, #0x10
	lsr r4, r4, #0x10
	ldr r1, _08072D00 @ =0x0808765C
	mov ip, r1
	mov r7, #0xF
	mov r6, #0x30
_08072CE2:
	sub r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r2, #0
	beq _08072D04
	mov r1, sp
	add r0, r1, r3
	add r1, r2, #0
	and r1, r7
	lsl r1, r1, #2
	add r1, ip
	ldr r1, [r1]
	strb r1, [r0]
	b _08072D0A
	.align 2, 0
_08072D00: .4byte gUnk_0808765C
_08072D04:
	mov r1, sp
	add r0, r1, r3
	strb r6, [r0]
_08072D0A:
	asr r2, r2, #4
	cmp r3, #0
	bne _08072CE2
	lsr r0, r5, #0x10
	add r1, r4, #0
	mov r2, r8
	mov r3, sp
	bl sub_08072BB4
_08072D1C:
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08072CAC

