	thumb_func_start sub_080252D4
sub_080252D4: @ 0x080252D4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r6, r0, #0
	mov r5, #0
	ldr r7, _0802533C @ =0x08081FA4
	mov r4, #0
_080252E0:
	lsl r0, r5, #3
	add r3, r6, r0
	ldrb r1, [r3]
	lsl r0, r1, #0x1F
	cmp r0, #0
	beq _08025328
	lsr r0, r1, #3
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r0, [r0]
	mov r2, #0x80
	lsl r2, r2, #2
	add r1, r0, r2
	ldrb r2, [r3, #4]
	ldrb r3, [r3, #5]
	mov r0, #0x10
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xC]
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1C]
	ldr r0, _08025340 @ =0x02015280
	str r0, [sp, #0x20]
	mov r0, #1
	bl sub_0807B6B8
	ldr r1, [r0]
	mov r2, #0x80
	lsl r2, r2, #3
	orr r1, r2
	str r1, [r0]
_08025328:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #0x1F
	bls _080252E0
	add sp, #0x24
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802533C: .4byte gUnk_08081FA4
_08025340: .4byte 0x02015280
	thumb_func_end sub_080252D4

