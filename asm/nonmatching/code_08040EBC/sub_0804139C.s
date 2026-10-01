	thumb_func_start sub_0804139C
sub_0804139C: @ 0x0804139C
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _080413CC @ =0x02017A40
	ldr r1, _080413D0 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _080413E0
	ldr r0, _080413D4 @ =0x00000206
	ldr r1, _080413D8 @ =0x00000712
	ldr r3, _080413DC @ =0x08084AF8
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
	b _08041452
	.align 2, 0
_080413CC: .4byte 0x02017A40
_080413D0: .4byte 0x000003E5
_080413D4: .4byte 0x00000206
_080413D8: .4byte 0x00000712
_080413DC: .4byte gUnk_08084AF8
_080413E0:
	ldr r1, _080413F4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080413F8
	mov r0, #0
	strb r0, [r5]
	b _08041454
	.align 2, 0
_080413F4: .4byte 0x03000040
_080413F8:
	ldr r0, _08041440 @ =0x00040004
	bl sub_08052F38
	cmp r0, #0
	beq _08041452
	ldr r0, _08041444 @ =0x0201CFB0
	ldr r2, _08041448 @ =0x00000824
	add r1, r0, r2
	ldr r3, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r2, r1, r0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r3, r0
	bne _0804142C
	ldrh r1, [r4, #2]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1A
	cmp r2, r0
	beq _0804144C
_0804142C:
	add r0, r4, #0
	add r1, r3, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804144C
	mov r0, #1
	b _08041454
	.align 2, 0
_08041440: .4byte 0x00040004
_08041444: .4byte 0x0201CFB0
_08041448: .4byte 0x00000824
_0804144C:
	mov r0, #3
	bl sub_08077AEC
_08041452:
	mov r0, #0
_08041454:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0804139C
	.align 2, 0

