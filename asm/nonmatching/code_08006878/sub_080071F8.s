	thumb_func_start sub_080071F8
sub_080071F8: @ 0x080071F8
	push {lr}
	bl sub_080064AC
	ldr r0, _08007218 @ =0x03000040
	ldr r1, _0800721C @ =0x0000485A
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #5
	bls _0800720C
	b _08007364
_0800720C:
	lsl r0, r0, #2
	ldr r1, _08007220 @ =0x08007224
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08007218: .4byte 0x03000040
_0800721C: .4byte 0x0000485A
_08007220: .4byte 0x08007224
_08007224:
	.4byte _0800723C
	.4byte _08007278
	.4byte _0800727E
	.4byte _08007340
	.4byte _0800724C
	.4byte _08007278
_0800723C:
	ldr r0, _08007248 @ =0x02013D90
	add r0, #0x40
	mov r1, #1
	strh r1, [r0]
	b _0800734A
	.align 2, 0
_08007248: .4byte 0x02013D90
_0800724C:
	ldr r0, _08007268 @ =0x02013D90
	add r2, r0, #0
	add r2, #0x40
	ldrh r1, [r2]
	ldr r0, _0800726C @ =0x00000333
	cmp r1, r0
	bhi _0800734A
	add r0, r1, #1
	strh r0, [r2]
	ldr r0, _08007270 @ =0x03000040
	ldr r1, _08007274 @ =0x0000485A
	add r0, r0, r1
	mov r1, #1
	b _08007354
_08007268: .4byte 0x02013D90
_0800726C: .4byte 0x00000333
_08007270: .4byte 0x03000040
_08007274: .4byte 0x0000485A
_08007278:
	bl sub_08006ABC
	b _08007344
_0800727E:
	ldr r2, _080072AC @ =0x02013D90
	add r1, r2, #0
	add r1, #0x40
	ldrh r0, [r1]
	strh r0, [r2, #2]
	ldrh r3, [r1]
	ldr r0, _080072B0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080072B4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080072C2
	cmp r0, #0x17
	ble _080072B8
	cmp r0, #0x18
	beq _080072BC
	b _080072C2
_080072AC: .4byte 0x02013D90
_080072B0: .4byte 0x000007FF
_080072B4: .4byte gUnk_08621DE0
_080072B8:
	mov r0, #0
	b _080072D8
_080072BC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080072D8
_080072C2:
	ldr r0, _08007304 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _08007308 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080072D8:
	str r0, [r2, #0x2C]
	add r0, r2, #0
	add r0, #0x40
	ldrh r3, [r0]
	ldr r0, _08007304 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08007308 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08007316
	cmp r0, #0x17
	ble _0800730C
	cmp r0, #0x18
	beq _08007310
	b _08007316
	.align 2, 0
_08007304: .4byte 0x000007FF
_08007308: .4byte gUnk_08621DE0
_0800730C:
	mov r0, #0
	b _0800732C
_08007310:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0800732C
_08007316:
	ldr r0, _08007334 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _08007338 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _0800733C @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0800732C:
	str r0, [r2, #0x30]
	bl sub_08006B80
	b _0800734A
_08007334: .4byte 0x000007FF
_08007338: .4byte gUnk_08621DE0
_0800733C: .4byte 0x000001FF
_08007340:
	bl sub_08006A98
_08007344:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08007356
_0800734A:
	ldr r0, _0800735C @ =0x03000040
	ldr r1, _08007360 @ =0x0000485A
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
_08007354:
	strb r1, [r0]
_08007356:
	mov r0, #0
	b _08007366
	.align 2, 0
_0800735C: .4byte 0x03000040
_08007360: .4byte 0x0000485A
_08007364:
	mov r0, #1
_08007366:
	pop {r1}
	bx r1
	thumb_func_end sub_080071F8
	.align 2, 0

