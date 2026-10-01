	thumb_func_start sub_08008300
sub_08008300: @ 0x08008300
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	ldr r0, _08008328 @ =0x0000042D
	cmp r1, r0
	beq _08008398
	cmp r1, r0
	bgt _08008348
	ldr r0, _0800832C @ =0x0000014B
	cmp r1, r0
	beq _08008388
	cmp r1, r0
	bgt _08008330
	sub r0, #2
	cmp r1, r0
	beq _08008380
	add r0, #1
	cmp r1, r0
	beq _08008384
	b _080083B8
_08008328: .4byte 0x0000042D
_0800832C: .4byte 0x0000014B
_08008330:
	ldr r0, _08008344 @ =0x0000014D
	cmp r1, r0
	beq _08008390
	cmp r1, r0
	blt _0800838C
	add r0, #1
	cmp r1, r0
	beq _08008394
	b _080083B8
	.align 2, 0
_08008344: .4byte 0x0000014D
_08008348:
	mov r0, #0x8D
	lsl r0, r0, #3
	cmp r1, r0
	beq _080083A8
	cmp r1, r0
	bgt _08008366
	sub r0, #2
	cmp r1, r0
	beq _080083A0
	cmp r1, r0
	bgt _080083A4
	sub r0, #1
	cmp r1, r0
	beq _0800839C
	b _080083B8
_08008366:
	ldr r0, _08008378 @ =0x0000046A
	cmp r2, r0
	beq _080083B0
	cmp r2, r0
	blt _080083AC
	ldr r0, _0800837C @ =0x0000060B
	cmp r2, r0
	beq _080083B4
	b _080083B8
_08008378: .4byte 0x0000046A
_0800837C: .4byte 0x0000060B
_08008380:
	mov r0, #1
	b _080083BA
_08008384:
	mov r0, #2
	b _080083BA
_08008388:
	mov r0, #3
	b _080083BA
_0800838C:
	mov r0, #4
	b _080083BA
_08008390:
	mov r0, #5
	b _080083BA
_08008394:
	mov r0, #6
	b _080083BA
_08008398:
	mov r0, #7
	b _080083BA
_0800839C:
	mov r0, #8
	b _080083BA
_080083A0:
	mov r0, #9
	b _080083BA
_080083A4:
	mov r0, #0xA
	b _080083BA
_080083A8:
	mov r0, #0xB
	b _080083BA
_080083AC:
	mov r0, #0xC
	b _080083BA
_080083B0:
	mov r0, #0xD
	b _080083BA
_080083B4:
	mov r0, #0xE
	b _080083BA
_080083B8:
	mov r0, #0
_080083BA:
	bx lr
	thumb_func_end sub_08008300

