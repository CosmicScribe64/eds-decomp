	thumb_func_start sub_0804145C
sub_0804145C: @ 0x0804145C
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r0, _08041478 @ =0x02017A40
	ldr r1, _0804147C @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #1
	beq _080414B4
	cmp r0, #1
	bgt _08041480
	cmp r0, #0
	beq _0804148A
	b _08041582
	.align 2, 0
_08041478: .4byte 0x02017A40
_0804147C: .4byte 0x000003E5
_08041480:
	cmp r0, #2
	beq _080414FC
	cmp r0, #3
	beq _08041518
	b _08041582
_0804148A:
	ldr r0, _080414A8 @ =0x00000206
	ldr r1, _080414AC @ =0x00000712
	ldr r3, _080414B0 @ =0x080849F4
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
_080414A0:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08041582
_080414A8: .4byte 0x00000206
_080414AC: .4byte 0x00000712
_080414B0: .4byte gUnk_080849F4
_080414B4:
	ldr r1, _080414F0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08041524
	mov r0, #0xF0
	bl sub_08052F38
	cmp r0, #0
	beq _08041582
	ldr r0, _080414F4 @ =0x0201CFB0
	ldr r3, _080414F8 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r5, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080414A0
	b _08041582
	.align 2, 0
_080414F0: .4byte 0x03000040
_080414F4: .4byte 0x0201CFB0
_080414F8: .4byte 0x00000824
_080414FC:
	ldr r0, _0804150C @ =0x00000206
	ldr r1, _08041510 @ =0x00000712
	ldr r3, _08041514 @ =0x08084B38
	mov r2, #0xB
	bl sub_080602A4
	b _080414A0
	.align 2, 0
_0804150C: .4byte 0x00000206
_08041510: .4byte 0x00000712
_08041514: .4byte gUnk_08084B38
_08041518:
	ldr r1, _0804152C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041530
_08041524:
	mov r0, #0
	strb r0, [r4]
	b _08041584
	.align 2, 0
_0804152C: .4byte 0x03000040
_08041530:
	mov r0, #0xF0
	bl sub_08052F38
	cmp r0, #0
	beq _08041582
	ldr r0, _08041570 @ =0x0201CFB0
	ldr r1, _08041574 @ =0x00000824
	add r2, r0, r1
	ldr r3, _08041578 @ =0x00000828
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
	ldrh r2, [r5, #0xC]
	cmp r0, r2
	beq _0804157C
	add r0, r5, #0
	add r2, r3, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804157C
	mov r0, #1
	b _08041584
_08041570: .4byte 0x0201CFB0
_08041574: .4byte 0x00000824
_08041578: .4byte 0x00000828
_0804157C:
	mov r0, #3
	bl sub_08077AEC
_08041582:
	mov r0, #0
_08041584:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0804145C
	.align 2, 0

