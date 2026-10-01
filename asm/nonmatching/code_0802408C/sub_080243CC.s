	thumb_func_start sub_080243CC
sub_080243CC: @ 0x080243CC
	push {lr}
	ldr r0, _080243F4 @ =0x0201CFB0
	mov r1, #0x83
	lsl r1, r1, #4
	add r0, r0, r1
	ldrb r1, [r0]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08024438
	lsr r0, r1, #1
	sub r0, #1
	cmp r0, #4
	bhi _08024438
	lsl r0, r0, #2
	ldr r1, _080243F8 @ =0x080243FC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080243F4: .4byte 0x0201CFB0
_080243F8: .4byte 0x080243FC
_080243FC:
	.4byte _08024410
	.4byte _08024418
	.4byte _08024420
	.4byte _08024428
	.4byte _08024430
_08024410:
	bl sub_0805D58C
	mov r0, #1
	b _0802443A
_08024418:
	bl sub_0805D708
	mov r0, #1
	b _0802443A
_08024420:
	bl sub_0805D848
	mov r0, #1
	b _0802443A
_08024428:
	bl sub_0805DA1C
	mov r0, #1
	b _0802443A
_08024430:
	bl sub_0805DB90
	mov r0, #1
	b _0802443A
_08024438:
	mov r0, #0
_0802443A:
	pop {r1}
	bx r1
	thumb_func_end sub_080243CC
	.align 2, 0

