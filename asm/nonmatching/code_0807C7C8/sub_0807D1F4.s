	thumb_func_start sub_0807D1F4
sub_0807D1F4: @ 0x0807D1F4
	push {r4, r5, lr}
	ldr r4, _0807D248 @ =0x0201F780
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r3, [r4, #3]
	lsl r2, r3, #0x1B
	lsr r2, r2, #0x1C
	bl sub_0807CCAC
	add r0, r4, #0
	add r0, #0x22
	ldrb r0, [r0]
	ldr r5, _0807D24C @ =0x000007FF
	add r1, r5, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	ldr r3, _0807D250 @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	add r2, r4, #0
	add r2, #0x10
	bl sub_0807BCFC
	cmp r0, #0
	beq _0807D2A8
	add r0, r4, #4
	add r1, r4, #0
	add r1, #0x14
	bl sub_0807BE60
	ldrh r1, [r4, #0x16]
	add r2, r1, #0
	ldr r0, _0807D254 @ =0x0000FFFF
	cmp r1, r0
	bne _0807D258
	mov r0, #0
	b _0807D284
	.align 2, 0
_0807D248: .4byte 0x0201F780
_0807D24C: .4byte 0x000007FF
_0807D250: .4byte gUnk_08622AB4
_0807D254: .4byte 0x0000FFFF
_0807D258:
	ldr r0, _0807D26C @ =0x000007CF
	cmp r1, r0
	bhi _0807D274
	and r1, r5
	lsl r0, r1, #1
	ldr r1, _0807D270 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0807D284
	.align 2, 0
_0807D26C: .4byte 0x000007CF
_0807D270: .4byte gUnk_08623DF4
_0807D274:
	ldr r3, _0807D29C @ =0xFFFFF830
	add r0, r2, r3
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _0807D2A0 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_0807D284:
	ldr r4, _0807D2A4 @ =0x0201F780
	strh r0, [r4, #0x20]
	ldrh r0, [r4, #0x20]
	bl sub_08077498
	ldrh r0, [r4]
	bl sub_0807761C
	bl sub_080754BC
	mov r0, #1
	b _0807D2C0
_0807D29C: .4byte 0xFFFFF830
_0807D2A0: .4byte gUnk_08623DF4
_0807D2A4: .4byte 0x0201F780
_0807D2A8:
	ldrh r0, [r4, #0x1E]
	sub r0, #1
	strh r0, [r4, #0x1E]
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0807D2BE
	ldr r0, _0807D2C8 @ =0x03000040
	ldr r2, _0807D2CC @ =0x00004859
	add r0, r0, r2
	mov r1, #0xE
	strb r1, [r0]
_0807D2BE:
	mov r0, #0
_0807D2C0:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0807D2C8: .4byte 0x03000040
_0807D2CC: .4byte 0x00004859
	thumb_func_end sub_0807D1F4

