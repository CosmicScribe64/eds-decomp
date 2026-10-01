	thumb_func_start sub_0807AC88
sub_0807AC88: @ 0x0807AC88
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r4, [sp, #0x1C]
	ldr r5, [sp, #0x20]
	ldr r6, [sp, #0x28]
	lsl r0, r0, #0x18
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r5, r5, #0x18
	lsr r1, r5, #0x18
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	lsr r3, r3, #0x13
	add r2, r2, r3
	lsl r2, r2, #1
	lsr r0, r0, #0xD
	add r2, r2, r0
	mov r0, #0xC0
	lsl r0, r0, #0x13
	add r5, r2, r0
	mov r7, #0xA
	cmp r6, #0
	beq _0807ACCC
	cmp r6, #1
	beq _0807ACFC
	b _0807AD32
_0807ACCC:
	mov r6, #0
	lsl r1, r1, #0xC
	mov r8, r1
_0807ACD2:
	add r0, r4, #0
	add r1, r7, #0
	bl __umodsi3
	add r0, r9
	mov r1, r8
	orr r0, r1
	strh r0, [r5]
	sub r5, #2
	add r0, r4, #0
	add r1, r7, #0
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #2
	bls _0807ACD2
	b _0807AD32
_0807ACFC:
	mov r6, #0
	lsl r1, r1, #0xC
	mov r8, r1
_0807AD02:
	add r0, r4, #0
	add r1, r7, #0
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0
	beq _0807AD1C
	add r0, r9
	mov r1, r8
	orr r0, r1
	strh r0, [r5]
	sub r5, #2
_0807AD1C:
	add r0, r4, #0
	add r1, r7, #0
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #2
	bls _0807AD02
_0807AD32:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807AC88
	.align 2, 0

