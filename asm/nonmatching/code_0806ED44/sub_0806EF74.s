	thumb_func_start sub_0806EF74
sub_0806EF74: @ 0x0806EF74
	push {r4, r5, r6, lr}
	ldr r1, _0806EF8C @ =0x03000040
	ldr r2, _0806EF90 @ =0x00004859
	add r0, r1, r2
	ldrb r0, [r0]
	add r2, r1, #0
	cmp r0, #0
	beq _0806EF94
	cmp r0, #1
	beq _0806EFA8
	b _0806EFB6
	.align 2, 0
_0806EF8C: .4byte 0x03000040
_0806EF90: .4byte 0x00004859
_0806EF94:
	ldr r3, _0806EFA4 @ =0x00004874
	add r1, r2, r3
	mov r0, #4
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0806EFB4
	.align 2, 0
_0806EFA4: .4byte 0x00004874
_0806EFA8:
	ldr r1, _0806F000 @ =0x0201DB20
	ldr r0, _0806F004 @ =0x00001C5A
	add r1, r1, r0
	mov r0, #0x20
	ldrb r3, [r1]
	orr r0, r3
_0806EFB4:
	strb r0, [r1]
_0806EFB6:
	ldr r1, _0806F008 @ =0x081A72A0
	ldr r0, _0806F00C @ =0x00004859
	add r4, r2, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r1, [r0]
	cmp r1, #0
	beq _0806F014
	ldr r0, _0806F000 @ =0x0201DB20
	ldr r3, _0806F004 @ =0x00001C5A
	add r6, r0, r3
	ldrb r2, [r6]
	lsl r0, r2, #0x1B
	lsr r5, r0, #0x1D
	bl _call_via_r1
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806EFE4
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806EFE4:
	ldrb r6, [r6]
	lsl r2, r6, #0x1B
	lsr r0, r2, #0x1D
	cmp r5, r0
	beq _0806EFFC
	ldr r0, _0806F010 @ =0x080875EC
	lsr r2, r2, #0x1D
	add r1, r5, #0
	bl sub_0801A7DC
	bl sub_0801A7E8
_0806EFFC:
	mov r0, #0
	b _0806F016
_0806F000: .4byte 0x0201DB20
_0806F004: .4byte 0x00001C5A
_0806F008: .4byte gUnk_081A72A0
_0806F00C: .4byte 0x00004859
_0806F010: .4byte gUnk_080875EC
_0806F014:
	mov r0, #1
_0806F016:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0806EF74

