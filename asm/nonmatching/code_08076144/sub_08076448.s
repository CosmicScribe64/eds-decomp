	thumb_func_start sub_08076448
sub_08076448: @ 0x08076448
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	str r4, [sp, #0]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	lsl r1, r0, #0x10
	lsr r2, r1, #0x10
	lsr r6, r0, #0x10
	mov r7, #0xFF
	lsl r7, r7, #8
	and r7, r4
	lsl r0, r4, #8
	ldr r5, _080764BC @ =0xFFFFFE00
	add r1, r5, #0
	and r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov ip, r0
	lsr r0, r3, #0x10
	mov r8, r0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	ldr r0, _080764C0 @ =0x03000040
	ldr r3, _080764C4 @ =0x00004830
	add r1, r0, r3
	add r3, r0, #0
	ldrb r1, [r1]
	cmp r1, #0x80
	bne _08076494
	b _08076604
_08076494:
	ldr r5, _080764C8 @ =0x00004831
	add r0, r3, r5
	ldrb r0, [r0]
	cmp r0, #0x20
	bne _080764A0
	b _08076604
_080764A0:
	ldr r0, _080764CC @ =0x00004040
	cmp r4, r0
	beq _0807656A
	cmp r4, r0
	bgt _080764DE
	cmp r4, #0x80
	beq _08076584
	cmp r4, #0x80
	bgt _080764D0
	cmp r4, #0
	beq _08076520
	cmp r4, #0x40
	beq _0807654E
	b _080765B6
_080764BC: .4byte 0xFFFFFE00
_080764C0: .4byte 0x03000040
_080764C4: .4byte 0x00004830
_080764C8: .4byte 0x00004831
_080764CC: .4byte 0x00004040
_080764D0:
	cmp r4, #0xC0
	beq _080765A6
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r4, r0
	beq _08076542
	b _080765B6
_080764DE:
	mov r0, #0x80
	lsl r0, r0, #8
	cmp r4, r0
	beq _0807652A
	cmp r4, r0
	bgt _080764FA
	mov r0, #0x81
	lsl r0, r0, #7
	cmp r4, r0
	beq _08076576
	add r0, #0x40
	cmp r4, r0
	beq _08076598
	b _080765B6
_080764FA:
	ldr r0, _0807650C @ =0x00008080
	cmp r4, r0
	beq _0807655C
	cmp r4, r0
	bgt _08076510
	sub r0, #0x40
	cmp r4, r0
	beq _08076536
	b _080765B6
_0807650C: .4byte 0x00008080
_08076510:
	ldr r0, _0807651C @ =0x000080C0
	ldr r1, [sp, #0]
	cmp r1, r0
	beq _08076592
	b _080765B6
	.align 2, 0
_0807651C: .4byte 0x000080C0
_08076520:
	sub r0, r2, #4
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	sub r0, r6, #4
	b _080765B2
_0807652A:
	sub r0, r2, #4
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #8
	b _080765B2
_08076536:
	sub r0, r2, #4
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _080765B2
_08076542:
	add r0, r2, #0
	sub r0, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	sub r0, r6, #4
	b _080765B2
_0807654E:
	add r0, r2, #0
	sub r0, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #8
	b _080765B2
_0807655C:
	add r0, r2, #0
	sub r0, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _080765B2
_0807656A:
	add r0, r2, #0
	sub r0, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	sub r0, r6, #4
	b _080765B2
_08076576:
	add r0, r2, #0
	sub r0, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #8
	b _080765B2
_08076584:
	add r0, r2, #0
	sub r0, #0x10
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _080765B2
_08076592:
	add r0, r2, #0
	sub r0, #0x10
	b _080765AA
_08076598:
	add r0, r2, #0
	sub r0, #0x20
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x10
	b _080765B2
_080765A6:
	add r0, r2, #0
	sub r0, #0x20
_080765AA:
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r6, #0
	sub r0, #0x20
_080765B2:
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
_080765B6:
	ldr r4, _08076614 @ =0x00004830
	add r5, r3, r4
	ldrb r0, [r5]
	lsl r1, r0, #3
	ldr r4, _08076618 @ =0x00004430
	add r0, r3, r4
	add r1, r1, r0
	mov r0, #0xFF
	and r6, r0
	orr r7, r6
	mov r4, #0x9C
	lsl r4, r4, #6
	add r0, r4, #0
	orr r7, r0
	strh r7, [r1]
	ldr r0, _0807661C @ =0x000001FF
	and r2, r0
	mov r0, ip
	orr r2, r0
	ldr r0, _08076620 @ =0x00004831
	add r4, r3, r0
	ldrb r3, [r4]
	lsl r0, r3, #9
	orr r2, r0
	strh r2, [r1, #2]
	mov r2, sl
	lsl r0, r2, #1
	strh r0, [r1, #4]
	ldrb r0, [r4]
	mov r1, r8
	mov r2, r9
	bl sub_08076160
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08076604:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08076614: .4byte 0x00004830
_08076618: .4byte 0x00004430
_0807661C: .4byte 0x000001FF
_08076620: .4byte 0x00004831
	thumb_func_end sub_08076448

