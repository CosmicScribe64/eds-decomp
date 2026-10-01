	thumb_func_start sub_0804158C
sub_0804158C: @ 0x0804158C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, _080415A8 @ =0x02017A40
	ldr r1, _080415AC @ =0x000003E5
	add r4, r0, r1
	ldrb r2, [r4]
	add r3, r0, #0
	cmp r2, #1
	beq _080415D8
	cmp r2, #1
	bgt _080415B0
	cmp r2, #0
	beq _080415B6
	b _08041614
_080415A8: .4byte 0x02017A40
_080415AC: .4byte 0x000003E5
_080415B0:
	cmp r2, #2
	beq _080415F8
	b _08041614
_080415B6:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #9
	mov r2, #0
	mov r3, #0
	bl sub_08022678
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
_080415D0:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08041692
_080415D8:
	ldr r0, _080415F0 @ =0x020192E0
	ldr r3, _080415F4 @ =0x00001B64
	add r0, r0, r3
	ldrh r1, [r0]
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r5, #0
	bl sub_0803DD7C
	b _080415D0
	.align 2, 0
_080415F0: .4byte 0x020192E0
_080415F4: .4byte 0x00001B64
_080415F8:
	ldr r0, _08041608 @ =0x00000206
	ldr r1, _0804160C @ =0x00000712
	ldr r3, _08041610 @ =0x08083E14
	mov r2, #0xB
	bl sub_080602A4
	b _080415D0
	.align 2, 0
_08041608: .4byte 0x00000206
_0804160C: .4byte 0x00000712
_08041610: .4byte gUnk_08083E14
_08041614:
	ldr r1, _0804162C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041634
	ldr r0, _08041630 @ =0x000003E5
	add r1, r3, r0
	mov r0, #0
	strb r0, [r1]
	b _08041694
	.align 2, 0
_0804162C: .4byte 0x03000040
_08041630: .4byte 0x000003E5
_08041634:
	ldr r0, _0804167C @ =0x00E000E0
	bl sub_08052F38
	cmp r0, #0
	beq _08041692
	ldr r0, _08041680 @ =0x0201CFB0
	ldr r1, _08041684 @ =0x00000824
	add r2, r0, r1
	ldr r3, _08041688 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldr r6, [r2]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r2]
	orr r1, r2
	add r0, r5, #0
	bl sub_0802B558
	cmp r0, #0
	beq _0804168C
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804168C
	mov r0, #1
	b _08041694
	.align 2, 0
_0804167C: .4byte 0x00E000E0
_08041680: .4byte 0x0201CFB0
_08041684: .4byte 0x00000824
_08041688: .4byte 0x00000828
_0804168C:
	mov r0, #3
	bl sub_08077AEC
_08041692:
	mov r0, #0
_08041694:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0804158C
	.align 2, 0

