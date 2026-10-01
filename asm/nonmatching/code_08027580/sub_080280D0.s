	thumb_func_start sub_080280D0
sub_080280D0: @ 0x080280D0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r6, #0x40
	mov r4, #0
	ldr r0, _0802816C @ =0x080826EA
	mov sl, r0
	mov r3, #4
	and r1, r3
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	ldr r0, _08028170 @ =0x080826FE
	add r0, r0, r5
	mov r8, r0
_080280FC:
	lsl r0, r5, #1
	add r0, r0, r4
	lsl r0, r0, #1
	add r0, sl
	ldrh r1, [r0]
	add r0, r4, #0
	mul r0, r6
	mov r7, #0x38
	add r2, r7, r0
	mov r3, #0x6E
	cmp r5, #0
	bne _08028116
	sub r3, #0x42
_08028116:
	str r6, [sp, #0]
	mov r0, #0x20
	str r0, [sp, #4]
	mov r7, #4
	str r7, [sp, #8]
	mov r7, r8
	ldrb r0, [r7]
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08028174 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	bl sub_0807B6B8
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, r9
	cmp r0, #0
	beq _08028150
	mov r0, #0x80
	lsl r0, r0, #3
	orr r1, r0
	str r1, [r2]
_08028150:
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #1
	bls _080280FC
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802816C: .4byte gUnk_080826EA
_08028170: .4byte gUnk_080826FE
_08028174: .4byte 0x02020310
	thumb_func_end sub_080280D0

