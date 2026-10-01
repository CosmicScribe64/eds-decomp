	thumb_func_start sub_0807569C
sub_0807569C: @ 0x0807569C
	push {r4, lr}
	ldr r4, _080756F4 @ =0x03000040
	ldr r0, _080756F8 @ =0x0000040C
	add r1, r4, r0
	ldrh r2, [r1]
	mov r0, #1
	ldrh r3, [r1]
	orr r0, r2
	strh r0, [r1]
	ldr r0, _080756FC @ =0x00004864
	add r1, r4, r0
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	ldr r0, _08075700 @ =0x00004866
	add r1, r4, r0
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	mov r1, #0x83
	lsl r1, r1, #3
	add r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _080756D2
	bl _call_via_r0
_080756D2:
	bl sub_0807E3B0
	ldr r1, _08075704 @ =0x00000414
	add r0, r4, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _080756E4
	bl _call_via_r0
_080756E4:
	ldr r0, _08075708 @ =0x00004861
	add r1, r4, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_080756F4: .4byte 0x03000040
_080756F8: .4byte 0x0000040C
_080756FC: .4byte 0x00004864
_08075700: .4byte 0x00004866
_08075704: .4byte 0x00000414
_08075708: .4byte 0x00004861
	thumb_func_end sub_0807569C

