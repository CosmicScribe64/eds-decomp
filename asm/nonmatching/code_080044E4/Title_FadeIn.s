	thumb_func_start Title_FadeIn
Title_FadeIn: @ 0x08005368
	push {r4, r5, r6, lr}
	ldr r0, _08005384 @ =0x03000040
	ldr r1, _08005388 @ =0x00004858
	add r6, r0, r1
	ldrb r2, [r6]
	add r4, r0, #0
	cmp r2, #1
	beq _080053A4
	cmp r2, #1
	bgt _0800538C
	cmp r2, #0
	beq _08005392
	b _08005448
	.align 2, 0
_08005384: .4byte 0x03000040
_08005388: .4byte 0x00004858
_0800538C:
	cmp r2, #2
	beq _080053D4
	b _08005448
_08005392:
	mov r0, #0x80
	lsl r0, r0, #0x13
	mov r2, #0xC0
	lsl r2, r2, #3
	add r1, r2, #0
	strh r1, [r0]
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
_080053A4:
	ldr r3, _080053CC @ =0x0000485E
	add r1, r4, r3
	mov r0, #3
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08005430
	mov r0, #1
	bl FadeFromBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08005430
	ldr r0, _080053D0 @ =0x00004858
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _08005430
	.align 2, 0
_080053CC: .4byte 0x0000485E
_080053D0: .4byte 0x00004858
_080053D4:
	mov r0, #4
	bl FadeToWhite
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08005430
	ldr r4, _08005434 @ =0x04000208
	mov r0, #0
	strh r0, [r4]
	ldr r3, _08005438 @ =0x04000200
	ldrh r2, [r3]
	ldr r1, _0800543C @ =0x0000FFFD
	add r0, r1, #0
	and r0, r2
	strh r0, [r3]
	ldr r0, _08005440 @ =0x03000000
	mov r2, #0
	str r2, [r0, #4]
	mov r5, #1
	strh r5, [r4]
	strh r2, [r4]
	ldrh r0, [r3]
	and r1, r0
	strh r1, [r3]
	strh r5, [r4]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xC8
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #0xA2
	lsl r0, r0, #4
	mov r2, #0xA1
	lsl r2, r2, #2
	ldr r3, _08005444 @ =0x087C056C
	mov r1, #0x90
	bl LoadBgImage4bpp
	bl Title_DrawMenu
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
_08005430:
	mov r0, #0
	b _08005456
_08005434: .4byte 0x04000208
_08005438: .4byte 0x04000200
_0800543C: .4byte 0x0000FFFD
_08005440: .4byte 0x03000000
_08005444: .4byte gTitleCopyrightImage
_08005448:
	bl Title_DrawMenu
	mov r0, #1
	bl FadeFromWhite
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_08005456:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end Title_FadeIn

