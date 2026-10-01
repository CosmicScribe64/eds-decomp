	thumb_func_start sub_0802FD18
sub_0802FD18: @ 0x0802FD18
	push {r4, r5, r6, lr}
	add r4, r1, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FD88
	cmp r4, #0
	beq _0802FD88
	ldr r6, _0802FD74 @ =0x000007FF
	add r0, r6, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802FD78 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802FD88
	ldr r5, _0802FD7C @ =0x0000058A
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802FD88
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802FD88
	add r0, r6, #0
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0802FD80 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0802FD84 @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	beq _0802FD88
	mov r0, #1
	b _0802FD8A
_0802FD74: .4byte 0x000007FF
_0802FD78: .4byte gUnk_08621DE0
_0802FD7C: .4byte 0x0000058A
_0802FD80: .4byte gUnk_08622AB4
_0802FD84: .4byte 0x00000603
_0802FD88:
	mov r0, #0
_0802FD8A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802FD18

