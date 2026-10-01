	thumb_func_start sub_0807B628
sub_0807B628: @ 0x0807B628
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r1, #0
	mov r8, r2
	mov r9, r3
	ldr r5, [sp, #0x24]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov sl, r0
	ldr r0, [r5, #8]
	mov r1, #0
	ldsh r0, [r0, r1]
	sub r6, r6, r3
	add r1, r6, #0
	bl sub_0807B4E0
	add r4, r0, #0
	ldr r0, [r5, #0xC]
	mov r2, #0
	ldsh r0, [r0, r2]
	mov r1, r8
	ldr r2, [sp, #0x20]
	sub r1, r1, r2
	mov r8, r1
	bl sub_0807B4E0
	add r4, r4, r0
	mov r0, r9
	add r7, r4, r0
	ldr r0, [r5, #0x10]
	mov r1, #0
	ldsh r0, [r0, r1]
	add r1, r6, #0
	bl sub_0807B4E0
	add r4, r0, #0
	ldr r0, [r5, #0x14]
	mov r2, #0
	ldsh r0, [r0, r2]
	mov r1, r8
	bl sub_0807B4E0
	add r4, r4, r0
	ldr r0, [sp, #0x20]
	add r4, r4, r0
	mov r1, sl
	cmp r1, #2
	beq _0807B694
	cmp r1, #3
	beq _0807B69C
	b _0807B6A4
_0807B694:
	ldr r0, _0807B698 @ =0x04000028
	b _0807B69E
_0807B698: .4byte 0x04000028
_0807B69C:
	ldr r0, _0807B6B4 @ =0x04000038
_0807B69E:
	str r7, [r0]
	add r0, #4
	str r4, [r0]
_0807B6A4:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807B6B4: .4byte 0x04000038
	thumb_func_end sub_0807B628

