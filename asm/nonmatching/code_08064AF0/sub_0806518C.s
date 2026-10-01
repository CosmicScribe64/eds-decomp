	thumb_func_start sub_0806518C
sub_0806518C: @ 0x0806518C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x90
	mov r8, r1
	add r4, r2, #0
	add r5, r3, #0
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r6, r7, #6
	ldr r0, _08065214 @ =0x0822C720
	add r6, r6, r0
	add r0, sp, #0x10
	add r1, r6, #0
	bl sub_080752D0
	bl sub_08064FCC
	mov r9, r0
	bl sub_08065034
	add r3, r0, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r4, #4
	mov r0, #0x1F
	and r4, r0
	add r5, #1
	and r5, r0
	lsl r5, r5, #5
	add r4, r4, r5
	lsl r4, r4, #1
	add r8, r4
	mov r0, #2
	str r0, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	str r0, [sp, #0xC]
	add r0, r6, #0
	mov r1, r8
	mov r2, r9
	bl sub_08079340
	bl sub_080650D4
	ldr r0, _08065218 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #1
	ldr r1, _0806521C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _08065220 @ =0xFFFFF893
	add r0, r0, r2
	cmp r0, #0xB
	bhi _08065294
	lsl r0, r0, #2
	ldr r1, _08065224 @ =0x08065228
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08065214: .4byte gUnk_0822C720
_08065218: .4byte 0x000007FF
_0806521C: .4byte gUnk_08622AB4
_08065220: .4byte 0xFFFFF893
_08065224: .4byte 0x08065228
_08065228:
	.4byte _08065280
	.4byte _08065280
	.4byte _08065280
	.4byte _08065294
	.4byte _08065294
	.4byte _08065294
	.4byte _08065294
	.4byte _08065294
	.4byte _08065294
	.4byte _08065258
	.4byte _0806526C
	.4byte _0806526C
_08065258:
	ldr r0, _08065264 @ =0x0201DB20
	ldr r1, _08065268 @ =0x00001C3B
	add r0, r0, r1
	mov r1, #3
	b _08065364
	.align 2, 0
_08065264: .4byte 0x0201DB20
_08065268: .4byte 0x00001C3B
_0806526C:
	ldr r0, _08065278 @ =0x0201DB20
	ldr r2, _0806527C @ =0x00001C3B
	add r0, r0, r2
	mov r1, #1
	b _08065364
	.align 2, 0
_08065278: .4byte 0x0201DB20
_0806527C: .4byte 0x00001C3B
_08065280:
	ldr r0, _0806528C @ =0x0201DB20
	ldr r1, _08065290 @ =0x00001C3B
	add r0, r0, r1
	mov r1, #0
	b _08065364
	.align 2, 0
_0806528C: .4byte 0x0201DB20
_08065290: .4byte 0x00001C3B
_08065294:
	ldr r2, _080652C4 @ =0x000007FF
	and r2, r7
	lsl r0, r2, #2
	ldr r1, _080652C8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _080652D4
	cmp r0, #0x16
	beq _080652E8
	lsl r0, r2, #1
	ldr r2, _080652CC @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _080652D0 @ =0x00000776
	cmp r1, r0
	bne _080652FC
	mov r1, #3
	b _0806535E
	.align 2, 0
_080652C4: .4byte 0x000007FF
_080652C8: .4byte gUnk_08621DE0
_080652CC: .4byte gUnk_08622AB4
_080652D0: .4byte 0x00000776
_080652D4:
	ldr r0, _080652E0 @ =0x0201DB20
	ldr r1, _080652E4 @ =0x00001C3B
	add r0, r0, r1
	mov r1, #5
	b _08065364
	.align 2, 0
_080652E0: .4byte 0x0201DB20
_080652E4: .4byte 0x00001C3B
_080652E8:
	ldr r0, _080652F4 @ =0x0201DB20
	ldr r2, _080652F8 @ =0x00001C3B
	add r0, r0, r2
	mov r1, #4
	b _08065364
	.align 2, 0
_080652F4: .4byte 0x0201DB20
_080652F8: .4byte 0x00001C3B
_080652FC:
	cmp r1, r0
	blt _0806530C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806530C
	mov r1, #1
	b _0806535E
_0806530C:
	ldr r0, _08065330 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _08065334 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806533E
	cmp r0, #0x16
	bgt _08065338
	cmp r0, #0x15
	beq _08065342
	b _0806534A
	.align 2, 0
_08065330: .4byte 0x000007FF
_08065334: .4byte gUnk_08621DE0
_08065338:
	cmp r0, #0x17
	beq _08065346
	b _0806534A
_0806533E:
	mov r1, #7
	b _0806535E
_08065342:
	mov r1, #8
	b _0806535E
_08065346:
	mov r1, #9
	b _0806535E
_0806534A:
	ldr r0, _08065374 @ =0x000007FF
	and r0, r7
	lsl r0, r0, #2
	ldr r2, _08065378 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r1, r0, #0x12
_0806535E:
	ldr r0, _0806537C @ =0x0201DB20
	ldr r2, _08065380 @ =0x00001C3B
	add r0, r0, r2
_08065364:
	strb r1, [r0]
	add sp, #0x90
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08065374: .4byte 0x000007FF
_08065378: .4byte gUnk_08621DE0
_0806537C: .4byte 0x0201DB20
_08065380: .4byte 0x00001C3B
	thumb_func_end sub_0806518C

