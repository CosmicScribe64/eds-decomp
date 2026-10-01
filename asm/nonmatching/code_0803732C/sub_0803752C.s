	thumb_func_start sub_0803752C
sub_0803752C: @ 0x0803752C
	push {lr}
	add r2, r0, #0
	ldr r0, _08037568 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _080375EC
	mov r0, #4
	ldrb r1, [r2, #4]
	and r0, r1
	cmp r0, #0
	bne _080375EC
	ldr r0, _0803756C @ =0x000007FF
	ldrh r1, [r2]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08037570 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08037574 @ =0x00000471
	cmp r1, r0
	beq _080375A0
	cmp r1, r0
	bgt _08037578
	sub r0, #1
	cmp r1, r0
	beq _0803758C
	b _080375EC
_08037568: .4byte 0x02017A40
_0803756C: .4byte 0x000007FF
_08037570: .4byte gUnk_08622AB4
_08037574: .4byte 0x00000471
_08037578:
	ldr r0, _08037588 @ =0x00000472
	cmp r1, r0
	beq _080375B4
	add r0, #1
	cmp r1, r0
	beq _080375C8
	b _080375EC
	.align 2, 0
_08037588: .4byte 0x00000472
_0803758C:
	mov r0, #1
	ldrb r2, [r2, #2]
	and r0, r2
	mov r1, #0x15
	cmp r0, #0
	beq _080375D6
	ldr r1, _0803759C @ =0x00008015
	b _080375D6
_0803759C: .4byte 0x00008015
_080375A0:
	mov r0, #1
	ldrb r2, [r2, #2]
	and r0, r2
	mov r1, #0x16
	cmp r0, #0
	beq _080375D6
	ldr r1, _080375B0 @ =0x00008016
	b _080375D6
_080375B0: .4byte 0x00008016
_080375B4:
	mov r0, #1
	ldrb r2, [r2, #2]
	and r0, r2
	mov r1, #0x18
	cmp r0, #0
	beq _080375D6
	ldr r1, _080375C4 @ =0x00008018
	b _080375D6
_080375C4: .4byte 0x00008018
_080375C8:
	mov r0, #1
	ldrb r2, [r2, #2]
	and r0, r2
	mov r1, #0x17
	cmp r0, #0
	beq _080375D6
	ldr r1, _080375E8 @ =0x00008017
_080375D6:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x7F
	b _080375F4
	.align 2, 0
_080375E8: .4byte 0x00008017
_080375EC:
	mov r0, #0
	bl sub_0804325C
	mov r0, #0
_080375F4:
	pop {r1}
	bx r1
	thumb_func_end sub_0803752C

