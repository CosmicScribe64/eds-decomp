	thumb_func_start sub_0804F384
sub_0804F384: @ 0x0804F384
	push {r4, r5, r6, lr}
	ldr r4, _0804F3B8 @ =0x0201AE60
	add r3, r4, #0
	add r3, #0x22
	ldrb r2, [r3]
	add r1, r2, #0
	cmp r1, #1
	beq _0804F3C0
	cmp r1, #2
	beq _0804F3D6
	ldr r1, _0804F3BC @ =0x03000040
	mov r0, #0x80
	ldrh r2, [r1, #6]
	and r0, r2
	add r6, r1, #0
	cmp r0, #0
	beq _0804F3EC
	mov r0, #0
	bl sub_08077AEC
	add r1, r4, #0
	ldrh r0, [r1, #0x14]
	cmp r0, #1
	bhi _0804F3E8
	b _0804F3DA
	.align 2, 0
_0804F3B8: .4byte 0x0201AE60
_0804F3BC: .4byte 0x03000040
_0804F3C0:
	add r1, r4, #0
	add r1, #0x23
	ldrb r0, [r1]
	cmp r0, #0x3B
	bhi _0804F3D0
	add r0, #1
	strb r0, [r1]
	b _0804F452
_0804F3D0:
	add r0, r2, #1
	strb r0, [r3]
	b _0804F452
_0804F3D6:
	mov r0, #1
	b _0804F454
_0804F3DA:
	add r0, #1
	strh r0, [r1, #0x14]
	ldr r6, _0804F3E4 @ =0x03000040
	b _0804F3EC
	.align 2, 0
_0804F3E4: .4byte 0x03000040
_0804F3E8:
	mov r0, #0
	strh r0, [r4, #0x14]
_0804F3EC:
	mov r0, #0x40
	ldrh r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	beq _0804F410
	mov r0, #0
	bl sub_08077AEC
	ldr r1, _0804F408 @ =0x0201AE60
	ldrh r0, [r1, #0x14]
	cmp r0, #0
	beq _0804F40C
	sub r0, #1
	b _0804F40E
_0804F408: .4byte 0x0201AE60
_0804F40C:
	mov r0, #2
_0804F40E:
	strh r0, [r1, #0x14]
_0804F410:
	mov r5, #1
	mov r0, #1
	ldrh r2, [r6, #6]
	and r0, r2
	cmp r0, #0
	beq _0804F430
	mov r0, #1
	bl sub_08077AEC
	ldr r0, _0804F45C @ =0x0201AE60
	add r2, r0, #0
	add r2, #0x22
	mov r1, #0
	strb r5, [r2]
	add r0, #0x23
	strb r1, [r0]
_0804F430:
	mov r4, #2
	add r0, r4, #0
	ldrh r6, [r6, #6]
	and r0, r6
	cmp r0, #0
	beq _0804F452
	mov r0, #2
	bl sub_08077AEC
	ldr r0, _0804F45C @ =0x0201AE60
	mov r1, #0
	strh r4, [r0, #0x14]
	add r2, r0, #0
	add r2, #0x22
	strb r5, [r2]
	add r0, #0x23
	strb r1, [r0]
_0804F452:
	mov r0, #0
_0804F454:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0804F45C: .4byte 0x0201AE60
	thumb_func_end sub_0804F384

