	thumb_func_start sub_080618C4
sub_080618C4: @ 0x080618C4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r5, r2, #0
	lsl r2, r1, #0x10
	lsr r1, r2, #0x10
	str r1, [sp, #0]
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #4]
	lsr r0, r0, #0x10
	mov sl, r0
	cmp r5, #0
	beq _08061956
	cmp r3, #0
	beq _08061956
	lsr r0, r2, #0x14
	lsl r0, r0, #5
	ldr r1, _08061968 @ =0x05000200
	add r0, r0, r1
	add r1, r3, #0
	mov r2, #0x40
	bl sub_08075294
	mov r0, #0
	mov r8, r0
_080618FE:
	mov r6, #0
	mov r1, #1
	add r1, r8
	mov r9, r1
_08061906:
	ldrh r7, [r5]
	mov r4, #0
	add r5, #2
	add r3, r6, #1
_0806190E:
	lsl r0, r4, #2
	add r2, r7, #0
	asr r2, r0
	mov r0, #0xF
	and r2, r0
	cmp r2, #0
	beq _08061938
	ldr r1, [sp, #4]
	add r0, r1, r4
	lsl r1, r6, #2
	add r0, r0, r1
	ldr r1, [sp, #0]
	add r2, r2, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r1, sl
	add r1, r8
	str r3, [sp, #8]
	bl sub_08061848
	ldr r3, [sp, #8]
_08061938:
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #3
	bls _0806190E
	lsl r0, r3, #0x10
	lsr r6, r0, #0x10
	cmp r6, #1
	bls _08061906
	mov r1, r9
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	cmp r0, #7
	bls _080618FE
_08061956:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08061968: .4byte 0x05000200
	thumb_func_end sub_080618C4

