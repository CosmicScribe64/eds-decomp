	thumb_func_start sub_08054770
sub_08054770: @ 0x08054770
	push {r4, r5, r6, r7, lr}
	ldr r4, _080547D0 @ =0x0201AE60
	ldrh r0, [r4, #0xA]
	lsl r1, r0, #3
	add r6, r1, #0
	add r6, #0x20
	ldrh r1, [r4, #0xE]
	add r0, r1, r0
	add r1, r4, #0
	add r1, #0x21
	ldrb r1, [r1]
	sub r0, r0, r1
	add r0, #2
	lsl r0, r0, #3
	sub r6, r6, r0
	lsl r5, r6, #0x10
	mov r0, #0x40
	orr r5, r0
	ldr r1, _080547D4 @ =0x0201CF90
	ldrb r0, [r1, #3]
	lsr r2, r0, #7
	ldr r0, _080547D8 @ =0x00007FFF
	ldrh r1, [r1, #4]
	and r0, r1
	lsl r0, r0, #1
	orr r0, r2
	bl sub_08062140
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldrh r0, [r4, #0x14]
	cmp r0, #0
	bne _080547E8
	ldr r2, _080547DC @ =0x081A4424
	ldr r1, _080547E0 @ =0x03000040
	ldr r0, _080547E4 @ =0x0000485E
	add r1, r1, r0
	mov r0, #0x1E
	ldrh r1, [r1]
	and r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	b _080547EC
_080547D0: .4byte 0x0201AE60
_080547D4: .4byte 0x0201CF90
_080547D8: .4byte 0x00007FFF
_080547DC: .4byte gUnk_081A4424
_080547E0: .4byte 0x03000040
_080547E4: .4byte 0x0000485E
_080547E8:
	mov r3, #0x80
	lsl r3, r3, #0x11
_080547EC:
	add r0, r5, #0
	mov r1, #0x80
	add r2, r7, #0
	bl sub_08076714
	lsl r4, r6, #0x10
	mov r0, #0x90
	orr r4, r0
	ldr r2, _08054828 @ =0x0201CF90
	ldrb r1, [r2, #1]
	lsl r0, r1, #0x19
	cmp r0, #0
	bge _08054830
	ldrb r0, [r2, #3]
	lsr r1, r0, #7
	ldr r0, _0805482C @ =0x00007FFF
	ldrh r2, [r2, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	bl sub_08062140
	mov r2, #0x80
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	b _08054832
	.align 2, 0
_08054828: .4byte 0x0201CF90
_0805482C: .4byte 0x00007FFF
_08054830:
	mov r5, #0x40
_08054832:
	ldr r0, _08054854 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08054864
	ldr r2, _08054858 @ =0x081A4424
	ldr r1, _0805485C @ =0x03000040
	ldr r0, _08054860 @ =0x0000485E
	add r1, r1, r0
	mov r0, #0x1E
	ldrh r1, [r1]
	and r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	mov r0, #0x20
	orr r3, r0
	b _08054866
_08054854: .4byte 0x0201AE60
_08054858: .4byte gUnk_081A4424
_0805485C: .4byte 0x03000040
_08054860: .4byte 0x0000485E
_08054864:
	ldr r3, _08054878 @ =0x01000020
_08054866:
	add r0, r4, #0
	mov r1, #0x80
	add r2, r5, #0
	bl sub_08076714
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08054878: .4byte 0x01000020
	thumb_func_end sub_08054770

