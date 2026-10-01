	thumb_func_start sub_08064F00
sub_08064F00: @ 0x08064F00
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	add r5, r0, #0
	add r4, r3, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r9, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r8, r2
	add r1, sp, #4
	mov r0, #0
	strh r0, [r1]
	ldr r0, _08064F40 @ =0x040000D4
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08064F44 @ =0x81000200
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	mov r0, #0x80
	lsl r0, r0, #3
	add r5, r5, r0
	mov r7, #0x20
	mov r6, #0x20
_08064F36:
	cmp r6, #0x80
	beq _08064F48
	cmp r6, #0xC0
	beq _08064F52
	b _08064F60
_08064F40: .4byte 0x040000D4
_08064F44: .4byte 0x81000200
_08064F48:
	mov r7, #0xA0
	mov r0, #1
	ldrb r1, [r4]
	orr r0, r1
	b _08064F5E
_08064F52:
	mov r7, #0xA0
	mov r1, #2
	neg r1, r1
	add r0, r1, #0
	ldrb r1, [r4]
	and r0, r1
_08064F5E:
	strb r0, [r4]
_08064F60:
	add r1, r7, #0
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	str r4, [sp, #0]
	add r0, r5, #0
	mov r2, r9
	mov r3, r8
	bl sub_080788AC
	add r5, #0x20
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, #0xFF
	bls _08064F36
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08064F00
	.align 2, 0

