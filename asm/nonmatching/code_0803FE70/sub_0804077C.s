	thumb_func_start sub_0804077C
sub_0804077C: @ 0x0804077C
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _080407AC @ =0x02017A40
	ldr r1, _080407B0 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _080407C0
	ldr r0, _080407B4 @ =0x00000206
	ldr r1, _080407B8 @ =0x00000712
	ldr r3, _080407BC @ =0x08084788
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0804080A
_080407AC: .4byte 0x02017A40
_080407B0: .4byte 0x000003E5
_080407B4: .4byte 0x00000206
_080407B8: .4byte 0x00000712
_080407BC: .4byte gUnk_08084788
_080407C0:
	ldr r1, _080407D4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080407D8
	mov r0, #0
	strb r0, [r5]
	b _0804080A
	.align 2, 0
_080407D4: .4byte 0x03000040
_080407D8:
	ldr r0, _080407E8 @ =0x00D000D0
	bl sub_08052F38
	cmp r0, #0
	bne _080407EC
	mov r0, #0
	b _0804080A
	.align 2, 0
_080407E8: .4byte 0x00D000D0
_080407EC:
	ldr r0, _08040810 @ =0x0201CFB0
	ldr r3, _08040814 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	mov r0, #1
_0804080A:
	pop {r4, r5}
	pop {r1}
	bx r1
_08040810: .4byte 0x0201CFB0
_08040814: .4byte 0x00000824
	thumb_func_end sub_0804077C

