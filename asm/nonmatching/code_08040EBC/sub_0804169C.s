	thumb_func_start sub_0804169C
sub_0804169C: @ 0x0804169C
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _080416B8 @ =0x02017A40
	ldr r2, _080416BC @ =0x000003E5
	add r5, r0, r2
	ldrb r0, [r5]
	cmp r0, #1
	beq _08041700
	cmp r0, #1
	bgt _080416C0
	cmp r0, #0
	beq _080416CA
	b _080417D2
	.align 2, 0
_080416B8: .4byte 0x02017A40
_080416BC: .4byte 0x000003E5
_080416C0:
	cmp r0, #2
	beq _08041750
	cmp r0, #3
	beq _08041764
	b _080417D2
_080416CA:
	mov r0, #8
	neg r0, r0
	ldrb r3, [r4, #0xA]
	and r0, r3
	strb r0, [r4, #0xA]
	add r0, r4, #0
	mov r2, #0
	bl sub_0802FCEC
	cmp r0, #0
	beq _080417B8
	ldr r0, _080416F4 @ =0x00000206
	ldr r1, _080416F8 @ =0x00000712
	ldr r3, _080416FC @ =0x08084B6C
_080416E6:
	mov r2, #0xB
	bl sub_080602A4
_080416EC:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _080417D2
_080416F4: .4byte 0x00000206
_080416F8: .4byte 0x00000712
_080416FC: .4byte gUnk_08084B6C
_08041700:
	ldr r1, _0804173C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08041770
	ldr r0, _08041740 @ =0x000E000E
	bl sub_08052F38
	cmp r0, #0
	beq _080417D2
	ldr r0, _08041744 @ =0x0201CFB0
	ldr r2, _08041748 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0804174C @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080416EC
	b _080417D2
	.align 2, 0
_0804173C: .4byte 0x03000040
_08041740: .4byte 0x000E000E
_08041744: .4byte 0x0201CFB0
_08041748: .4byte 0x00000824
_0804174C: .4byte 0x00000828
_08041750:
	ldr r0, _08041758 @ =0x00000206
	ldr r1, _0804175C @ =0x00000712
	ldr r3, _08041760 @ =0x08084BA0
	b _080416E6
_08041758: .4byte 0x00000206
_0804175C: .4byte 0x00000712
_08041760: .4byte gUnk_08084BA0
_08041764:
	ldr r1, _08041778 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804177C
_08041770:
	mov r0, #0
	strb r0, [r5]
	b _080417D4
	.align 2, 0
_08041778: .4byte 0x03000040
_0804177C:
	ldr r0, _080417BC @ =0x000E000E
	bl sub_08052F38
	cmp r0, #0
	beq _080417D2
	ldr r0, _080417C0 @ =0x0201CFB0
	ldr r1, _080417C4 @ =0x00000824
	add r2, r0, r1
	ldr r3, _080417C8 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	ldr r1, [r2]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	ldrb r2, [r2]
	orr r0, r2
	ldrh r2, [r4, #0xC]
	cmp r2, r0
	beq _080417CC
	add r0, r4, #0
	add r2, r3, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080417CC
_080417B8:
	mov r0, #1
	b _080417D4
_080417BC: .4byte 0x000E000E
_080417C0: .4byte 0x0201CFB0
_080417C4: .4byte 0x00000824
_080417C8: .4byte 0x00000828
_080417CC:
	mov r0, #3
	bl sub_08077AEC
_080417D2:
	mov r0, #0
_080417D4:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0804169C
	.align 2, 0

