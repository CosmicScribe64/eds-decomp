	thumb_func_start sub_0803447C
sub_0803447C: @ 0x0803447C
	push {r4, lr}
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	beq _0803448C
	b _0803463C
_0803448C:
	ldr r1, _080344AC @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x7B
	add r2, r1, #0
	cmp r0, #5
	bls _080344A0
	b _0803463C
_080344A0:
	lsl r0, r0, #2
	ldr r1, _080344B0 @ =0x080344B4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080344AC: .4byte 0x02017A40
_080344B0: .4byte 0x080344B4
_080344B4:
	.4byte _0803462C
	.4byte _080345E0
	.4byte _08034588
	.4byte _0803455C
	.4byte _08034514
	.4byte _080344CC
_080344CC:
	ldr r0, _080344E8 @ =0x000007FF
	ldrh r1, [r3]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080344EC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080344F0 @ =0x000003F3
	cmp r1, r0
	beq _080344F4
	add r0, #0xD
	cmp r1, r0
	beq _08034500
	b _08034508
_080344E8: .4byte 0x000007FF
_080344EC: .4byte gUnk_08622AB4
_080344F0: .4byte 0x000003F3
_080344F4:
	ldr r0, _080344FC @ =0x000003E1
	add r1, r2, r0
	mov r0, #2
	b _08034506
_080344FC: .4byte 0x000003E1
_08034500:
	ldr r0, _08034544 @ =0x000003E1
	add r1, r2, r0
	mov r0, #5
_08034506:
	strb r0, [r1]
_08034508:
	mov r0, #0xF8
	lsl r0, r0, #2
	add r1, r2, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_08034514:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08034548 @ =0x000007FF
	ldrh r3, [r3]
	and r1, r3
	lsl r1, r1, #1
	ldr r2, _0803454C @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	bgt _08034534
	b _0803463C
_08034534:
	ldr r0, _08034550 @ =0x00000206
	ldr r1, _08034554 @ =0x00000712
	ldr r3, _08034558 @ =0x08082CA8
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7E
	b _0803463E
_08034544: .4byte 0x000003E1
_08034548: .4byte 0x000007FF
_0803454C: .4byte gUnk_08622AB4
_08034550: .4byte 0x00000206
_08034554: .4byte 0x00000712
_08034558: .4byte gUnk_08082CA8
_0803455C:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _08034580 @ =0x000007FF
	ldrh r3, [r3]
	and r2, r3
	lsl r2, r2, #1
	ldr r3, _08034584 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7D
	b _0803463E
	.align 2, 0
_08034580: .4byte 0x000007FF
_08034584: .4byte gUnk_08622AB4
_08034588:
	ldr r2, _080345D0 @ =0x0201D810
	ldrb r1, [r2, #5]
	lsl r0, r1, #0x1E
	lsr r1, r0, #0x1E
	ldrh r3, [r2, #6]
	add r1, r1, r3
	lsl r1, r1, #2
	add r2, #0xC
	add r4, r1, r2
	lsr r0, r0, #0x1E
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x13
	mov r3, #0xD4
	cmp r0, #0
	bge _080345AE
	ldr r3, _080345D4 @ =0x000080D4
_080345AE:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _080345D8 @ =0x02017A40
	ldr r2, _080345DC @ =0x000003E1
	add r0, r0, r2
	ldrb r1, [r0]
	sub r1, #1
	strb r1, [r0]
	lsl r1, r1, #0x18
	cmp r1, #0
	beq _0803463C
	mov r0, #0x7C
	b _0803463E
_080345D0: .4byte 0x0201D810
_080345D4: .4byte 0x000080D4
_080345D8: .4byte 0x02017A40
_080345DC: .4byte 0x000003E1
_080345E0:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08034618 @ =0x000007FF
	ldrh r3, [r3]
	and r1, r3
	lsl r1, r1, #1
	ldr r2, _0803461C @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	beq _0803463C
	ldr r0, _08034620 @ =0x00000206
	ldr r1, _08034624 @ =0x00000712
	ldr r3, _08034628 @ =0x08082CE0
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7B
	b _0803463E
_08034618: .4byte 0x000007FF
_0803461C: .4byte gUnk_08622AB4
_08034620: .4byte 0x00000206
_08034624: .4byte 0x00000712
_08034628: .4byte gUnk_08082CE0
_0803462C:
	ldr r0, _08034638 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0803463C
	mov r0, #0x7F
	b _0803463E
_08034638: .4byte 0x0201AE60
_0803463C:
	mov r0, #0
_0803463E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0803447C

