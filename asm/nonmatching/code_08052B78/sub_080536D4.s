	thumb_func_start sub_080536D4
sub_080536D4: @ 0x080536D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	str r0, [sp, #0]
	mov r4, #0
	ldr r2, _08053738 @ =0x02017F84
	lsl r1, r0, #4
	mov r0, #0x60
	sub r0, r0, r1
	lsl r0, r0, #0x10
	mov r8, r0
	mov r7, #0x24
	add r5, r2, #0
	ldr r0, _0805373C @ =0x081A4424
	mov sl, r0
	ldr r2, _08053740 @ =0x0300489E
	mov r9, r2
_080536FC:
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl sub_08062140
	mov r3, #0x80
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r6, [sp, #0]
	cmp r6, #0
	beq _0805371A
	mov r2, #0x40
_0805371A:
	add r1, r7, #0
	mov r0, r8
	orr r1, r0
	ldr r3, _08053744 @ =0x02017F7C
	ldrb r3, [r3]
	cmp r4, r3
	bne _08053748
	mov r0, #0x1E
	mov r6, r9
	ldrh r6, [r6]
	and r0, r6
	add r0, sl
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	b _0805374C
_08053738: .4byte 0x02017F84
_0805373C: .4byte gUnk_081A4424
_08053740: .4byte 0x0300489E
_08053744: .4byte 0x02017F7C
_08053748:
	mov r3, #0x80
	lsl r3, r3, #0x11
_0805374C:
	add r0, r1, #0
	mov r1, #0x80
	bl sub_08076714
	add r7, #0x20
	add r5, #4
	add r4, #1
	cmp r4, #4
	ble _080536FC
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_080536D4
	.align 2, 0

