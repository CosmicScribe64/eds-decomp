	thumb_func_start sub_08052810
sub_08052810: @ 0x08052810
	push {r4, lr}
	cmp r0, #0
	beq _08052848
	bl sub_080578F4
	ldr r1, _0805283C @ =0x020192E0
	ldr r2, _08052840 @ =0x00001B64
	add r1, r1, r2
	strh r0, [r1]
	ldr r1, _08052844 @ =0x00008008
	lsl r0, r0, #0x18
	mov r2, #0xB0
	lsl r2, r2, #0xC
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r1, #0
	mov r1, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #1
	b _080528F2
_0805283C: .4byte 0x020192E0
_08052840: .4byte 0x00001B64
_08052844: .4byte 0x00008008
_08052848:
	ldr r2, _08052878 @ =0x020192E0
	ldr r3, _0805287C @ =0x00001B62
	add r4, r2, r3
	ldrb r0, [r4]
	cmp r0, #0
	beq _08052890
	cmp r0, #1
	beq _080528AC
	ldr r0, _08052880 @ =0x00000D68
	add r1, r2, r0
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1, #9]
	and r0, r3
	strb r0, [r1, #9]
	ldr r0, _08052884 @ =0x0201CFB0
	ldr r1, _08052888 @ =0x0000082C
	add r0, r0, r1
	ldr r1, [r0]
	ldr r3, _0805288C @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
	b _080528F2
_08052878: .4byte 0x020192E0
_0805287C: .4byte 0x00001B62
_08052880: .4byte 0x00000D68
_08052884: .4byte 0x0201CFB0
_08052888: .4byte 0x0000082C
_0805288C: .4byte 0x00001B64
_08052890:
	ldr r0, _080528A0 @ =0x00000206
	ldr r1, _080528A4 @ =0x00000712
	ldr r3, _080528A8 @ =0x08086298
	mov r2, #0xB
	bl sub_080602A4
	b _080528EA
	.align 2, 0
_080528A0: .4byte 0x00000206
_080528A4: .4byte 0x00000712
_080528A8: .4byte gUnk_08086298
_080528AC:
	ldr r0, _080528F8 @ =0x00000D68
	add r1, r2, r0
	mov r0, #1
	ldrb r2, [r1, #9]
	orr r0, r2
	strb r0, [r1, #9]
	mov r0, #0x80
	lsl r0, r0, #9
	bl sub_08052F38
	cmp r0, #0
	beq _080528F0
	mov r0, #1
	bl sub_08077AEC
	ldr r0, _080528FC @ =0x0201CFB0
	ldr r3, _08052900 @ =0x00000824
	add r1, r0, r3
	ldrh r1, [r1]
	ldr r2, _08052904 @ =0x00000828
	add r3, r0, r2
	add r2, #4
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r2, r0, #8
	ldrb r3, [r3]
	orr r2, r3
	mov r0, #8
	mov r3, #0
	bl sub_0801EC58
_080528EA:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080528F0:
	mov r0, #0
_080528F2:
	pop {r4}
	pop {r1}
	bx r1
_080528F8: .4byte 0x00000D68
_080528FC: .4byte 0x0201CFB0
_08052900: .4byte 0x00000824
_08052904: .4byte 0x00000828
	thumb_func_end sub_08052810

