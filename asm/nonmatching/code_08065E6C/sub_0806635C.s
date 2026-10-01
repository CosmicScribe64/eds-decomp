	thumb_func_start sub_0806635C
sub_0806635C: @ 0x0806635C
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r0, _08066380 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _08066384 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _08066388 @ =0xFFFFF893
	add r0, r0, r1
	cmp r0, #0xB
	bhi _080663C4
	lsl r0, r0, #2
	ldr r1, _0806638C @ =0x08066390
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08066380: .4byte 0x000007FF
_08066384: .4byte gUnk_08622AB4
_08066388: .4byte 0xFFFFF893
_0806638C: .4byte 0x08066390
_08066390:
	.4byte _080663C0
	.4byte _080663C0
	.4byte _080663C0
	.4byte _080663C4
	.4byte _080663C4
	.4byte _080663C4
	.4byte _080663C4
	.4byte _080663C4
	.4byte _080663C4
	.4byte _080663EE
	.4byte _08066418
	.4byte _08066418
_080663C0:
	mov r0, #0
	b _0806646E
_080663C4:
	ldr r2, _080663F4 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _080663F8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08066404
	cmp r0, #0x16
	beq _08066408
	lsl r0, r2, #1
	ldr r1, _080663FC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08066400 @ =0x00000776
	cmp r1, r0
	bne _0806640C
_080663EE:
	mov r0, #3
	b _0806646E
	.align 2, 0
_080663F4: .4byte 0x000007FF
_080663F8: .4byte gUnk_08621DE0
_080663FC: .4byte gUnk_08622AB4
_08066400: .4byte 0x00000776
_08066404:
	mov r0, #5
	b _0806646E
_08066408:
	mov r0, #4
	b _0806646E
_0806640C:
	cmp r1, r0
	blt _0806641C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806641C
_08066418:
	mov r0, #1
	b _0806646E
_0806641C:
	ldr r0, _08066440 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08066444 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806644E
	cmp r0, #0x16
	bgt _08066448
	cmp r0, #0x15
	beq _08066452
	b _0806645A
	.align 2, 0
_08066440: .4byte 0x000007FF
_08066444: .4byte gUnk_08621DE0
_08066448:
	cmp r0, #0x17
	beq _08066456
	b _0806645A
_0806644E:
	mov r0, #7
	b _0806646E
_08066452:
	mov r0, #8
	b _0806646E
_08066456:
	mov r0, #9
	b _0806646E
_0806645A:
	ldr r0, _08066470 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08066474 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806646E:
	bx lr
_08066470: .4byte 0x000007FF
_08066474: .4byte gUnk_08621DE0
	thumb_func_end sub_0806635C

