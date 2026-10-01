	thumb_func_start sub_08021DA8
sub_08021DA8: @ 0x08021DA8
	push {r4, lr}
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, _08021DD0 @ =0x020192E0
	ldr r4, _08021DD4 @ =0x00001B62
	add r0, r2, r4
	ldrb r0, [r0]
	cmp r0, #0
	beq _08021DE0
	cmp r0, #1
	beq _08021E44
	ldr r0, _08021DD8 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	ldr r3, _08021DDC @ =0x00001B64
	add r0, r2, r3
	strh r1, [r0]
	mov r0, #1
	b _08021EAC
	.align 2, 0
_08021DD0: .4byte 0x020192E0
_08021DD4: .4byte 0x00001B62
_08021DD8: .4byte 0x0201AE60
_08021DDC: .4byte 0x00001B64
_08021DE0:
	ldr r0, _08021DEC @ =0x0000FFFF
	cmp r1, r0
	bne _08021DF0
	mov r0, #0
	b _08021E1E
	.align 2, 0
_08021DEC: .4byte 0x0000FFFF
_08021DF0:
	ldr r0, _08021E04 @ =0x000007CF
	cmp r1, r0
	bhi _08021E0C
	add r0, #0x30
	and r0, r1
	lsl r0, r0, #1
	ldr r4, _08021E08 @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	b _08021E1E
_08021E04: .4byte 0x000007CF
_08021E08: .4byte gUnk_08623DF4
_08021E0C:
	ldr r2, _08021E30 @ =0xFFFFF830
	add r0, r1, r2
	ldr r1, _08021E34 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r4, _08021E38 @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	add r0, #1
_08021E1E:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r3, #0
	bl sub_080197C0
	ldr r0, _08021E3C @ =0x020192E0
	ldr r1, _08021E40 @ =0x00001B62
	add r0, r0, r1
	b _08021EA4
_08021E30: .4byte 0xFFFFF830
_08021E34: .4byte 0x000007FF
_08021E38: .4byte gUnk_08623DF4
_08021E3C: .4byte 0x020192E0
_08021E40: .4byte 0x00001B62
_08021E44:
	ldr r0, _08021E58 @ =0x000005ED
	cmp r1, r0
	beq _08021E88
	cmp r1, r0
	bgt _08021E60
	ldr r0, _08021E5C @ =0x00000489
	cmp r1, r0
	beq _08021E6C
	b _08021E94
	.align 2, 0
_08021E58: .4byte 0x000005ED
_08021E5C: .4byte 0x00000489
_08021E60:
	ldr r0, _08021E68 @ =0x000005EF
	cmp r1, r0
	beq _08021E88
	b _08021E94
_08021E68: .4byte 0x000005EF
_08021E6C:
	ldr r0, _08021E7C @ =0x00000206
	ldr r1, _08021E80 @ =0x00000712
	ldr r3, _08021E84 @ =0x08081D34
	mov r2, #0xB
	bl sub_080602A4
	b _08021E94
	.align 2, 0
_08021E7C: .4byte 0x00000206
_08021E80: .4byte 0x00000712
_08021E84: .4byte gUnk_08081D34
_08021E88:
	ldr r0, _08021EB4 @ =0x00000206
	ldr r1, _08021EB8 @ =0x00000712
	ldr r3, _08021EBC @ =0x08081D70
	mov r2, #0xB
	bl sub_080602A4
_08021E94:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	ldr r0, _08021EC0 @ =0x020192E0
	ldr r2, _08021EC4 @ =0x00001B62
	add r0, r0, r2
_08021EA4:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r0, #0
_08021EAC:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08021EB4: .4byte 0x00000206
_08021EB8: .4byte 0x00000712
_08021EBC: .4byte gUnk_08081D70
_08021EC0: .4byte 0x020192E0
_08021EC4: .4byte 0x00001B62
	thumb_func_end sub_08021DA8

